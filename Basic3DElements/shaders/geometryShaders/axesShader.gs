#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 5) out;

in DATA {
	vec4 origin;
	vec3 color;
} data_in[];


out vec3 fColor;

void main()
{    
    fColor = data_in[0].color; 
    gl_Position = data_in[0].origin;  
    EmitVertex();   
	
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    EndPrimitive();
}