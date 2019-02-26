#include "sceneElements.h"
int Camera::UBOcreated = 0;
unsigned int Camera::UBO = 0;

Camera::Camera()
{
	projectionMatrix = ortho<float>(-10.f, 10.f, -10.f, 10.f, 0.f, 10.f);
	
	cameraPos = vec3(0.f, 0.f, 3.f);
	cameraFront = vec3(0.f, 0.f, -1.f);
	cameraUp = vec3(0.f, 1.f, 0.f);

	viewMatrix = glm::lookAt(
		cameraPos,
		cameraPos + cameraFront,
		cameraUp
	);


	if (!UBOcreated)
		registerUBO();
}

Camera::~Camera()
{

}

void Camera::registerUBO()
{
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, bindingMatricesPoint, UBO);
	UBOcreated = 1;
}

void Camera::align(vec3 camPosition, vec3 lookAtPoint)
{
	vec3 vecUp;
	if (camPosition.x == 0.f && camPosition.z == 0.f)
		vecUp = vec3(0.f, 0.f, 1.f);
	else
		vecUp = vec3(0.f, 1.f, 0.f);

	viewMatrix = glm::lookAt(
		camPosition,
		lookAtPoint,
		vecUp
	);

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), &viewMatrix[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::refreshView()
{
	viewMatrix = glm::lookAt(
		cameraPos,
		cameraPos + cameraFront,
		cameraUp
	);

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), &viewMatrix[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::rotate(float xDeg, float yDeg, vec3 *point)
{
	if (xDeg != 0.f || yDeg != 0.f)
	{
		yaw += xDeg;
		pitch += yDeg;

		if (pitch > 89.f)
			pitch = 89.f;
		if (pitch < -89.f)
			pitch = -89.f;

		if (point == NULL)
		{
			if (firstRot)
			{
				yaw = -90.f;
				pitch = 0.f;
				firstRot = 0;
			}

			vec3 front;
			front.x = cos(radians(yaw)) * cos(radians(pitch));
			front.y = sin(radians(pitch));
			front.z = sin(radians(yaw)) * cos(radians(pitch));
			cameraFront = normalize(front);
			refreshView();
		}
		else
		{	
			vec3 RVec = *point - cameraPos;
			vec3 front;
			
			float r = sqrt(RVec.x * RVec.x + RVec.y * RVec.y + RVec.z * RVec.z);
			cameraPos = *point;
			
			front.x = cos(radians(yaw)) * cos(radians(pitch));
			front.y = sin(radians(pitch));
			front.z = sin(radians(yaw)) * cos(radians(pitch));
			
			cameraFront = -normalize(front);
						
			cameraPos = cameraPos - cameraFront * r;

			refreshView();
		}		
	}	
}

void Camera::translate(vec3 position)
{
	cameraPos -= cameraFront * position.z;
	cameraPos += normalize(cross(cameraFront, cameraUp)) * position.x;
	cameraPos -= cameraUp * position.y;
		
	refreshView();
}

void Camera::setOrthoProjection(float left, float right, float bottom, float top, float near, float far)
{
	projectionMatrix = ortho<float>(left, right, bottom, top, near, far);

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), &projectionMatrix[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::setPerspectiveProjection(float viewWideInDeg, float screenWidth, float screenHight, float near, float far)
{
	projectionMatrix = perspective<float>(radians(viewWideInDeg), screenWidth / screenHight, near, far);

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), &projectionMatrix[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::registerSceneObject(SceneObjectBase *sceneObject)
{
	for (int i = 0; i < sceneObject->shaderProgramList.size(); i++)
	{
		unsigned int programID = sceneObject->shaderProgramList[i];
		unsigned int matrices_index = glGetUniformBlockIndex(programID, "Matrices");
		glUniformBlockBinding(programID, matrices_index, bindingMatricesPoint);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), value_ptr(projectionMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), value_ptr(viewMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

