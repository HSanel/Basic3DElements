#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "shaders.h"

using namespace glm;
using namespace std;

class SceneObjectBase
{
	friend class Camera;
protected:
	vector<unsigned int> shaderProgramList;
public:
	SceneObjectBase();
	~SceneObjectBase();
	virtual void draw();
};

class Scene
{
private:
	map<string, Camera*>  camTable;
	map<string, SceneObjectBase*> objectTable;
	string actualCam;
public:
	Scene();
	~Scene();

	void registerCam(Camera *cam, string name);
	void changeCam(string name);
	void deleteCam(string name);
	Camera* getCam(string name);
	Camera* getActiveCam();
	string getActiveCamName();
	void addSceneObject(SceneObjectBase *object, string name);
	void deleteSceneObject(string name);
	SceneObjectBase* getSceneObject(string name);
	void draw();
};

class Model
{
private:
	vec3 position;
	mat4 modelMatrix;
	float scale;
public:
	Model();
	~Model();

	void Scale(float value);
	void SetPosition(float x, float y, float z);
};

class CoordAxes3D : public SceneObjectBase
{
private:
	const ShaderBase *shaderLine = 0;
	const ShaderBase *shaderArrow = 0;
	vec3 xColor, yColor, zColor;
	float length;
	float lineWidth = 2.f;
	float verticesLine[18] = {
		0.0f, 0.0f, 0.0f,  //origin x
		0.0f, 0.0f, 0.0f,  //origin y
		0.0f, 0.0f, 0.0f,  //origin z
		1.0f, 0.0f, 0.0f,  //x
		0.0f, 1.0f, 0.0f,  //y
		0.0f, 0.0f, 1.0f   //z
	};

	float colorLine[18] = {
		1.0f, 0.0f, 0.0f,  //origin x
		0.0f, 1.0f, 0.0f,  //origin y
		0.0f, 0.0f, 1.0f,  //origin z
		1.0f, 0.0f, 0.0f,  //x
		0.0f, 1.0f, 0.0f,  //y
		0.0f, 0.0f, 1.0f   //z
	};

	int indicesLine[6] = {
		0, 3,
		1, 4,
		2, 5
	};

	float verticesArrow[15] = {
		0.3f ,0.f, 0.f,
		0.0f, 0.1f, 0.1f,
		0.0f, -0.1f, 0.1f,
		0.0f, -0.1f, -0.1f,
		0.0f, 0.1f, -0.1f
	};

	float colorArrow[9] = {
		1.0f, 0.0f, 0.0f,  
		0.0f, 1.0f, 0.0f,  
		0.0f, 0.0f, 1.0f,  
	};

	int indicesArrow[7] = {
		0,1,
		2,3,
		0,4,1

	};

	mat4 model;
	mat4 view;
	mat4 projection;
	GLuint VAOLine, VBOLine[2], EBOLine;
	mat4 matArrowX, matArrowY, matArrowZ;
	GLuint VAOArrow, VBOArrow[2], EBOArrow, UBOArrow;
	static unsigned int matArrowBindingPoint;
	void _updateColor();
	vec3 _normalize(vec3 vec);
public:
	CoordAxes3D();
	~CoordAxes3D();
	void setPosition(float x, float y, float z);
	void rotate(float degree, float x, float y, float z);
	void setLength(float value);
	void draw();
	void setLineWidth(float lineWidth);
	void setXcolor(float r, float g, float b);
	void setYcolor(float r, float g, float b);
	void setZcolor(float r, float g, float b);
	void align(vec3 xAxis, vec3 yAxis);
	void align(float xAxis[3], float yAxis[3]);
};

class Camera
{
private:
	vec3 position;
	CoordAxes3D *coordAxes3D;
	mat4 viewMatrix, projectionMatrix;
	float width, height;
	vec3 cameraPos, cameraFront, cameraUp;
	vec3 *cameraRotationPoint;
	float yaw, pitch, lastXDeg, lastYDeg;
	int firstRot = 1;
	static unsigned int UBO;
	const static unsigned int bindingMatricesPoint = 0;
	static int UBOcreated;
	void registerUBO();
	void refreshView();
public:
	Camera();
	~Camera();

	void registerSceneObject(SceneObjectBase *sceneObject);
	void rotate(float xDeg, float yDeg, vec3 *point = NULL);
	void translate(vec3 position);
	void setOrthoProjection(float left, float right, float top, float bottom, float near, float far);
	void setPerspectiveProjection(float viewWideInDeg, float screenWidth,float screenHight, float near, float far);
	void align(vec3 position, vec3 lookAtPoint);
};

class Ground : public SceneObjectBase
{
private:
	float major;
	float *lines;
	float lineWidth = 2.f;
	int countVertices;
	unsigned int VAO, VBO;
	static Ground *localElement;
	ShaderBase *shader;
	Ground();
public:
	~Ground();
	void draw();
	void setLineWidth(float lineWidth);
	void generateLines(float major, float left, float right, float near, float far);
	static Ground *getGround(float major, float left, float right, float near, float far);
	void setColor(float r, float g, float b);
};



