#pragma once

#include "Common.h"
#include "Vector2.h"
#include "pocketpy.h"

using namespace pkpy;

inline PX_Object* _get_px_obj(PyObject* obj){
    static const StrName m_px_obj = "_px_obj";
    PyObject* px_obj = obj->attr(m_px_obj);
    return _CAST(PX_Object*, px_obj);
}

inline PX_Object* get_px_obj(PyObject* obj){
    vm->isinstance(obj, g_tp_node);
    return _get_px_obj(obj);
}

struct PX_ChildrenIter: BaseIter{
    PyObject* ref;
    PX_Object* curr;

    PX_ChildrenIter(VM* vm, PyObject* ref): BaseIter(vm){
        this->ref = ref;
        curr = get_px_obj(ref)->pChilds;
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

inline void _register_node_type(VM* vm, PyObject* mod, PyObject* type){
    vm->bind_method<0>(type, "_px_obj_init", [](VM* vm, ArgsView args){
        PX_Object* px_root = nullptr;
        if(g_root != nullptr) px_root = get_px_obj(g_root);
        PX_Object* obj = PX_ObjectCreate(&App.runtime.mp_game, px_root, 0, 0, 0, 0, 0, 0);
        PX_ObjectSetUserPointer(obj, args[0]);
        // 设置Node的回调
        obj->Func_ObjectUpdate = [](PX_Object* obj, unsigned int _){
            PyObject* self = (PyObject*)obj->User_ptr;
            static StrName m_update = "_update";
            ::vm->call_method(self, m_update);
        };
        obj->Func_ObjectRender = [](px_surface* psurface, PX_Object* obj, px_uint elpased){
            PyObject* self = (PyObject*)obj->User_ptr;
            static StrName m_draw = "_draw";
            PyObject* method = ::vm->get_unbound_method(self, m_draw, &self);
            if(self != PY_NULL) ::vm->call(method, self);
        };
        return VAR(obj);
    });

    type->attr().set("position", vm->property(
        [](VM* vm, ArgsView args){
            PX_Object* obj = get_px_obj(args[0]);
            return VAR_T(Vector2, obj->x, obj->y);
        },
        [](VM* vm, ArgsView args){
            PX_Object* obj = get_px_obj(args[0]);
            Vector2& pos = CAST(Vector2&, args[1]);
            obj->x = pos.x;
            obj->y = pos.y;
            return vm->None;
        }));

    type->attr().set("parent", vm->property(
        [](VM* vm, ArgsView args){
            PX_Object* obj = get_px_obj(args[0]);
            if(obj->pParent == NULL) return vm->None;
            return (PyObject*)(obj->pParent->User_ptr);
        },
        [](VM* vm, ArgsView args){
            PX_Object* obj = get_px_obj(args[0]);
            PX_Object* pParent;
            if(args[1] == vm->None){
                pParent = get_px_obj(g_root);
            }else{
                pParent = get_px_obj(args[1]);
            }
            if(obj == pParent) vm->ValueError("can't set parent to self");
            PX_ObjectSetParent(obj, pParent);
            return vm->None;
        }));

    type->attr().set("children", vm->property(
        [](VM* vm, ArgsView args){
            return vm->PyIter(PX_ChildrenIter(vm, args[0]));
        }));

    type->attr().set("width", vm->property(
        [](VM* vm, ArgsView args){
            PX_Object* obj = get_px_obj(args[0]);
            return VAR(obj->Width);
        },
        [](VM* vm, ArgsView args){
            PX_Object* obj = get_px_obj(args[0]);
            obj->Width = vm->num_to_float(args[1]);
            return vm->None;
        }));
    type->attr().set("height", vm->property(
        [](VM* vm, ArgsView args){
            PX_Object* obj = get_px_obj(args[0]);
            return VAR(obj->Height);
        },
        [](VM* vm, ArgsView args){
            PX_Object* obj = get_px_obj(args[0]);
            obj->Height = vm->num_to_float(args[1]);
            return vm->None;
        }));

    type->attr().set("enabled", vm->property(
        [](VM* vm, ArgsView args){
            PX_Object* obj = get_px_obj(args[0]);
            return VAR(obj->Enabled != 0);
        },
        [](VM* vm, ArgsView args){
            PX_Object* obj = get_px_obj(args[0]);
            obj->Enabled = (int)CAST(bool, args[1]);
            return vm->None;
        }));

    type->attr().set("visible", vm->property(
        [](VM* vm, ArgsView args){
            PX_Object* obj = get_px_obj(args[0]);
            return VAR(obj->Visible != 0);
        },
        [](VM* vm, ArgsView args){
            PX_Object* obj = get_px_obj(args[0]);
            obj->Visible = (int)CAST(bool, args[1]);
            return vm->None;
        }));
}