#pragma once

#include "_common.h"

using namespace pkpy;


struct Color{
    PY_CLASS(Color, pype, Color)

    float r, g, b, a;
    Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    Color(const px_color& c){
        r = c._argb.r / 255.0f;
        g = c._argb.g / 255.0f;
        b = c._argb.b / 255.0f;
        a = c._argb.a / 255.0f;
    }

    PX_TEXTURERENDER_BLEND* blend() { return (PX_TEXTURERENDER_BLEND*)&r; }

    px_color to_px_color() const {
        unsigned char r = (unsigned char)(this->r * 255);
        unsigned char g = (unsigned char)(this->g * 255);
        unsigned char b = (unsigned char)(this->b * 255);
        unsigned char a = (unsigned char)(this->a * 255);
        return PX_COLOR(a, r, g, b);
    }

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
