#include "sceneElements.h"

unsigned int CoordAxes3D::matArrowBindingPoint = 1;

CoordAxes3D::CoordAxes3D()
{
	if (shaderLine == 0)
	{
		shaderLine = new ShaderBase("shaders\\vertexShaders\\axesShader.vs", "shaders\\fragmentShaders\\axesShader.fs");
		shaderProgramList.push_back(shaderLine->ID);
	}

	if (shaderArrow == 0)
	{
		shaderArrow = new ShaderBase("shaders\\vertexShaders\\axesShader.vs", "shaders\\fragmentShaders\\axesShader.fs");
		shaderProgramList.push_back(shaderArrow->ID);
	}
#pragma region Lines

	GLuint arrowID =  shaderLine->GetAttribLocation("arrow");
	GLuint colorID = shaderLine->GetAttribLocation("color");

	glGenVertexArrays(1, &VAOLine);
	glGenBuffers(2, VBOLine);
	glGenBuffers(1, &EBOLine);

	glBindVertexArray(VAOLine);

	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOLine);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesLine), indicesLine, GL_DYNAMIC_DRAW);

	//vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBOLine[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLine), verticesLine, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(arrowID, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
	glEnableVertexAttribArray(arrowID);

	//color
	glBindBuffer(GL_ARRAY_BUFFER, VBOLine[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorLine), colorLine, GL_DYNAMIC_DRAW);
	
	glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(colorID);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#pragma endregion

#pragma region Arrow
	arrowID = shaderArrow->GetAttribLocation("arrow");
	colorID = shaderArrow->GetAttribLocation("color");

	glGenVertexArrays(1, &VAOArrow);
	glGenBuffers(2, VBOArrow);
	glGenBuffers(1, &EBOArrow);
	glGenBuffers(1, &UBOArrow);

	glBindVertexArray(VAOArrow);

	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOArrow);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesArrow), indicesArrow, GL_DYNAMIC_DRAW);

	//vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBOArrow[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesArrow), verticesArrow, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(arrowID, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(arrowID);

	//color
	glBindBuffer(GL_ARRAY_BUFFER, VBOArrow[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorArrow), colorArrow, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(colorID);
	glVertexAttribDivisor(colorID, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//xyz Transformation
	matArrowX = translate(mat4(1.f), vec3(1.f, 0.f, 0.f));

	matArrowY = glm::rotate(mat4(1.f), radians(90.f), vec3(0.f, 0.f, 1.f));
	matArrowY = translate(matArrowY, vec3(1.f, 0.f, 0.f));

	matArrowZ = glm::rotate(mat4(1.f), radians(-90.f), vec3(0.f, 1.f, 0.f));
	matArrowZ = translate(matArrowZ, vec3(1.f, 0.f, 0.f));

	glBindBuffer(GL_UNIFORM_BUFFER, UBOArrow);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(mat4), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), &matArrowX[0][0]);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), &matArrowY[0][0]);
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(mat4), sizeof(mat4), &matArrowZ[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	unsigned int matArrow_index = glGetUniformBlockIndex(shaderArrow->ID, "MatArrow");
	glUniformBlockBinding(shaderArrow->ID, matArrow_index, matArrowBindingPoint);
	glBindBufferBase(GL_UNIFORM_BUFFER, matArrowBindingPoint, UBOArrow);
#pragma endregion

	model = mat4(1.f);
	shaderLine->use();
	shaderLine->setMat4("model", model);
	shaderLine->setBool("isArrow", false);

	shaderArrow->use();
	shaderArrow->setMat4("model", model);
	shaderArrow->setBool("isArrow", true);
}

CoordAxes3D::~CoordAxes3D()
{
}
 
void CoordAxes3D::setPosition(float x, float y, float z)
{
	shaderLine->use();
	shaderLine->setVec3("translationVec", vec3(x, y, z));

	shaderArrow->use();
	shaderArrow->setVec3("translationVec", vec3(x, y, z));
	//model = translate(model, vec3(x, y, z));
	//shader->setMat4("model", model);
}

void CoordAxes3D::rotate(float degree, float x, float y, float z)
{
	shaderLine->use();
	shaderLine->setFloat("angle_deg", degree);
	shaderLine->setVec3("rotationVec", vec3(x, y, z));
	//float rad = (degree/360.f) * glm::pi<float>();
	//model = glm::rotate(model, rad, vec3(x, y, z));
	//shader->setMat4("model", model);
}

void CoordAxes3D::setLength(float value)
{
	shaderLine->use();
	model = glm::scale(model, vec3(value, value, value));
	shaderLine->setMat4("model", model);

	shaderArrow->use();
	model = glm::scale(model, vec3(value, value, value));
	matArrowX = translate(matArrowX, vec3(value, 0.f, 0.f));
	matArrowY = translate(matArrowY, vec3(0.f, value, 0.f));
	matArrowZ = translate(matArrowZ, vec3(0.f, 0.f, value));
	shaderArrow->setMat4("model", model);
}

void CoordAxes3D::setLineWidth(float lineWidth)
{
	if (lineWidth > 1.f)
	{
		this->lineWidth = lineWidth;
	}
	else
	{
		string infoOutput = "ERROR::CoordAxes3D::lineWidth too small\n-------------------------------------------";
		cout << infoOutput << endl;
		LogInfo(infoOutput);
	}
}

void CoordAxes3D::draw()
{
	glLineWidth(lineWidth);
	shaderLine->use();
	glBindVertexArray(VAOLine);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

	shaderArrow->use();
	glBindVertexArray(VAOArrow);
	glDrawElementsInstanced(GL_TRIANGLE_STRIP, 7, GL_UNSIGNED_INT, 0, 3);
}

void CoordAxes3D::align(vec3 xAxis, vec3 yAxis)
{
	vec3 nXAxis = _normalize(xAxis);
	vec3 nYAxis = _normalize(yAxis);


}

void CoordAxes3D::setXcolor(float r, float g, float b)
{
	int line = 1;
	int i1 = 3 * (line - 1);
	int i2 = i1 + 1;
	int i3 = i1 + 2;
	colorLine[i1] = r;
	colorLine[i2] = g;
	colorLine[i3] = b;

	colorLine[i1 + 9] = r;
	colorLine[i2 + 9] = g;
	colorLine[i3 + 9] = b;

	colorArrow[0] = r;
	colorArrow[1] = g;
	colorArrow[2] = b;

	_updateColor();
}
void CoordAxes3D::setYcolor(float r, float g, float b)
{
	int line = 2;
	int i1 = 3 * (line - 1);
	int i2 = i1 + 1;
	int i3 = i1 + 2;
	colorLine[i1] = r;
	colorLine[i2] = g;
	colorLine[i3] = b;

	colorLine[i1 + 9] = r;
	colorLine[i2 + 9] = g;
	colorLine[i3 + 9] = b;

	colorArrow[3] = r;
	colorArrow[4] = g;
	colorArrow[5] = b;

	_updateColor();
}
void CoordAxes3D::setZcolor(float r, float g, float b)
{
	int line = 3;
	int i1 = 3 * (line - 1);
	int i2 = i1 + 1;
	int i3 = i1 + 2;
	colorLine[i1] = r;
	colorLine[i2] = g;
	colorLine[i3] = b;

	colorLine[i1 + 9] = r;
	colorLine[i2 + 9] = g;
	colorLine[i3 + 9] = b;

	colorArrow[6] = r;
	colorArrow[7] = g;
	colorArrow[8] = b;

	_updateColor();
}

void CoordAxes3D::_updateColor()
{
	GLuint colorID = shaderLine->GetAttribLocation("color");
	glBindBuffer(GL_ARRAY_BUFFER, VBOLine[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorLine), colorLine, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(colorID);

	colorID = shaderArrow->GetAttribLocation("color");
	glBindBuffer(GL_ARRAY_BUFFER, VBOArrow[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorArrow), colorArrow, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(colorID);
	glVertexAttribDivisor(colorID, 1);
}

vec3 CoordAxes3D::_normalize(vec3 vec)
{
	float length = glm::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	return vec * (1 / length);
}
