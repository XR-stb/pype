#pragma once

#include "_common.h"
#include "Node.h"
#include "Color.h"

inline void _register_progressbar_type(VM* vm, PyObject* mod){
    PyObject* type = mod->attr("ProgressBar");
    vm->bind_method<0>(type, "_px_obj_init", [](VM* vm, ArgsView args){
        PX_Object* obj = PX_Object_ProcessBarCreate(
            &App.runtime.mp_game,
            get_px_obj(g_root), // PX_Object *Parent,
            0,
            0,
            120,
            40
        );
        PX_ObjectSetUserPointer(obj, args[0]);
        return VAR(obj);
    });

    type->attr().set("color", vm->property(
        [](VM* vm, ArgsView args){
            PX_Object* obj = get_px_obj(args[0]);
            PX_Object_ProcessBar* bar = PX_Object_GetProcessBar(obj);
            return VAR_T(Color, bar->Color);
        },
        [](VM* vm, ArgsView args){
            PX_Object* obj = _get_px_obj(args[0]);
            PX_Object_ProcessBar* bar = PX_Object_GetProcessBar(obj);
            bar->Color = CAST(Color&, args[1]).to_px_color();
            return vm->None;
        }));

    type->attr().set("max_value", vm->property(
        [](VM* vm, ArgsView args){
            PX_Object* obj = _get_px_obj(args[0]);
            PX_Object_ProcessBar* bar = PX_Object_GetProcessBar(obj);
            return VAR(bar->MAX);
        },
        [](VM* vm, ArgsView args){
            PX_Object* obj = _get_px_obj(args[0]);
            PX_Object_ProcessBar* bar = PX_Object_GetProcessBar(obj);
            bar->MAX = CAST(i64, args[1]);
            return vm->None;
        }));

    type->attr().set("value", vm->property(
        [](VM* vm, ArgsView args){
            PX_Object* obj = _get_px_obj(args[0]);
            PX_Object_ProcessBar* bar = PX_Object_GetProcessBar(obj);
            return VAR(bar->Value);
        },
        [](VM* vm, ArgsView args){
            PX_Object* obj = _get_px_obj(args[0]);
            PX_Object_ProcessBar* bar = PX_Object_GetProcessBar(obj);
            bar->Value = CAST(i64, args[1]);
            return vm->None;
        }));
}