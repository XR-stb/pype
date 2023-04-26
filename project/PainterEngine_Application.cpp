#include "Common.h"
#include "Framework.h"
using namespace pkpy;

px_uint PX_APPLICATION_SURFACE_SIZE = 680;

px_uint PX_APPLICATION_MEMORYPOOL_UI_SIZE = 1024*1024*8;
px_uint PX_APPLICATION_MEMORYPOOL_RESOURCES_SIZE = 1024*1024*64;
px_uint PX_APPLICATION_MEMORYPOOL_GAME_SIZE = 1024*1024*8;

// 这是干啥的？
px_uint PX_APPLICATION_MEMORYPOOL_SPACE_SIZE = 1024*1024*8;

bool _execute_user_script(){
	Bytes content = _read_file_cwd("main.py");
	if(!content) return false;
	PyObject* ret = vm->exec(content.str(), "main.py", EXEC_MODE);
	return ret != nullptr;
}

px_bool PX_ApplicationInitializeDefault(PX_Runtime *runtime, px_int screen_width, px_int screen_height)
{
	Bytes content = _read_file_cwd("config.py");
	if(!content){
		std::cout << "config.py文件未找到" << std::endl;
		return PX_FALSE;
	}

	VM* vm = new VM(true);
	PyObject* ret = vm->exec(content.str(), "config.py", EXEC_MODE);
	if(ret == nullptr){
		delete vm;
		return PX_FALSE;
	}else{
		PyObject* val;
		val = vm->_main->attr("PX_APPLICATION_SURFACE_SIZE");
		PX_APPLICATION_SURFACE_SIZE = CAST(i64, val);
		val = vm->_main->attr("PX_APPLICATION_MEMORYPOOL_UI_SIZE");
		PX_APPLICATION_MEMORYPOOL_UI_SIZE = CAST(i64, val) * 1024 * 1024;
		val = vm->_main->attr("PX_APPLICATION_MEMORYPOOL_RESOURCES_SIZE");
		PX_APPLICATION_MEMORYPOOL_RESOURCES_SIZE = CAST(i64, val) * 1024 * 1024;
		val = vm->_main->attr("PX_APPLICATION_MEMORYPOOL_GAME_SIZE");
		PX_APPLICATION_MEMORYPOOL_GAME_SIZE = CAST(i64, val) * 1024 * 1024;
		val = vm->_main->attr("PX_APPLICATION_MEMORYPOOL_SPACE_SIZE");
		PX_APPLICATION_MEMORYPOOL_SPACE_SIZE = CAST(i64, val) * 1024 * 1024;
		delete vm;
	}

	px_int surface_width=0,surface_height=0;
	px_int window_width=0,window_height=0;
	px_double wdh;
	if (screen_width==0|| screen_height==0)
	{
		window_width = 0;
		window_height = 0;
	}
	else
	{
		wdh = screen_width * 1.0 / screen_height;
		surface_height = (px_int)(PX_sqrtd(PX_APPLICATION_SURFACE_SIZE * PX_APPLICATION_SURFACE_SIZE / wdh));
		surface_width = (px_int)(surface_height * wdh);
		window_width = screen_width / 2;
		window_height = screen_height / 2;
	}
	
	// 使用动态内存分配
	const px_uint PX_ApplicationRuntimeSize = PX_APPLICATION_MEMORYPOOL_UI_SIZE+PX_APPLICATION_MEMORYPOOL_RESOURCES_SIZE+PX_APPLICATION_MEMORYPOOL_GAME_SIZE+PX_APPLICATION_MEMORYPOOL_SPACE_SIZE;
	void* PX_ApplicationRuntime = malloc(PX_ApplicationRuntimeSize);
	if(!PX_RuntimeInitialize(runtime,surface_width,surface_height,window_width,window_height,PX_ApplicationRuntime,PX_ApplicationRuntimeSize,PX_APPLICATION_MEMORYPOOL_UI_SIZE,PX_APPLICATION_MEMORYPOOL_RESOURCES_SIZE,PX_APPLICATION_MEMORYPOOL_GAME_SIZE))
		return PX_FALSE;
	return PX_TRUE;
}

px_bool PX_ApplicationInitialize(PX_Application *pApp,px_int screen_width,px_int screen_height)
{
	// 设置工作目录
	bool curr_is_ok = std::filesystem::exists("main.py");
	if(!curr_is_ok){
		if(std::filesystem::exists("../../project/main.py")){
			std::filesystem::current_path("../../project");
		}else{
			std::cerr << "main.py文件未找到" << std::endl;
			return PX_FALSE;
		}
	}

	px_bool ok = PX_ApplicationInitializeDefault(&pApp->runtime, screen_width, screen_height);
	if(!ok) return PX_FALSE;

	ok = PX_WorldInitialize(
		&pApp->runtime.mp_game,
		&World,
		0xffffff,			// px_int world_width,
		0xffffff,			// px_int world_height,
		pApp->runtime.surface_width,
		pApp->runtime.surface_height,
		PE_MEMORY_CALC_SIZE		// px_dword calcsize
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

	ok = _execute_user_script();
	return ok;
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
	// PX_ApplicationEventDefault
	if (e.Event==PX_OBJECT_EVENT_WINDOWRESIZE)
	{
		px_int surface_width=0,surface_height=0;
		px_double wdh;

		wdh=PX_Object_Event_GetWidth(e)*1.0/PX_Object_Event_GetHeight(e);
		surface_height=(px_int)(PX_sqrtd(PX_APPLICATION_SURFACE_SIZE*PX_APPLICATION_SURFACE_SIZE/wdh));
		surface_width=(px_int)(surface_height*wdh);

		PX_RuntimeResize(&pApp->runtime,surface_width,surface_height,(px_int)PX_Object_Event_GetWidth(e),(px_int)PX_Object_Event_GetHeight(e));
		return;
	}

	switch(e.Event){
		case PX_OBJECT_EVENT_KEYDOWN: {
			px_uint code = PX_Object_Event_GetKeyDown(e);
			if(code == 5){
				PyObject* ret = vm->exec("for obj in list(_root.children):\n  destroy(obj)", "<PainterEngine>", EXEC_MODE, g_mod);
				if(ret == nullptr) exit(1);
				bool ok = _execute_user_script();
				if(!ok) exit(1);
			}
			Input::PressedKeys.insert(code);
		} break;
	}

	try{
		PX_WorldPostEvent(&World, e);
	}catch(Exception& e){
		std::cerr << e.summary() << std::endl;
		exit(1);
	}
}

