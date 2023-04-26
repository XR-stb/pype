#pragma once

#include "Common.h"
#include "Node.h"
#include "Vector2.h"
#include "Input.h"

#include "Button.h"
#include "Label.h"

using namespace pkpy;

struct GCProxy{
    PY_CLASS(GCProxy, pype, _GCProxy)

    static void _register(VM* vm, PyObject* mod, PyObject* type){
        vm->bind_static_method<-1>(type, "__new__", CPP_NOT_IMPLEMENTED());
    }
};

namespace pkpy{
    template<> inline void gc_mark<GCProxy>(GCProxy& proxy){
        PX_Object* px_root = get_px_obj(g_root);
        traverse(px_root, [](PX_Object* obj){
            OBJ_MARK((PyObject*)obj->User_ptr);
        });
    }
}

inline void python_init(){
    vm = pkpy_new_vm(true);
    vm->bind_builtin_func<0>("input", [](VM* vm, ArgsView args){
        return VAR(pkpy::getline());
    });
    g_mod = vm->new_module("pype");
    Vector2::register_class(vm, g_mod);
    Input::register_class(vm, g_mod);
    GCProxy::register_class(vm, g_mod);

    /*************全局私有函数*************/
    vm->bind_func<1>(g_mod, "_PX_ObjectDelete", [](VM* vm, ArgsView args){
        PX_Object* obj = get_px_obj(args[0]);
        PX_ObjectDelete(obj);
        return vm->None;
    });

    vm->bind_func<1>(g_mod, "_PX_LoadTextureFromFile", [](VM* vm, ArgsView args){
        const Str& path = CAST(Str&, args[0]);
        px_texture* tex = (px_texture*)malloc(sizeof(px_texture));

        bool ok;
        Bytes content = _read_file_cwd(path, &ok);
        if(!ok) return vm->None;

    	ok = PX_TextureCreateFromMemory(&App.runtime.mp_resources, (char*)content._data.c_str(), content._data.size(), tex);
        if(!ok){
            free(tex);
            return vm->None;
        }
        return VAR_T(VoidP, tex);
    });

    vm->bind_func<4>(g_mod, "_PX_TextureRenderEx", [](VM* vm, ArgsView args){
        px_surface* psurface = &App.runtime.RenderSurface;
        px_texture* tex = CAST(px_texture*, args[0]);
        Vector2& pos = CAST(Vector2&, args[1]);
        float angle = CAST(float, args[2]);
        float scale = CAST(float, args[3]);
        PX_TextureRenderEx(psurface, tex, pos.x, pos.y, PX_ALIGN_LEFTTOP, NULL, scale, angle);
        return vm->None;
    });

    vm->bind_func<1>(g_mod, "_PX_WorldObjectXYtoScreenXY", [](VM* vm, ArgsView args){
        const Vector2& vec = CAST(Vector2&, args[0]);
        px_point ret = PX_WorldObjectXYtoScreenXY(&World, vec.x, vec.y);
        return VAR_T(Vector2, ret.x, ret.y);
    });

    vm->bind_func<0>(g_mod, "memory_usage", [](VM* vm, ArgsView args){
        auto f = [](px_memorypool* mp){
            size_t used = mp->Size - mp->FreeSize;
            double percent = (double)used / mp->Size * 100;
            double used_mb = used/1024.0/1024.0;
            double total_mb = mp->Size/1024.0/1024.0;
            percent = (int)(percent * 100 + 0.5) / 100.0;
            used_mb = (int)(used_mb * 100 + 0.5) / 100.0;
            total_mb = (int)(total_mb * 100 + 0.5) / 100.0;
            return fmt(used_mb, " / ", total_mb, " MB\t(", percent, "%)");
        };
        std::stringstream ss;
        ss << "TOTAL:     " << f(&App.runtime.mp) << std::endl;
        ss << "- UI:        " << f(&App.runtime.mp_ui) << std::endl;
        ss << "- RESOURCES: " << f(&App.runtime.mp_resources) << std::endl;
        ss << "- GAME:      " << f(&App.runtime.mp_game) << std::endl;
        double vm_usage = memory_usage()/1024.0/1024.0;
        vm_usage = (int)(vm_usage * 100 + 0.5) / 100.0;
        ss << "Python VM: " << vm_usage << " MB";
        (*vm->_stdout) << ss.str() << std::endl;
        return vm->None;
    });

    // 注册Python库源码
    for(auto it = pe::kPythonLibs.begin(); it != pe::kPythonLibs.end(); ++it){
        CodeObject_ code = vm->compile(
            it->second,
            fmt("<", it->first.substr(2), ">"),
            EXEC_MODE
        );
        vm->_exec(code, g_mod);
    }

    // 初始化Node基类
    g_tp_node = OBJ_GET(Type, g_mod->attr("Node"));
    _register_node_type(vm, g_mod, vm->_t(g_tp_node));

    // 注册其他类
    _register_button_type(vm, g_mod);
    _register_label_type(vm, g_mod);

    // 创建根对象
    g_root = vm->call(vm->_t(g_tp_node));
    g_mod->attr().set("_root", g_root);
    PX_Object* px_root = get_px_obj(g_root);

    // 将根对象加入世界
    PX_WorldAddObject(&World, px_root);
    // 设置相机指向(0, 0)
    PX_WorldSetCamera(&World, px_point{0, 0, 0});
    // 创建GC代理，该代理用于对象树标记
    g_mod->attr().set("_gc_proxy", VAR_T(GCProxy));
}


// 帧率控制
struct FrameCounter{
	px_dword update_freq = 1000 / 60;
	px_dword accum_time = 0;
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