#pragma once
#include <string>
#include "Vector3D.h"
#include "Matrix4x4.h"

using namespace std;

class VertexShader;
class PixelShader;
class AGameObject
{
public:
	AGameObject(string name);
	~AGameObject();

	virtual void update(float deltaTime) = 0;
	virtual void draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader);

	void setPosition(float x, float y, float z);
	void setPosition(Vector3D pos);
	void setRotation(Vector3D rot);
	void setScale(Vector3D scale);
	Vector3D getLocalScale();
	Vector3D getLocalPosition();
	Vector3D getLocalRotation();

	string getName();

	void objectToCamera(Matrix4x4 view, Matrix4x4 projection);

	struct Vertex {
		Vector3D position;
		Vector3D color;
		Vector3D color2;
	};

	_declspec(align(16)) //make CBData a size of 16-bytes.
		struct CBData {
		Matrix4x4 worldMatrix;
		Matrix4x4 viewMatrix;
		Matrix4x4 projMatrix;
		//unsigned int time; //size of only 4 bytes. this won't align in GPU device because device requires 16 bytes.
		float time;
	};

protected:
	string name;
	Vector3D localPosition;
	Vector3D localRotation;
	Vector3D localScale;
	Matrix4x4 localMatrix;

	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;

};

