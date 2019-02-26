#include "sceneElements.h"


Ground *Ground::localElement = 0;

Ground::Ground()
{
	if (shader == 0)
	{
		shader = new ShaderBase("shaders\\vertexShaders\\groundShader.vs", "shaders\\fragmentShaders\\groundShader.fs");
		shaderProgramList.push_back(shader->ID);
	}

	mat4 projection = glm::perspective<float>(radians(45.f), 1.f, 0.1, 10.f);
	mat4 view(1.f);

	shader->use();
	shader->setVec3("color", vec3(0.8f, 0.8f, 0.8f));
	glLineWidth(lineWidth);
}

void Ground::setLineWidth(float lineWidth)
{
	if (lineWidth > 1.f)
	{
		this->lineWidth = lineWidth;
		glLineWidth(lineWidth);
	}
	else
	{
		string infoOutput = "ERROR::CoordAxes3D::lineWidth too small\n-------------------------------------------";
		cout << infoOutput << endl;
		LogInfo(infoOutput);
	}
}

Ground::~Ground()
{

}

void Ground::draw()
{
	shader->use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, countVertices);
}

void Ground::generateLines(float major, float left, float right, float near, float far)
{
	int verticalToViewSize = ((right - left) / major)+1;
	int parallelToViewSize = (far - near) / major;
	countVertices = (verticalToViewSize + parallelToViewSize) * 6;
	lines = (float*)malloc(countVertices * sizeof(float));

	float step = 0.f;
	int i, k;
	for (i = 0; i < verticalToViewSize; i++)
	{
		k = i * 6;
		lines[k] = left + step;
		lines[k + 1] = 0.f;
		lines[k + 2] = -near;

		lines[k + 3] = left + step;
		lines[k + 4] = 0.f;
		lines[k + 5] = -(far - major);

		step += major;
	}

	step = 0.f;

	for (; i < verticalToViewSize + parallelToViewSize; i++)
	{
		k = i * 6;
		lines[k] = left;
		lines[k + 1] = 0.f;
		lines[k + 2] = -(near + step);

		lines[k + 3] = right;
		lines[k + 4] = 0.f;
		lines[k + 5] = -(near + step);
		step += major;
	}

	shader->use();
	GLuint lineVertexID = shader->GetAttribLocation("lineVertex");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*countVertices, lines, GL_STATIC_DRAW);

	glVertexAttribPointer(lineVertexID, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(lineVertexID);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Ground *Ground::getGround(float major, float left, float right, float near, float far)
{
	localElement = new Ground();
	localElement->generateLines(major, left, right, near, far);
	return localElement;
}

void Ground::setColor(float r, float g, float b)
{
	shader->use();
	shader->setVec3("color", vec3(r, g, b));
}