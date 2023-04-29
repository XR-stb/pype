#pragma once

#include "_common.h"
#include "Node.h"

inline void _register_label_type(VM* vm, PyObject* mod){
    PyObject* type = mod->attr("Label");
    vm->bind_method<0>(type, "_px_obj_init", [](VM* vm, ArgsView args){
        PX_Object* obj = PX_Object_LabelCreate(
            &App.runtime.mp_game,
            get_px_obj(g_root), // PX_Object *Parent,
            0,
            0,
            120,
            40,
            "Button",             // const px_char *Text
            NULL,                 // PX_FontModule *fontmodule,
            PX_COLOR(255, 255, 255, 255)
        );
        PX_ObjectSetUserPointer(obj, args[0]);
        return VAR(obj);
    });

    type->attr().set("text", vm->property(
        [](VM* vm, ArgsView args){
            PX_Object* obj = get_px_obj(args[0]);
            char* value = PX_Object_LabelGetText(obj);
            return VAR(Str(value));
        },
        [](VM* vm, ArgsView args){
            PX_Object* obj = get_px_obj(args[0]);
            char* value = CAST(Str&, args[1]).c_str_dup();
            PX_Object_LabelSetText(obj, value);
            free(value);
            return vm->None;
        }));
}