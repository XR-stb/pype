#pragma once

#include <queue>
#include <sstream>

#include "PainterEngine_Application.h"
#include "../architecture/PainterEngine.h"

#include "pocketpy.h"
#include "_generated.h"

using namespace pkpy;

inline PX_Application App;
inline VM* vm = nullptr;                    // global python vm
inline PyObject* g_mod = nullptr;           // global PainterEngine module
inline PyObject* g_root = nullptr;          // global root gameObject

// 层序遍历对象树
inline void traverse(PX_Object* obj, std::function<void(PX_Object*)> f){
    if(obj == NULL) return;
    // 层序遍历
    std::queue<PX_Object*> q;
    q.push(obj);
    while(!q.empty()){
        PX_Object* cur = q.front(); q.pop();
        f(cur);    // 调用自定义函数
        PX_Object* child = cur->pChilds;
        while(child != NULL){
            q.push(child);
            child = child->pNextBrother;
        }
    }
}

struct GameObject {
    PY_CLASS(GameObject, PainterEngine, GameObject)

    PX_Object* obj;

    GameObject(){
        PX_Object* px_root = nullptr;
        if(g_root != nullptr){
            px_root = CAST(GameObject&, g_root).obj;
        }
        obj = PX_ObjectCreate(&App.runtime.mp_game, px_root, 0, 0, 0, 0, 0, 0);

        obj->Func_ObjectUpdate = [](PX_Object* obj, unsigned int _){
            PyObject* py = (PyObject*)obj->User_ptr;
            static StrName m_update = "_update";
            PyObject* self;
            PyObject* callable = vm->get_unbound_method(py, m_update, &self);
            if(self == PY_NULL) return;
            try{
                vm->call_method(self, callable);
            }catch(const pkpy::Exception& e){
                std::cerr << e.summary() << std::endl;
                exit(1);
            }
        };
    }

    static void _register(VM* vm, PyObject* mod, PyObject* type);
};

struct PX_ChildrenIter: BaseIter{
    PyObject* ref;
    PX_Object* curr;

    PX_ChildrenIter(VM* vm, PyObject* ref): BaseIter(vm){
        this->ref = ref;    // ref of GameObject
        GameObject& go = CAST(GameObject&, ref);
        curr = go.obj->pChilds;
    }

    PyObject* next() override{
        if(curr == NULL) return nullptr;
        PyObject* ret = (PyObject*)curr->User_ptr;
        curr = curr->pNextBrother;
        return ret;
    }

    void _gc_mark() const override {
        OBJ_MARK(ref);
    }
};

inline void python_init(){
    // vm init
    vm = pkpy_new_vm(true);
    vm->bind_builtin_func<0>("input", [](VM* vm, ArgsView args){
        return VAR(pkpy::getline());
    });
    g_mod = vm->new_module("PainterEngine");
    PyObject* go_type = GameObject::register_class(vm, g_mod);

    vm->bind_func<1>(g_mod, "Destroy", [](VM* vm, ArgsView args){
        GameObject& self = CAST(GameObject&, args[0]);
        PX_ObjectDelete(self.obj);
        self.obj = NULL;
        return vm->None;
    });

    /*************全局私有函数*************/
    vm->bind_func<1>(g_mod, "_PX_LoadTextureFromFile", [](VM* vm, ArgsView args){
        const Str& path = CAST(Str&, args[0]);
        char* path_c = path.c_str_dup();
        px_texture* tex = (px_texture*)malloc(sizeof(px_texture));
        bool ok = PX_LoadTextureFromFile(&App.runtime.mp_resources, tex, path_c);
        free(path_c);
        if(!ok){
            free(tex);
            return vm->None;
        }
        return VAR_T(VoidP, tex);
    });

    CodeObject_ code = vm->compile(pe::kPythonLibs["PainterEngine"], "<PainterEngine>", EXEC_MODE);
    vm->_exec(code, g_mod);

    // 创建根对象
    try{
        g_root = vm->call(go_type, VAR("/"));
        g_mod->attr().set("_root", g_root);
    }catch(Exception& e){
        std::cerr << e.summary() << std::endl;
        exit(1);
    }
}

namespace pkpy{
    template<> inline void gc_mark<GameObject>(GameObject& go){
        // only do mark on root
        if(go.obj->pParent != NULL) return;
        PX_Object* px_root = CAST(GameObject&, g_root).obj;
        traverse(px_root, [](PX_Object* obj){
            OBJ_MARK((PyObject*)obj->User_ptr);
        });
    }
}

inline void GameObject::_register(VM* vm, PyObject* mod, PyObject* type){
    vm->bind_static_method<-1>(type, "__new__", [](VM* vm, ArgsView args){
        PyObject* go = VAR_T(GameObject);
        go->enable_instance_dict();
        GameObject& self = CAST(GameObject&, go);
        PX_ObjectSetUserPointer(self.obj, go);
        return go;
    });

    type->attr().set("position", vm->property(
        [](VM* vm, ArgsView args){
            GameObject& self = CAST(GameObject&, args[0]);
            float x = self.obj->x;
            float y = self.obj->y;
            float z = self.obj->z;
            static StrName m_Vector3("Vector3");
            PyObject* tp = g_mod->attr(m_Vector3);
            return vm->call(tp, VAR(x), VAR(y), VAR(z));
        }));

    type->attr().set("parent", vm->property(
        [](VM* vm, ArgsView args){
            GameObject& self = CAST(GameObject&, args[0]);
            if(self.obj->pParent == NULL) return vm->None;
            return (PyObject*)(self.obj->pParent->User_ptr);
        }));
    type->attr().set("children", vm->property(
        [](VM* vm, ArgsView args){
            return vm->PyIter(PX_ChildrenIter(vm, args[0]));
        }));
    type->attr().set("Width", vm->property(
        [](VM* vm, ArgsView args){
            GameObject& self = CAST(GameObject&, args[0]);
            return VAR(self.obj->Width);
        }));
    type->attr().set("Height", vm->property(
        [](VM* vm, ArgsView args){
            GameObject& self = CAST(GameObject&, args[0]);
            return VAR(self.obj->Height);
        }));
    type->attr().set("Length", vm->property(
        [](VM* vm, ArgsView args){
            GameObject& self = CAST(GameObject&, args[0]);
            return VAR(self.obj->Length);
        }));
}


// 帧率控制
struct FrameCounter{
	px_dword update_freq = 1000 / 60;
	px_dword accum_time = 0xffff;
    px_dword elapsed = 0;
	bool flag = false;

	bool do_update(px_dword* delta){
		if(flag) throw std::runtime_error("do_update() called twice in one frame");
		accum_time += *delta;
		if(accum_time >= update_freq){
            elapsed = accum_time;
            *delta = elapsed;
			accum_time = 0;
			flag = true;
		}
		return flag;
	}

	bool do_render(px_dword* delta){
		if(flag){
			flag = false;
            *delta = elapsed;
			return true;
		}
		return false;
	}
};

inline FrameCounter g_frame_counter;