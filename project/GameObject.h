#pragma once

#include "Common.h"
#include "Vector2.h"

using namespace pkpy;

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
            PyObject* self = (PyObject*)obj->User_ptr;
            static StrName m_update = "_update";
            vm->call_method(self, m_update);
        };

        obj->Func_ObjectRender = [](px_surface* psurface, PX_Object* obj, px_uint elpased){
            PyObject* self = (PyObject*)obj->User_ptr;
            static StrName m_renderer = "renderer";
            PyObject* r = self->attr(m_renderer);
            if(r != vm->None) vm->call_method(r, __call__);
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
        if(curr == NULL) return vm->StopIteration;
        PyObject* ret = (PyObject*)curr->User_ptr;
        curr = curr->pNextBrother;
        return ret;
    }

    void _gc_mark() const override {
        OBJ_MARK(ref);
    }
};

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

    type->attr().set("localPosition", vm->property(
        [](VM* vm, ArgsView args){
            GameObject& self = CAST(GameObject&, args[0]);
            float x = self.obj->x;
            float y = self.obj->y;
            return VAR_T(Vector2, x, y);
        },
        [](VM* vm, ArgsView args){
            GameObject& self = CAST(GameObject&, args[0]);
            Vector2& pos = CAST(Vector2&, args[1]);
            self.obj->x = pos.x;
            self.obj->y = pos.y;
            return vm->None;
        }));

    type->attr().set("position", vm->property(
        [](VM* vm, ArgsView args){
            GameObject& self = CAST(GameObject&, args[0]);
            float x = self.obj->x;
            float y = self.obj->y;
            PX_Object* parent = self.obj->pParent;
            while(parent != NULL){
                x += parent->x;
                y += parent->y;
                parent = parent->pParent;
            }
            return VAR_T(Vector2, x, y);
        },
        [](VM* vm, ArgsView args){
            GameObject& self = CAST(GameObject&, args[0]);
            Vector2& pos = CAST(Vector2&, args[1]);
            PX_Object* parent = self.obj->pParent;
            while(parent != NULL){
                pos.x -= parent->x;
                pos.y -= parent->y;
                parent = parent->pParent;
            }
            self.obj->x = pos.x;
            self.obj->y = pos.y;
            return vm->None;
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

    type->attr().set("activeSelf", vm->property(
        [](VM* vm, ArgsView args){
            GameObject& self = CAST(GameObject&, args[0]);
            return VAR(self.obj->Enabled);
        }));
    vm->bind_method<1>(type, "SetActive", [](VM* vm, ArgsView args){
        GameObject& self = CAST(GameObject&, args[0]);
        self.obj->Enabled = CAST(bool, args[1]);
        return vm->None;
    });
    vm->bind_method<1>(type, "SetParent", [](VM* vm, ArgsView args){
        GameObject& self = CAST(GameObject&, args[0]);
        PX_Object* pParent;
        if(args[1] == vm->None){
            pParent = CAST(GameObject&, g_root).obj;
        }else{
            GameObject& parent = CAST(GameObject&, args[1]);
            pParent = parent.obj;
        }
        PX_ObjectSetParent(self.obj, pParent);
        return vm->None;
    });
}