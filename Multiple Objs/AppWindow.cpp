#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "Cube.h"
#include "Plane.h"
#include <stdlib.h>
#include "InputSystem.h"
#include "SceneCameraHandler.h"

struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};


__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};


AppWindow::AppWindow()
{
}

void AppWindow::update()
{
	constant cc;
	cc.m_time = ::GetTickCount();

	m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;


	Matrix4x4 temp;

	m_delta_scale += m_delta_time / 0.55f;

	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));
	
	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f,1.5f, 0), m_delta_pos));

	//cc.m_world *= temp;

	/*cc.m_world.setScale(Vector3D(1, 1, 1));

	temp.setIdentity();
	temp.setRotationZ(m_delta_scale);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_delta_scale);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_delta_scale);
	cc.m_world *= temp;*/

	//cc.m_world.setIdentity();

	//Matrix4x4 world_cam;
	//world_cam.setIdentity();

	//temp.setIdentity();
	//temp.setRotationX(m_rot_x);
	//world_cam *= temp;

	//temp.setIdentity();
	//temp.setRotationY(m_rot_y);
	//world_cam *= temp;

	//Vector3D new_pos = m_world_cam.getTranslation()+ world_cam.getZDirection()*(m_forward*0.3f);

	//new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.3f);

	//world_cam.setTranslation(new_pos);
	//m_world_cam = world_cam;

	//world_cam.inverse();


	//viewMatrix = world_cam;
	///*projectionMatrix.setOrthoLH
	//(
	//	(this->getClientWindowRect().right - this->getClientWindowRect().left)/300.0f,
	//	(this->getClientWindowRect().bottom - this->getClientWindowRect().top)/300.0f,
	//	-4.0f,
	//	4.0f
	//);*/

	//int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	//int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	//projectionMatrix.setPerspectiveFovLV(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

	//m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

	SceneCameraHandler::getInstance()->update();
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	GraphicsEngine::get()->init();
	m_swap_chain=GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	SceneCameraHandler::initialize();
	

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;



	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs=GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

	//for(int i =0;i<3;i++)
	//{
	//	float randnumX = rand() % 100 - rand() % 100;
	//	float randnumY = rand() % 100 - rand() % 100;
	//	randnumX /= 100;
	//	randnumY /= 100;

	//	cube = new Cube("one", shader_byte_code, size_shader);
	//	cube->setAnimSpeed(0);
	//	cube->setPosition(Vector3D(randnumX, randnumY, 0));
	//	cube->setScale((Vector3D(.5, .5, .5)));
	//	gameObjList.push_back(cube);
	//}

	cube = new Cube("one", shader_byte_code, size_shader);
	cube->setAnimSpeed(0);
	cube->setPosition(Vector3D(0, 0, 0));
	cube->setScale((Vector3D(.25, .25, .25)));
	gameObjList.push_back(cube);

	/*plane = new Plane("two", shader_byte_code, size_shader);
	plane->setAnimSpeed(0);
	plane->setPosition(Vector3D(0, 0, 0));
	plane->setScale((Vector3D(.75, .01, .75)));
	gameObjList.push_back(plane);*/

	GraphicsEngine::get()->releaseCompiledShader();


	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f,0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);




	
	


	//GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	//GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	//GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	//GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	for(int i =0; i < gameObjList.size();i++)
	{
		gameObjList[i]->objectToCamera(viewMatrix, projectionMatrix);
		gameObjList[i]->update(m_delta_time);
		gameObjList[i]->draw(rc.right - rc.left, rc.bottom - rc.top, m_vs, m_ps);
	}



	update();

	

	/*SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);


	 FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(),0, 0);*/
	m_swap_chain->present(true);


	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = (m_old_delta)?((m_new_delta - m_old_delta) / 1000.0f):0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_ib->release();
	m_cb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

//void AppWindow::onKeyDown(int key)
//{
//	if (key == 'W')
//	{
//		//m_rot_x += 0.707f * m_delta_time;
//		m_forward = 1.0f;
//	}
//	else if (key == 'S')
//	{
//		//m_rot_x -= 0.707f * m_delta_time;
//		m_forward = -1.0f;
//	}
//	else if (key == 'A')
//	{
//		//m_rot_y += 0.707f * m_delta_time;
//		m_rightward = -1.0f;
//	}
//	else if (key == 'D')
//	{
//		//m_rot_y -= 0.707f * m_delta_time;
//		m_rightward = 1.0f;
//	}
//
//	cube->setRotation(Vector3D(m_rot_x, m_rot_y, 0));
//	
//}
//
//void AppWindow::onKeyUp(int key)
//{
//	m_forward = 0.0f;
//	m_rightward = 0.0f;
//}
//
//void AppWindow::onMouseMove(const Point& mouse_pos)
//{
//	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
//	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);
//
//	m_rot_x += (mouse_pos.m_y-(height / 2.0f)) * m_delta_time;
//	m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time;
//
//	InputSystem::get()->setCursorPosition(Point(width/2.0f, height/2.0f));
//
//	cube->setRotation(Vector3D(m_rot_x, m_rot_y, 0));
//}
//
//void AppWindow::onLeftMouseDown(const Point& mouse_pos)
//{
//	cube->setScale(Vector3D(.1f, .1f, .1f));
//}
//
//void AppWindow::onLeftMouseUp(const Point& mouse_pos)
//{
//	cube->setScale(Vector3D(.25f, .25f, .25f));
//}
//
//void AppWindow::onRightMouseDown(const Point& mouse_pos)
//{
//	cube->setScale(Vector3D(.5f, .5f, .5f));
//}
//
//void AppWindow::onRightMouseUp(const Point& mouse_pos)
//{
//	cube->setScale(Vector3D(.25f, .25f, .25f));
//}

