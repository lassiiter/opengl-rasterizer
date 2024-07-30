#version 330 core

//Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Coordinate
layout (location = 1) in vec2 aTex;

// Texture Coordinates
out vec2 texCoord;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	texCoord = vec2(aTex.x, aTex.y);
}