#include "PainterEngine_Application.h"
PX_Application App;

#include "framework.h"
using namespace pkpy;

px_bool PX_ApplicationInitialize(PX_Application *pApp,px_int screen_width,px_int screen_height)
{
	PX_ApplicationInitializeDefault(&pApp->runtime, screen_width, screen_height);
	// 初始化Python环境
	python_init();

	pkpy_vm_exec(vm, pe::kPythonLibs["main"]);
	return PX_TRUE;
}

px_void PX_ApplicationUpdate(PX_Application *pApp, px_dword elapsed)
{
	if(!g_frame_counter.do_update(&elapsed)) return;

	// 设置Time.deltaTime
	static StrName m_Time = "Time";
	static StrName m_deltaTime = "deltaTime";
	g_mod->attr(m_Time)->attr().set(m_deltaTime, VAR(elapsed / 1000.0));
	PX_Object* px_root = CAST(GameObject&, g_root).obj;
	PX_ObjectUpdate(px_root, elapsed);
}

px_void PX_ApplicationRender(PX_Application *pApp, px_dword elapsed)
{
	if(!g_frame_counter.do_render(&elapsed)) return;

	px_surface *pRenderSurface = &pApp->runtime.RenderSurface;
	PX_RuntimeRenderClear(&pApp->runtime, PX_OBJECT_UI_DEFAULT_BACKGROUNDCOLOR);
	PX_Object* px_root = CAST(GameObject&, g_root).obj;
	PX_ObjectRender(pRenderSurface, px_root, elapsed);
}

px_void PX_ApplicationPostEvent(PX_Application *pApp,PX_Object_Event e)
{
	PX_ApplicationEventDefault(&pApp->runtime, e);

	
}

