#pragma once

#include "Common.h"
#include "pocketpy.h"

using namespace pkpy;

struct Texture2D {
    PY_CLASS(Texture2D, pype, Texture2D)

    px_texture* ptr;
    Texture2D(px_texture* ptr) : ptr(ptr) {}

    static void _register(VM* vm, PyObject* mod, PyObject* type){
        vm->bind_static_method<1>(type, "__new__", [](VM* vm, ArgsView args){
            px_texture* ptr = CAST(px_texture*, args[0]);
            return VAR_T(Texture2D, ptr);
        });

        vm->bind_method<0>(type, "__repr__", [](VM* vm, ArgsView args){
            Texture2D& self = CAST(Texture2D&, args[0]);
            std::stringstream ss;
            ss << "Texture2D(" << self.ptr->width << ", " << self.ptr->height << ")";
            return VAR(ss.str());
        });

#define PROPERTY(name)                                              \
        type->attr().set(#name, vm->property(                       \
            [](VM* vm, ArgsView args){                              \
                Texture2D& self = CAST(Texture2D&, args[0]);        \
                return VAR(self.ptr->name);                         \
            }));

        PROPERTY(width);
        PROPERTY(height);
        PROPERTY(limit_left);
        PROPERTY(limit_top);
        PROPERTY(limit_right);
        PROPERTY(limit_bottom);

#undef PROPERTY
        
    }
};