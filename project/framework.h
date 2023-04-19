#pragma once

#include "Common.h"
#include "GameObject.h"
#include "Vector2.h"

using namespace pkpy;

inline void python_init(){
    // vm init
    vm = pkpy_new_vm(true);
    vm->bind_builtin_func<0>("input", [](VM* vm, ArgsView args){
        return VAR(pkpy::getline());
    });
    g_mod = vm->new_module("PainterEngine");
    PyObject* go_type = GameObject::register_class(vm, g_mod);
    PyObject* vec2_type = Vector2::register_class(vm, g_mod);

    vm->bind_func<1>(g_mod, "Destroy", [](VM* vm, ArgsView args){
        GameObject& self = CAST(GameObject&, args[0]);
        PX_ObjectDelete(self.obj);
        self.obj = NULL;
        return vm->None;
    });

    /*************全局私有函数*************/
    vm->bind_func<1>(g_mod, "_PX_LoadTextureFromFile", [](VM* vm, ArgsView args){
        const Str& path = CAST(Str&, args[0]);
        char* path_c = path.c_str_dup();
        px_texture* tex = (px_texture*)malloc(sizeof(px_texture));
        bool ok = PX_LoadTextureFromFile(&App.runtime.mp_resources, tex, path_c);
        free(path_c);
        if(!ok){
            free(tex);
            return vm->None;
        }
        return VAR_T(VoidP, tex);
    });

    vm->bind_func<2>(g_mod, "_PX_TextureRender", [](VM* vm, ArgsView args){
        px_surface* psurface = &App.runtime.RenderSurface;
        px_texture* tex = CAST(px_texture*, args[0]);
        const Vector2& vec = CAST(Vector2&, args[1]);
        // PX_TEXTURERENDER_BLEND* blend = CAST(VoidP&, args[5]);
        PX_TextureRender(psurface, tex, vec.x, vec.y, PX_ALIGN_LEFTTOP, NULL);
        return vm->None;
    });

    vm->bind_func<1>(g_mod, "_PX_WorldObjectXYtoScreenXY", [](VM* vm, ArgsView args){
        const Vector2& vec = CAST(Vector2&, args[0]);
        px_point ret = PX_WorldObjectXYtoScreenXY(&World, vec.x, vec.y);
        return VAR_T(Vector2, ret.x, ret.y);
    });


    // 注册Python库源码
    for(auto it = pe::kPythonLibs.begin(); it != pe::kPythonLibs.end(); ++it){
        if(it->first == "main" || it->first == "__main__") continue;
        CodeObject_ code = vm->compile(
            it->second,
            fmt("<", it->first, ">"),
            EXEC_MODE
        );
        vm->_exec(code, g_mod);
    }

    // 创建根对象
    g_root = vm->call(go_type, VAR("/"));
    g_mod->attr().set("_root", g_root);
    PX_Object* px_root = CAST(GameObject&, g_root).obj;
    // 将根对象加入世界
    PX_WorldAddObject(&World, px_root);

    // 设置相机指向(0, 0)
    PX_WorldSetCamera(&World, px_point{0, 0, 0});
}


// 帧率控制
struct FrameCounter{
	px_dword update_freq = 1000 / 60;
	px_dword accum_time = 0xffff;
    px_dword elapsed = 0;
	bool flag = false;

	bool do_update(px_dword* delta){
		if(flag) throw std::runtime_error("do_update() called twice in one frame");
		accum_time += *delta;
		if(accum_time >= update_freq){
            elapsed = accum_time;
            *delta = elapsed;
			accum_time = 0;
			flag = true;
		}
		return flag;
	}

	bool do_render(px_dword* delta){
		if(flag){
			flag = false;
            *delta = elapsed;
			return true;
		}
		return false;
	}
};

inline FrameCounter g_frame_counter;