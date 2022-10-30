#include "AGameObject.h"

AGameObject::AGameObject(string name)
{
	this->name = name;
}

AGameObject::~AGameObject()
{
}

void AGameObject::setPosition(float x, float y, float z)
{
	this->localPosition = Vector3D(x, y, z);
}

void AGameObject::setPosition(Vector3D pos)
{
	this->localPosition = pos;
}

void AGameObject::setRotation(Vector3D rot)
{
	this->localRotation = rot;
}

void AGameObject::setScale(Vector3D scale)
{
	this->localScale = scale;
}

Vector3D AGameObject::getLocalScale()
{
	return this->localScale;
}

Vector3D AGameObject::getLocalPosition()
{
	return this->localPosition;
}

Vector3D AGameObject::getLocalRotation()
{
	return this->localRotation;
}

string AGameObject::getName()
{
	return this->name;
}

void AGameObject::objectToCamera(Matrix4x4 view, Matrix4x4 projection)
{
	viewMatrix = view;
	projectionMatrix = projection;


}


