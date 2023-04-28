#pragma once

#include "Common.h"
#include <cctype>

#include "_keycodes.h"

using namespace pkpy;

struct Input {
    PY_CLASS(Input, pype, Input)

    // 一旦对key调用了相关函数，就会被加入到这个集合中
    inline static std::set<int> _subscribed_keys;

    inline static std::set<int> _pressed_keys;
    inline static std::set<int> _pressed_keys_last_frame;

    static void _register(VM* vm, PyObject* mod, PyObject* type){
        vm->bind_static_method<-1>(type, "__new__", CPP_NOT_IMPLEMENTED());

        vm->bind_static_method<1>(type, "get_key", [](VM* vm, ArgsView args){
            int code = CAST(int, args[0]);
            return VAR(get_key(code));
        });

        vm->bind_static_method<1>(type, "get_key_down", [](VM* vm, ArgsView args){
            int code = CAST(int, args[0]);
            return VAR(get_key_down(code));
        });

        vm->bind_static_method<1>(type, "get_key_up", [](VM* vm, ArgsView args){
            int code = CAST(int, args[0]);
            return VAR(get_key_up(code));
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

    static bool get_key(int code){
        _subscribed_keys.insert(code);
        return _pressed_keys.count(code) != 0;
    }

    static bool get_key_down(int code){
        _subscribed_keys.insert(code);
        return _pressed_keys_last_frame.count(code) == 0 && _pressed_keys.count(code) != 0;
    }

    static bool get_key_up(int code){
        _subscribed_keys.insert(code);
        return _pressed_keys_last_frame.count(code) != 0 && _pressed_keys.count(code) == 0;
    }

    static void begin_update_subscribed_keys(){
        for(int code : _subscribed_keys){
            int scancode = keycode_to_scancode(code);
            if(scancode == 0) continue;
            if(_platform_get_key(scancode)){
                _pressed_keys.insert(code);
            }
        }
    }

    static void end_update_subscribed_keys(){
        _pressed_keys_last_frame.swap(_pressed_keys);
        _pressed_keys.clear();
    }
};