#pragma once

#include "Common.h"
#include "pocketpy.h"
#include <cctype>

using namespace pkpy;

struct Input {
    PY_CLASS(Input, PainterEngine, Input)

    inline static px_int CurrentKeycode;

    static void _register(VM* vm, PyObject* mod, PyObject* type){
        vm->bind_static_method<-1>(type, "__new__", CPP_NOT_IMPLEMENTED());

        vm->bind_static_method<1>(type, "GetKey", [](VM* vm, ArgsView args){
            if(is_non_tagged_type(args[0], vm->tp_str)) {
                const Str& key = CAST(Str&, args[0]);
                // TODO: check for multi char key
                return VAR(CurrentKeycode == key[0] || CurrentKeycode == tolower(key[0]));
            }
            px_int code = CAST(px_int, args[0]);
            return VAR(CurrentKeycode == code);
        });
    }
};