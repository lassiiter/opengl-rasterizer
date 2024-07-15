#version 330 core

//Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Coordinate
layout (location = 2) in vec2 aTex;

// Outputs the color for the Fragment Shader
out vec3 color;

// Texture Coordinates
out vec2 texCoord;

uniform mat4 camMatrix;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(aPos,1.0f);
	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	texCoord = aTex;
}