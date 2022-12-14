#include "AppWindow.h"
#include "EngineTime.h"


struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
	vec3 position1;
	vec3 color;
	vec3 color1;
};

__declspec(align(16))
struct constant
{
	float m_angle;
};

AppWindow::AppWindow()
{
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex list[] =
	{
		

		//X - Y - Z
		/*{-0.5f,-0.5f,0.0f,    -0.25f,-0.25f,0.0f,   1,0,0,  0,0,1}, // POS1
		{-0.5f,0.5f,0.0f,     -0.25f,0.75f,0.0f,    0,1,0,  0.5f,0.5f,0.5f }, // POS2
		{ 0.5f,-0.5f,0.0f,     0.75f,-0.25f,0.0f,   0,0,1,  1,0,0 },// POS2
		{ 0.5f,0.5f,0.0f,      0.75f,0.75f,0.0f,    1,0,0,  0,0,1 }*/


		{-0.51f,-0.38f,0.0f,    -0.81f,-0.92,0.0,   0,0,0,  0,1,0 }, // POS1
		{-0.43f,0.38f,0.0f,     -0.92f,0.53,0.0f,    1,1,0,  0,1,1 }, // POS2
		{ 0.35f,-0.72f,0.0f,     0.98,-0.77f,0.0f,   0,0,1,  1,0,0 },// POS2
		{ 0.64f,0.72f,0.0f,      -0.81f,-0.92,0.0,    1,1,1,  0,0,1 }

		//square translation
		/*{-0.32f,-0.11f,0.0f,    -0.82,-0.61,0.0f,   0,0,0,  0,1,0}, // POS1
		{-0.11f,0.78f,0.0f,     -0.61f,0.28f,0.0f,    1,1,0,  0,1,1 }, // POS2
		{ 0.75f,-0.73f,0.0f,     0.25f,-0.99f,0.0f,   0,0,1,  1,0,0 },// POS2
		{ 0.88f,0.77f,0.0f,      0.38f,0.27f,0.0f,    1,1,1,  0,0,1 }*/


	};

		

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl","vsmain", &shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	
	

	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();


	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);

	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();


	constant cc;
	cc.m_angle = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));



}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	
	unsigned long new_time = 0;
	
	float anim_speed = rand() % 10 + EngineTime::getDeltaTime() * (rand() % 100 - rand() % 10);
	//std::cout << "Frame update Finish in:" << anim_speed << "\n";

	m_angle +=  anim_speed * EngineTime::getDeltaTime();
	constant cc;
	cc.m_angle = m_angle;

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);


	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}