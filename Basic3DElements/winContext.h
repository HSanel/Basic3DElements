#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

using namespace std;

int LogInfo(string info);

class Form
{
private: 
	GLFWwindow *window;
	int width, height;
public:
	Form(int, int);
	~Form();
	void show();
	void destroy();
	int ShouldClose();
	void draw();
	void PollEvents();
	void Clear(float, float, float);
	void getWindowSize(int &, int &);
	void initOpenGL();
	GLFWwindow* getWindow();

};

