#pragma once

#include "_common.h"
#include "pocketpy.h"

using namespace pkpy;


struct Color{
    PY_CLASS(Color, pype, Color)

    float r, g, b, a;
    Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

    PX_TEXTURERENDER_BLEND* blend() { return (PX_TEXTURERENDER_BLEND*)&r; }

    static void _register(VM* vm, PyObject* mod, PyObject* type){
        vm->bind_static_method<-1>(type, "__new__", [](VM* vm, ArgsView args){
            if(args.size()!=3 && args.size()!=4){
                vm->TypeError("Color() takes 3 or 4 arguments");
            }
            float r = vm->num_to_float(args[0]);
            float g = vm->num_to_float(args[1]);
            float b = vm->num_to_float(args[2]);
            float a = args.size()==4 ? vm->num_to_float(args[3]) : 1.0f;
            return VAR_T(Color, r, g, b, a);
        });

        vm->bind_method<0>(type, "__repr__", [](VM* vm, ArgsView args){
            Color& self = CAST(Color&, args[0]);
            std::stringstream ss;
            ss << "Color(" << self.r << ", " << self.g << ", " << self.b << ", " << self.a << ")";
            return VAR(ss.str());
        });

        vm->bind_method<1>(type, "__add__", [](VM* vm, ArgsView args){
            Color& self = CAST(Color&, args[0]);
            Color& other = CAST(Color&, args[1]);
            return VAR_T(Color, self.r+other.r, self.g+other.g, self.b+other.b, self.a+other.a);
        });

        vm->bind_method<1>(type, "__sub__", [](VM* vm, ArgsView args){
            Color& self = CAST(Color&, args[0]);
            Color& other = CAST(Color&, args[1]);
            return VAR_T(Color, self.r-other.r, self.g-other.g, self.b-other.b, self.a-other.a);
        });

        vm->bind_method<1>(type, "__mul__", [](VM* vm, ArgsView args){
            Color& self = CAST(Color&, args[0]);
            float other = vm->num_to_float(args[1]);
            return VAR_T(Color, self.r*other, self.g*other, self.b*other, self.a*other);
        });

        vm->bind_method<1>(type, "__truediv__", [](VM* vm, ArgsView args){
            Color& self = CAST(Color&, args[0]);
            float other = vm->num_to_float(args[1]);
            return VAR_T(Color, self.r/other, self.g/other, self.b/other, self.a/other);
        });

        vm->bind_method<1>(type, "__eq__", [](VM* vm, ArgsView args){
            Color& self = CAST(Color&, args[0]);
            Color& other = CAST(Color&, args[1]);
            return VAR(self.r==other.r && self.g==other.g && self.b==other.b && self.a==other.a);
        });

        vm->bind_method<1>(type, "__ne__", [](VM* vm, ArgsView args){
            Color& self = CAST(Color&, args[0]);
            Color& other = CAST(Color&, args[1]);
            return VAR(self.r!=other.r || self.g!=other.g || self.b!=other.b || self.a!=other.a);
        });
    }
};

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