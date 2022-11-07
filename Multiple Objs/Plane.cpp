#include "Plane.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include <iostream>
#include "SceneCameraHandler.h"
#include "Matrix4x4.h"

#include "AppWindow.h"

Plane::Plane(string name, void* shaderByteCode, size_t sizeShader):AGameObject(name)
{
	//create buffers for drawing. vertex data that needs to be drawn are temporarily placed here.
	Vertex quadList[] = {
		//X, Y, Z
		//FRONT FACE
		{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1,1,1),  Vector3D(1,1,1) },
		{Vector3D(-0.5f,0.5f,-0.5f),    Vector3D(1,1,1), Vector3D(1,1,1) },
		{Vector3D(0.5f,0.5f,-0.5f),   Vector3D(1,1,1),  Vector3D(1,1,1) },
		{Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,1,1), Vector3D(1,1,1) },

		//BACK FACE
		{Vector3D(0.5f,-0.5f,0.5f),    Vector3D(1,1,1), Vector3D(1,1,1) },
		{Vector3D(0.5f,0.5f,0.5f),    Vector3D(1,1,1), Vector3D(1,1,1) },
		{Vector3D(-0.5f,0.5f,0.5f),   Vector3D(1,1,1),  Vector3D(1,1,1) },
		{Vector3D(-0.5f,-0.5f,0.5f),     Vector3D(1,1,1), Vector3D(1,1,1) },
	};

	this->vertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	this->vertexBuffer->load(quadList, sizeof(Vertex), ARRAYSIZE(quadList), shaderByteCode, sizeShader);

	unsigned int indexList[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		1,6,5,
		5,2,1,
		//BOTTOM SIDE
		7,0,3,
		3,4,7,
		//RIGHT SIDE
		3,2,5,
		5,4,3,
		//LEFT SIDE
		7,6,1,
		1,0,7
	};
	this->indexBuffer = GraphicsEngine::get()->createIndexBuffer();
	this->indexBuffer->load(indexList, ARRAYSIZE(indexList));

	//create constant buffer
	CBData cbData = {};
	cbData.time = 0;
	this->constantBuffer = GraphicsEngine::get()->createConstantBuffer();
	this->constantBuffer->load(&cbData, sizeof(CBData));
}

Plane::~Plane()
{
	this->vertexBuffer->release();
	this->indexBuffer->release();
	AGameObject::~AGameObject();
}

void Plane::update(float deltaTime)
{
	this->ticks += deltaTime;
	this->deltaTime = deltaTime;
}

void Plane::draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader)
{
	GraphicsEngine* graphEngine = GraphicsEngine::get();
	DeviceContext* deviceContext = graphEngine->getImmediateDeviceContext();
	CBData cbData = {};
	//cbData.time = this->ticks * this->speed;

	if (this->deltaPos > 1.0f) {
		this->deltaPos = 0.9f;
		this->speed *= -1.0f;
	}
	else if (this->deltaPos < 0.25f) {
		this->deltaPos = 0.26f;
		this->speed *= -1.0f;
	}
	else {
		this->deltaPos += this->deltaTime * 0.1f;
	}

	
	//Matrix4x4 scaleMatrix; scaleMatrix.setScale(Vector3D::lerp(Vector3D(0.5f, 0.5f, 0.5f), Vector3D(1.0f, 1.0f, 1.0f), (sin(this->deltaPos * this->speed) + 1.0f) / 2.0f));
	//cbData.worldMatrix = cbData.worldMatrix.multiplyTo(scaleMatrix);

	Matrix4x4 allMatrix; allMatrix.setIdentity();
	Matrix4x4 translationMatrix; translationMatrix.setIdentity();  translationMatrix.setTranslation(this->getLocalPosition());
	Matrix4x4 scaleMatrix; scaleMatrix.setScale(this->getLocalScale());
	Vector3D rotation = this->getLocalRotation();
	Matrix4x4 zMatrix; zMatrix.setRotationZ(rotation.getZ());
	Matrix4x4 xMatrix; xMatrix.setRotationX(rotation.getX());
	Matrix4x4 yMatrix; yMatrix.setRotationY(rotation.getY());

	//Scale --> Rotate --> Transform as recommended order.
	Matrix4x4 rotMatrix; rotMatrix.setIdentity();
	rotMatrix = rotMatrix.multiplyTo(xMatrix.multiplyTo(yMatrix.multiplyTo(zMatrix)));
	allMatrix = allMatrix.multiplyTo(scaleMatrix.multiplyTo(rotMatrix));
	allMatrix = allMatrix.multiplyTo(translationMatrix);
	cbData.worldMatrix = allMatrix;

	//cbData.worldMatrix = cbData.worldMatrix.multiplyTo(xMatrix.multiplyTo(yMatrix.multiplyTo(zMatrix)));
	//cbData.worldMatrix *= trans;
	//std::cout << this->getLocalPosition().m_x;
	//cbData.viewMatrix.setIdentity();
	//cbData.projMatrix.setOrthoLH(width / 400.0f, height / 400.0f, -4.0f, 4.0f);

	Matrix4x4 cameraMatrix = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
	cbData.viewMatrix = cameraMatrix;

	float aspectRatio = (float)width / (float)height;
	cbData.projMatrix.setPerspectiveFovLV(aspectRatio, aspectRatio, 0.1f, 1000.0f);

	this->constantBuffer->update(deviceContext, &cbData);
	deviceContext->setConstantBuffer(vertexShader, this->constantBuffer);
	deviceContext->setConstantBuffer(pixelShader, this->constantBuffer);

	deviceContext->setVertexShader(vertexShader);
	deviceContext->setPixelShader(pixelShader);

	deviceContext->setVertexBuffer(this->vertexBuffer);
	deviceContext->setIndexBuffer(this->indexBuffer);

	deviceContext->drawIndexedTriangleList(this->indexBuffer->getSizeIndexList(), 0, 0);
}

void Plane::setAnimSpeed(float speed)
{
	this->speed = speed;
}
