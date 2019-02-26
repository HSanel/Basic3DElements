#version 330 core
in vec3 arrow;
in vec3 color;

uniform mat4 model;

uniform float scale;
uniform float angle_deg;
uniform vec3 rotationVec;
uniform vec3 translationVec;
uniform bool isArrow;

layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

layout (std140) uniform MatArrow
{
	mat4 matArrowX;
	mat4 matArrowY;
	mat4 matArrowZ;
};

out vec3 oColor;
float cRad, sRad;

vec3 makeUnitvector(vec3 vec)
{
	float abs = 1/sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	return vec3(vec.x * abs, vec.y * abs, vec.z * abs);
}
	
void main()
{	
	
	float rad = radians(angle_deg);
	mat4 initModel, resultModel, modelScale, modelRot, modelTrans;
	
	//--------------initialize Models----------------
	
	initModel[0].xyzw = vec4(1.0,0.0,0.0,0.0);	
	initModel[1].xyzw = vec4(0.0,1.0,0.0,0.0);	
	initModel[2].xyzw = vec4(0.0,0.0,1.0,0.0);	
	initModel[3].xyzw = vec4(0.0,0.0,0.0,1.0);	
	
	resultModel = initModel;
	modelScale = initModel;
	modelRot = initModel;
	modelTrans = initModel;
	//--------------scale Matrix---------------------
	
	if(scale != 0)
	{
		modelScale = modelScale * scale;
		modelScale[3][3] = 1;
	}
	
	//--------------rotation Matrix-------------------
	cRad = cos(rad);
	sRad = sin(rad);
	
	vec3 rotVec;
	
	if(rotationVec != vec3(0.0, 0.0, 0.0))
		rotVec = makeUnitvector(rotationVec);
	else
		rotVec = vec3(1.0,0.0,0.0);
	
	//column ; row
	modelRot[0][0] = cRad + rotVec.x * rotVec.x * (1 - cRad);		 
	modelRot[1][0] = rotVec.x * rotVec.y * (1 - cRad) - rotVec.z * sRad; 		
	modelRot[2][0] = rotVec.x * rotVec.z * (1 - cRad) + rotVec.y * sRad; 		
	modelRot[3][0] = 0;

	modelRot[0][1] = rotVec.y * rotVec.x * (1 - cRad) + rotVec.z * sRad;		 
	modelRot[1][1] = cRad + rotVec.y * rotVec.y * (1 - cRad); 		
	modelRot[2][1] = rotVec.y * rotVec.z * (1 - cRad) - rotVec.x * sRad; 		
	modelRot[3][1] = 0;
	
	modelRot[0][2] = rotVec.z * rotVec.x * (1 - cRad) - rotVec.y * sRad;		 
	modelRot[1][2] = rotVec.z * rotVec.y * (1 - cRad) + rotVec.x * sRad; 		
	modelRot[2][2] = cRad + rotVec.z * rotVec.z * (1 - cRad); 		
	modelRot[3][2] = 0;

	modelRot[0][3] = 0;		 
	modelRot[1][3] = 0; 		
	modelRot[2][3] = 0; 		
	modelRot[3][3] = 1;
	
	//---------------transformation Model--------------------
	modelTrans[3][0] = translationVec.x;
	modelTrans[3][1] = translationVec.y;
	modelTrans[3][2] = translationVec.z;
	modelTrans[3][3] = 1;
	
	//----------------Arrow Transformation--------------------
	if(isArrow)
	{
		mat4 arrowModel;
		if(gl_InstanceID == 0)
		{
			arrowModel = matArrowX;
		}
		else if(gl_InstanceID == 1)
		{
			arrowModel = matArrowY;
		}
		else if(gl_InstanceID == 2)
		{
			arrowModel = matArrowZ;
		}
		resultModel = model * modelTrans * modelRot * modelScale * arrowModel ;
	}
	else
	{
		resultModel = model * modelTrans * modelRot * modelScale ;
	}
	
	gl_Position = projection * view * resultModel * vec4(arrow, 1.0);	
	oColor = color;
}