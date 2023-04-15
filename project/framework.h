#pragma once

#include <queue>
#include "../architecture/PainterEngine.h"

#include "pocketpy.h"
#include "_generated.h"

using namespace pkpy;

inline const size_t _global_pool_size = 1024 * 1024 * 10;   // 10MB
inline void* _global_pool = malloc(_global_pool_size);
inline px_memorypool _global_mp = MP_Create(_global_pool, _global_pool_size);

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


// 记得写GC遍历方法
struct GameObject {
    PY_CLASS(GameObject, PainterEngine, GameObject)

    PX_Object* obj;
    Str name;
    List components;
    bool deleted;

    GameObject(){
        deleted = false;
        PX_Object* px_root = nullptr;
        if(g_root != nullptr){
            px_root = CAST(GameObject&, g_root).obj;
        }
        obj = PX_ObjectCreate(
            &_global_mp,
            px_root,   // PX_Object* Parent,
            0,      // px_float x,
            0,      // px_float y,
            0,      // px_float z,
            0,      // px_float Width,
            0,      // px_float Height,
            0       // px_float Length
        );
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
    g_mod = vm->new_module("PainterEngine");
    PyObject* go_type = GameObject::register_class(vm, g_mod);

    vm->bind_func<1>(g_mod, "Destroy", [](VM* vm, const Args& args){
        GameObject& self = CAST(GameObject&, args[0]);
        PX_ObjectDelete(self.obj);
        self.deleted = true;
        return vm->None;
    });

    // 加载一个资源，根据文件后缀名返回相应的指针
    vm->bind_func<1>(g_mod, "load", [](VM* vm, const Args& args){
        Str& path = CAST(Str&, args[0]);
        return vm->None;
    });

    CodeObject_ code = vm->compile(pe::kPythonLibs["PainterEngine"], "<PainterEngine>", EXEC_MODE);
    vm->_exec(code, g_mod);

    // 创建根对象
    try{
        g_root = vm->call(go_type, Args{VAR("/")});
    }catch(Exception& e){
        std::cerr << e.summary() << std::endl;
        exit(1);
    }
    g_mod->attr().set("_root", g_root);
}

namespace pkpy{
    template<> inline void gc_mark<GameObject>(GameObject& go){
        for(PyObject* obj: go.components){
            OBJ_MARK(obj);
        }

        // only do mark on root
        if(go.obj->pParent != NULL) return;
        PX_Object* px_root = CAST(GameObject&, g_root).obj;
        traverse(px_root, [](PX_Object* obj){
            OBJ_MARK((PyObject*)obj->User_ptr);
        });
    }
}

inline void GameObject::_register(VM* vm, PyObject* mod, PyObject* type){
    vm->bind_static_method<-1>(type, "__new__", [](VM* vm, const Args& args){
        PyObject* go = VAR_T(GameObject);
        GameObject& self = CAST(GameObject&, go);
        PX_ObjectSetUserPointer(self.obj, go);
        if(args.size() > 1) vm->TypeError("GameObject() takes 1 or 0 arguments");
        self.name = args.size()==0 ? "未命名物体" : CAST(Str&, args[0]);
        return go;
    });

    vm->bind_method<1>(type, "AddComponent", [](VM* vm, const Args& args){
        GameObject& self = CAST(GameObject&, args[0]);
        PyObject* obj = vm->call(args[1], Args{args[0]});
        self.components.push_back(obj);
        return obj;
    });

    vm->bind_method<0>(type, "children", [](VM* vm, const Args& args){
        // return vm->PyIter(PX_ChildrenIter(vm, args[0]));
        auto iter = PX_ChildrenIter(vm, args[0]);
        List list;
        PyObject* obj = iter.next();
        while(obj != nullptr){
            list.push_back(obj);
            obj = iter.next();
        }
        return VAR(Tuple(std::move(list)));
    });

    vm->bind_method<0>(type, "_update", [](VM* vm, const Args& args){
        return vm->None;
    });

    vm->bind_method<0>(type, "components", [](VM* vm, const Args& args){
        GameObject& self = CAST(GameObject&, args[0]);
        Tuple t(self.components.size());
        for(int i = 0; i < self.components.size(); i++){
            t[i] = self.components[i];
        }
        return VAR(std::move(t));
    });

    type->attr().set("name", vm->property(
        [](VM* vm, const Args& args){
            GameObject& self = CAST(GameObject&, args[0]);
            return VAR(self.name);
        }));
}