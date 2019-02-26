#include "winContext.h"

int LogInfo(string info)
{
	try
	{
		fstream f;
		f.open("LogData.txt", ios::app | ios::out);
		f << info << endl;
		f.close();
	}
	catch (...)
	{
		cout << "ERROR::LOGGER::INFO_NOT_SUCCESFULLY_WRITE" << endl;
		return 0;
	}
	return 1;
}


Form::Form(int width, int height)
{
	this->width = width;
	this->height = height;

	if (!glfwInit())
	{
		cout << "GLFW konnte nicht initialisirt werden" << endl;
		glfwTerminate();
	}

	window = glfwCreateWindow(width, height, "AnatomieSimulator", NULL, NULL);

	if (!window)
	{
		cout << "Fenster konnte nicht erzeugt werden" << endl;
		glfwDestroyWindow(window);
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}



Form::~Form()
{
	glfwTerminate();
	glfwDestroyWindow(window);
}

void Form::show()
{
	glfwMakeContextCurrent(window);
	glEnable(GL_DEPTH_TEST);
}

void Form::initOpenGL()
{
	if (glewInit())
	{
		string infoOutput = "ERROR::FORM::OPENGL::GLEW_NOT_SUCCESFULLY_INITIALIZE\n------------------------------------------------------";
		cout << infoOutput << endl;
		LogInfo(infoOutput);
	}
}

void Form::destroy()
{
	glfwDestroyWindow(window);
}

int Form::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Form::draw()
{
	glfwSwapBuffers(window);
}

void Form::PollEvents()
{
	glfwPollEvents();
}

void Form::Clear(float red, float green, float blue)
{
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Form::getWindowSize(int &width, int &height)
{
	width = this->width;
	height = this->height;
}

GLFWwindow* Form::getWindow()
{
	return window;
}
