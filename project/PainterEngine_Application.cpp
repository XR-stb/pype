#include "Framework.h"
using namespace pkpy;

px_bool PX_ApplicationInitialize(PX_Application *pApp,px_int screen_width,px_int screen_height)
{
	PX_ApplicationInitializeDefault(&pApp->runtime, screen_width, screen_height);
	px_bool ok = PX_WorldInitialize(
		&pApp->runtime.mp_game,
		&World,
		0xffffff,			// px_int world_width,
		0xffffff,			// px_int world_height,
		screen_width,		// px_int surface_width,
		screen_height,		// px_int surface_height,
		(px_dword)65535		// px_dword calcsize
	);
	if(!ok){
		std::cerr << "PX_WorldInitialize failed" << std::endl;
		return PX_FALSE;
	}

	try{
		// 初始化Python环境
		python_init();
	}catch(Exception& e){
		std::cerr << e.summary() << std::endl;
		return PX_FALSE;
	}

	bool curr_is_ok = std::filesystem::exists("main.py");
	if(!curr_is_ok){
		if(std::filesystem::exists("../../project/main.py")){
			std::filesystem::current_path("../../project");
		}else{
			std::cerr << "main.py文件未找到" << std::endl;
			return PX_FALSE;
		}
	}

	PX_IO_Data io_data = PX_LoadFileToIOData("main.py");
	PyObject* ret = vm->exec(std::string_view((char*)io_data.buffer, io_data.size), "main.py", EXEC_MODE);
	if(ret == nullptr) return PX_FALSE;
	return PX_TRUE;
}

px_void PX_ApplicationUpdate(PX_Application *pApp, px_dword elapsed)
{
	if(!g_frame_counter.do_update(&elapsed)) return;

	// 设置Time.delta_time
	static StrName m_Time = "Time";
	static StrName m_deltaTime = "delta_time";
	g_mod->attr(m_Time)->attr().set(m_deltaTime, VAR(elapsed / 1000.0));

	try{
		PX_WorldUpdate(&World, elapsed);
	}catch(Exception& e){
		std::cerr << e.summary() << std::endl;
		exit(1);
	}
}

px_void PX_ApplicationRender(PX_Application *pApp, px_dword elapsed)
{
	if(!g_frame_counter.do_render(&elapsed)) return;

	px_surface *pRenderSurface = &pApp->runtime.RenderSurface;
	PX_RuntimeRenderClear(&pApp->runtime, PX_OBJECT_UI_DEFAULT_BACKGROUNDCOLOR);

	try{
		PX_WorldRender(pRenderSurface, &World, elapsed);
	}catch(Exception& e){
		std::cerr << e.summary() << std::endl;
		exit(1);
	}

	// reset CurrentKeycode at the end of frame
	Input::PressedKeys.clear();
}

px_void PX_ApplicationPostEvent(PX_Application *pApp, PX_Object_Event e)
{
	PX_ApplicationEventDefault(&pApp->runtime, e);

	if(e.Event == PX_OBJECT_EVENT_KEYDOWN){
		px_uint code = PX_Object_Event_GetKeyDown(e);
		Input::PressedKeys.insert(code);
	}
}

