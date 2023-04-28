#pragma once

#include "Common.h"
#include <cctype>

#include "_keycodes.h"

using namespace pkpy;

struct Input {
    PY_CLASS(Input, pype, Input)

    inline static std::set<int> _pressed_keys;

    static void _register(VM* vm, PyObject* mod, PyObject* type){
        vm->bind_static_method<-1>(type, "__new__", CPP_NOT_IMPLEMENTED());

        vm->bind_static_method<1>(type, "get_key", [](VM* vm, ArgsView args){
            int code = CAST(int, args[0]);
            return VAR(is_pressed(code));
        });

        vm->bind_static_method<0>(type, "get_pressed_keys", [](VM* vm, ArgsView args){
            Tuple t(_pressed_keys.size());
            int i = 0;
            for(auto code : _pressed_keys){
                t[i++] = VAR(code);
            }
            return VAR(std::move(t));
        });

        PyObject* tp_keycode = vm->new_type_object(mod, "KeyCode", vm->tp_object);
        vm->bind_static_method<1>(tp_keycode, "nameof", [](VM* vm, ArgsView args){
            unsigned int code = CAST(unsigned int, args[0]);
            if(code >= KEY_MAPPING_SIZE){
                vm->ValueError("undefined key code");
            }
            return VAR(kVirtualKeyCodes[code]);
        });

        for(int i=0; i<KEY_MAPPING_SIZE; i++){
            tp_keycode->attr().set(kVirtualKeyCodes[i], VAR(i));
        }
    }

    static void press_scancode(int code){
        auto it = KEY_MAPPING.find(code);
        if(it != KEY_MAPPING.end()){
            code = it->second;
        }else{
            std::cerr << "undefined key code: " << code << std::endl;
            return;
        }
        _pressed_keys.insert(code);
    }

    static void clear_pressed_keys(){
        _pressed_keys.clear();
    }

    static bool is_pressed(int code){
        return _pressed_keys.count(code) > 0;
    }
};