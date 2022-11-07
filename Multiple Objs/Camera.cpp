#include "Camera.h"
#include "InputSystem.h"
#include "EngineTime.h"

Camera::Camera(string name) : AGameObject(name)
{
	this->forwardDirection = Vector3D(1.0f, 0.0f, 1.0f);
	this->backwardDirection = Vector3D(-1.0f, 0.0f, -1.0f);

	this->setPosition(0.0f, 0.0f, -4.0f);
	this->updateViewMatrix();
	InputSystem::get()->addListener(this);
}

Camera::~Camera()
{
	InputSystem::get()->removeListener(this);
}

void Camera::update(float deltaTime)
{
	Vector3D localPos = this->getLocalPosition();
	float x = localPos.getX();
	float y = localPos.getY();
	float z = localPos.getZ();
	float moveSpeed = 10.f;

	if (InputSystem::get()->isKeyDown('W')) {
		z += deltaTime * moveSpeed;
		this->setPosition(x, y, z);
		this->updateViewMatrix();
	}
	else if (InputSystem::get()->isKeyDown('S')) {
		z -= deltaTime * moveSpeed;
		this->setPosition(x, y, z);
		this->updateViewMatrix();
	}
	else if (InputSystem::get()->isKeyDown('A')) {
		x -= deltaTime * moveSpeed;
		this->setPosition(x, y, z);
		this->updateViewMatrix();
	}
	else if (InputSystem::get()->isKeyDown('D')) {
		x += deltaTime * moveSpeed;
		this->setPosition(x, y, z);
		this->updateViewMatrix();
	}
	else if (InputSystem::get()->isKeyDown('Q')) {
		y -= deltaTime * moveSpeed;
		this->setPosition(x, y, z);
		this->updateViewMatrix();
	}
	else if (InputSystem::get()->isKeyDown('E')) {
		y += deltaTime * moveSpeed;
		this->setPosition(x, y, z);
		this->updateViewMatrix();
	}
}

Matrix4x4 Camera::getViewMatrix()
{
	return this->localMatrix;
}

void Camera::onKeyDown(int key)
{
}

void Camera::onKeyUp(int key)
{
}

void Camera::onMouseMove(const Point& mouse_pos)
{
	if (this->mouseDown) {
		Vector3D localRot = this->getLocalRotation();
		float x = localRot.getX();
		float y = localRot.getY();
		float z = localRot.getZ();
		
		float speed = 0.05f;
		x += (mouse_pos.m_y - (768 / 2.0f))* speed * EngineTime::getDeltaTime();
		y += (mouse_pos.m_x - (1024 / 2.0f)) * speed * EngineTime::getDeltaTime();

		this->setRotation(Vector3D(x, y, z));
		InputSystem::get()->setCursorPosition(Point(1024 / 2.0f, 768 / 2.0f));
		this->updateViewMatrix();
	}
}

void Camera::onLeftMouseDown(const Point& mouse_pos)
{
}

void Camera::onLeftMouseUp(const Point& mouse_pos)
{
}

void Camera::onRightMouseDown(const Point& mouse_pos)
{
	this->mouseDown = true;
}

void Camera::onRightMouseUp(const Point& mouse_pos)
{
	this->mouseDown = false;
}

void Camera::updateViewMatrix()
{
	Matrix4x4 worldCam; worldCam.setIdentity();
	Matrix4x4 temp; temp.setIdentity();

	Vector3D localRot = this->getLocalRotation();

	temp.setRotationX(localRot.getX());
	worldCam = worldCam.multiplyTo(temp);

	temp.setRotationY(localRot.getY());
	worldCam = worldCam.multiplyTo(temp);

	temp.setTranslation(this->getLocalPosition());
	worldCam = worldCam.multiplyTo(temp);

	worldCam.inverse();
	this->localMatrix = worldCam;
}




