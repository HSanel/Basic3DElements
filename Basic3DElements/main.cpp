#include <iostream>
#include <stdio.h>
#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "winContext.h"
#include "sceneElements.h"

using namespace std;
//beim fertigen Programm die Einstellung in "Project Eigenschaften" unter C/C++, Codegenerierung die Laufzeitbibliothek von /MDd auf /MD umstellen
//5 commit
#define H 800
#define W 1000

Form *mainWin;

void keyControl(GLFWwindow* window, Scene* scene)
{
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		scene->changeCam("front");
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		scene->changeCam("side");
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		scene->getActiveCam()->translate(vec3(-0.05f, 0.f, 0.f));
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		scene->getActiveCam()->translate(vec3(0.05f, 0.f, 0.0f));
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		scene->getActiveCam()->translate(vec3(0.0f, 0.05f, 0.f));
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		scene->getActiveCam()->translate(vec3(0.0f, -0.1f, 0.0f));

	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		scene->getActiveCam()->translate(vec3(0.0f, 0.f, 0.1f));
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		scene->getActiveCam()->translate(vec3(0.0f, 0.0f, -0.1f));

	else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		scene->getActiveCam()->rotate(0.0,0.4/*, &vec3(-5.f, 2.f, 0.f)*/);
	else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		scene->getActiveCam()->rotate(0.0, -0.4 /*&vec3(-5.f, 2.f, 0.f)*/);
	else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		scene->getActiveCam()->rotate(-0.4, 0.0 /*&vec3(-5.f, 2.f, 0.f)*/);
	else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		scene->getActiveCam()->rotate(0.4, 0.0/*, &vec3(-5.f, 2.f, 0.f)*/);
}

int main(int argc, char* argv[])
{
	mainWin = new Form(W,H);
	mainWin->show();
	mainWin->initOpenGL();
	

	CoordAxes3D *axes = new CoordAxes3D();
	CoordAxes3D *axes2 = new CoordAxes3D();
	axes2->setPosition(-5.f, 2.f, 0.f);
	
	Camera *cam = new Camera();
	Camera *cam2 = new Camera();  
	//cam2->align(vec3(2.f, 0.f, 1.f), vec3(0.f, 0.f, 0.f));
	cam->setPerspectiveProjection(45.f, W, H , 0.1f, 100.f);
	cam2->setPerspectiveProjection(45.f, W, H, 0.1f, 100.f);
	//cam->setOrthoProjection(-10.f, 10.f, -10.f, 10.f, 0.1, 10.f);
	//cam2->setOrthoProjection(-10.f, 10.f, -10.f, 10.f, 0.1, 10.f);
	cam2->translate(vec3(5.f, 0.f, 0.f));
	cam2->rotate(0.f, 90.f);
	Ground *ground = Ground::getGround(1.f, -10.f, 10.f, -10.f, 10.f);

	Scene *scene = new Scene();
	scene->registerCam(cam, "front");
	scene->registerCam(cam2, "side");
	scene->addSceneObject(ground, "ground");
	scene->addSceneObject(axes, "firstAxes");
	scene->addSceneObject(axes2, "secondAxes");
	
	scene->changeCam("front");
	while (!mainWin->ShouldClose())
	{
		keyControl(mainWin->getWindow(), scene);
		mainWin->Clear(0.6f, 0.6f, 0.6f);

		//axes->draw();
		scene->draw();
		//ground->draw();

		mainWin->draw();
		mainWin->PollEvents();
	}
}

//int main(int argc, char* argv[])
//{
//	mainWin = new Form(W, H);
//	mainWin->show();
//	mainWin->initOpenGL();
//
//	ShaderBase *shader = new ShaderBase("shaders\\vertexShaders\\groundShader.vs", "shaders\\fragmentShaders\\groundShader.fs");
//
//	float right = 10.f, left = -10.f, major = 1.f, near = 0.1f, far = 10.f;
//	mat4 projection = glm::perspective<float>(radians(80.f), W / H, near, far);
//	mat4 view(1.f);
//	
//
//	float *lines = (float*)malloc(sizeof(float*) * 6);
//
//	int verticalToViewSize = (right - left) / major;
//	int parallelToViewSize = (far - near) / major;
//	int countVertices = (verticalToViewSize + parallelToViewSize) * 6;
//	lines = (float*)malloc(countVertices * sizeof(float));
//
//	float step = 0.f;
//	int i, k;
//	for (i = 0; i < verticalToViewSize; i++)
//	{
//		k = i * 6;
//		lines[k] = left + step;
//		lines[k + 1] = 1.f;
//		lines[k + 2] = -(near + 0.1);
//
//		lines[k + 3] = left + step;
//		lines[k + 4] = 1.f;
//		lines[k + 5] = -far;
//
//		step += major;
//	}
//
//	step = 0.f;
//	float step2 = 0.f;
//	for (; i < verticalToViewSize + parallelToViewSize; i++)
//	{
//		k = i * 6;
//		lines[k] = left;
//		lines[k + 1] = 1.f;
//		lines[k + 2] = -(near + step);
//
//		lines[k + 3] = right;
//		lines[k + 4] = 1.f;
//		lines[k + 5] = -(near + step);
//		step += major;
//		step2 += 2.f / parallelToViewSize;
//	}
//
//	GLuint lineVertexID = shader->GetAttribLocation("lineVertex");
//	int a = _msize(lines);
//	unsigned int VAO, VBO;
//
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//
//	glBindVertexArray(VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, _msize(lines), &lines[0], GL_STATIC_DRAW);
//
//	glVertexAttribPointer(lineVertexID, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
//	glEnableVertexAttribArray(lineVertexID);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//	shader->use();
//	shader->setVec3("color", vec3(0.0f, 0.0f, 0.0f));
//	shader->setMat4("p", projection);
//	shader->setMat4("v", view);
//
//
//	while (!mainWin->ShouldClose())
//	{
//		mainWin->Clear(0.7f, 0.7f, 0.7f);
//		glLineWidth(2.f);
//		shader->use();
//		glBindVertexArray(VAO);
//		glDrawArrays(GL_LINES, 0, countVertices);
//
//		mainWin->draw();
//		mainWin->PollEvents();
//	}
//}