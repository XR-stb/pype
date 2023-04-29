#pragma once

#include "_common.h"

using namespace pkpy;

struct Vector2 {
    PY_CLASS(Vector2, pype, Vector2)
    float x;
    float y;

    Vector2(float x, float y) : x(x), y(y) {}
    Vector2(px_point p) : x(p.x), y(p.y) {}

    operator px_point() const {
        return {x, y};
    }

    static void _register(VM* vm, PyObject* mod, PyObject* type){
        vm->bind_static_method<2>(type, "__new__", [](VM* vm, ArgsView args){
            float x = vm->num_to_float(args[0]);
            float y = vm->num_to_float(args[1]);
            return VAR_T(Vector2, x, y);
        });

        vm->bind_method<0>(type, "__repr__", [](VM* vm, ArgsView args){
            Vector2& self = CAST(Vector2&, args[0]);
            std::stringstream ss;
            ss << "Vector2(" << self.x << ", " << self.y << ")";
            return VAR(ss.str());
        });

        vm->bind_method<1>(type, "__add__", [](VM* vm, ArgsView args){
            Vector2& self = CAST(Vector2&, args[0]);
            Vector2& other = CAST(Vector2&, args[1]);
            return VAR_T(Vector2, self.x + other.x, self.y + other.y);
        });

        vm->bind_method<1>(type, "__sub__", [](VM* vm, ArgsView args){
            Vector2& self = CAST(Vector2&, args[0]);
            Vector2& other = CAST(Vector2&, args[1]);
            return VAR_T(Vector2, self.x - other.x, self.y - other.y);
        });

        vm->bind_method<1>(type, "__mul__", [](VM* vm, ArgsView args){
            Vector2& self = CAST(Vector2&, args[0]);
            f64 other = vm->num_to_float(args[1]);
            return VAR_T(Vector2, self.x * other, self.y * other);
        });

        vm->bind_method<1>(type, "__truediv__", [](VM* vm, ArgsView args){
            Vector2& self = CAST(Vector2&, args[0]);
            f64 other = vm->num_to_float(args[1]);
            return VAR_T(Vector2, self.x / other, self.y / other);
        });

        vm->bind_method<1>(type, "__eq__", [](VM* vm, ArgsView args){
            Vector2& self = CAST(Vector2&, args[0]);
            Vector2& other = CAST(Vector2&, args[1]);
            return VAR(self.x == other.x && self.y == other.y);
        });

        vm->bind_method<1>(type, "__ne__", [](VM* vm, ArgsView args){
            Vector2& self = CAST(Vector2&, args[0]);
            Vector2& other = CAST(Vector2&, args[1]);
            return VAR(self.x != other.x || self.y != other.y);
        });
    }
};