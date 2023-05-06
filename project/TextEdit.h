#pragma once

#include "_common.h"
#include "Node.h"
#include "Color.h"

inline void _register_textedit_type(VM* vm, PyObject* mod){
    PyObject* type = vm->new_type_object(mod, "TextEdit", g_tp_node);
    vm->bind_method<0>(type, "_px_obj_init", [](VM* vm, ArgsView args){
        PX_Object* obj = PX_Object_EditCreate(
            &App.runtime.mp_game,
            get_px_obj(g_root),     // PX_Object *Parent,
            0,
            0,
            120,
            40,
            NULL                    // PX_FontModule *fontmodule
        );
        PX_ObjectSetUserPointer(obj, args[0]);
        return VAR(obj);
    });

    vm->bind_method<1>(type, "set_max_length", [](VM* vm, ArgsView args){
        PX_Object* obj = _get_px_obj(args[0]);
        PX_Object_EditSetMaxTextLength(obj, CAST(i64, args[1]));
        return vm->None;
    });

    vm->bind_method<1>(type, "set_focus", [](VM* vm, ArgsView args){
        PX_Object* obj = _get_px_obj(args[0]);
        bool value = CAST(bool, args[1]);
        PX_Object_EditSetFocus(obj, value);
        return vm->None;
    });

    vm->bind_method<1>(type, "set_border", [](VM* vm, ArgsView args){
        PX_Object* obj = _get_px_obj(args[0]);
        bool value = CAST(bool, args[1]);
        PX_Object_EditSetBorder(obj, value);
        return vm->None;
    });

    vm->bind_method<2>(type, "set_auto_newline", [](VM* vm, ArgsView args){
        PX_Object* obj = _get_px_obj(args[0]);
        bool value = CAST(bool, args[1]);
        int spacing = CAST(int, args[2]);
        PX_Object_EditAutoNewLine(obj, value, spacing);
        return vm->None;
    });

    type->attr().set("text", vm->property(
        [](VM* vm, ArgsView args){
            PX_Object* obj = _get_px_obj(args[0]);
            char* value = PX_Object_EditGetText(obj);
            return VAR(Str(value));
        },
        [](VM* vm, ArgsView args){
            PX_Object* obj = _get_px_obj(args[0]);
            char* value = CAST(Str&, args[1]).c_str_dup();
            PX_Object_EditSetText(obj, value);
            free(value);
            return vm->None;
        }));

#define PROPERTY_COLOR(name, cname)                                 \
    type->attr().set(#name, vm->property(                           \
        [](VM* vm, ArgsView args){                                  \
            PX_Object* obj = _get_px_obj(args[0]);                  \
            PX_Object_Edit* edit = PX_Object_GetEdit(obj);          \
            return VAR_T(Color, edit->cname);                       \
        },                                                          \
        [](VM* vm, ArgsView args){                                  \
            PX_Object* obj = _get_px_obj(args[0]);                  \
            PX_Object_Edit* edit = PX_Object_GetEdit(obj);          \
            edit->cname = CAST(Color&, args[1]).to_px_color();      \
            return vm->None;                                        \
        }));

    PROPERTY_COLOR(color, TextColor);
    PROPERTY_COLOR(background, BackgroundColor);
    PROPERTY_COLOR(border_color, BorderColor);
    PROPERTY_COLOR(cursor_color, CursorColor);
#undef PROPERTY_COLOR
}