#include "PainterEngine_Application.h"
PX_Application App;

#include "framework.h"
using namespace pkpy;
PyObject* _update;		// 缓存内部的_update函数，避免每次都去查找

px_bool PX_ApplicationInitialize(PX_Application *pApp,px_int screen_width,px_int screen_height)
{
	PX_ApplicationInitializeDefault(&pApp->runtime, screen_width, screen_height);

	python_init();
	_update = g_mod->attr().try_get("_update");
	if(!_update) exit(1);

	pkpy_vm_exec(vm, pe::kPythonLibs["main"]);
	return PX_TRUE;
}

px_void PX_ApplicationUpdate(PX_Application *pApp, px_dword elapsed)
{
	float deltaTime = (float)elapsed / 1000.0f;
	try{
		vm->call(_update, Args{VAR(deltaTime)});
	}catch(const pkpy::Exception& e){
		std::cerr << e.summary() << std::endl;
		exit(1);
	}
}

px_void PX_ApplicationRender(PX_Application *pApp,px_dword elapsed)
{
	px_surface *pRenderSurface = &pApp->runtime.RenderSurface;
	PX_RuntimeRenderClear(&pApp->runtime, PX_OBJECT_UI_DEFAULT_BACKGROUNDCOLOR);
	PX_Object* px_root = CAST(GameObject&, g_root).obj;
	PX_ObjectRender(pRenderSurface, px_root, elapsed);
}

px_void PX_ApplicationPostEvent(PX_Application *pApp,PX_Object_Event e)
{
	PX_ApplicationEventDefault(&pApp->runtime, e);

	
}

