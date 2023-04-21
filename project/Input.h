#pragma once

#include "Common.h"
#include <cctype>

using namespace pkpy;

struct Input {
    PY_CLASS(Input, PainterEngine, Input)

    inline static std::set<px_int> PressedKeys;

    static void _register(VM* vm, PyObject* mod, PyObject* type){
        vm->bind_static_method<-1>(type, "__new__", CPP_NOT_IMPLEMENTED());

        vm->bind_static_method<1>(type, "get_key", [](VM* vm, ArgsView args){
            if(is_non_tagged_type(args[0], vm->tp_str)) {
                const Str& key = CAST(Str&, args[0]);
                // TODO: check for multi char key
                bool _0 = PressedKeys.count(key[0]) > 0;
                bool _1 = PressedKeys.count(tolower(key[0])) > 0;
                return VAR(_0 || _1);
            }
            px_int code = CAST(px_int, args[0]);
            return VAR(PressedKeys.count(code) > 0);
        });
    }
};