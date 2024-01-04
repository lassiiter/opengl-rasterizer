#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

// Vertices coordinates
GLfloat vertices[] = {
	// positions         // colors
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};

float firstTriangle[] = {
	//indicies			   //color
	   -0.9f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,  // left 
	   -0.0f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,  // right
	   -0.45f, 0.5f, 0.0f, 1.0f,0.0f,0.0f,  // top 
};
float secondTriangle[] = {
	0.0f, -0.5f, 0.0f,  // left
	0.9f, -0.5f, 0.0f,  // right
	0.45f, 0.5f, 0.0f   // top 
};

// Indices for vertices order
GLuint indices[] =
{
	0,1,2,
	1,2,3
};

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "OpenGL"
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Links VBO to VAO
	VAO1.LinkVBO(VBO1, 0);

	VAO VAO2;
	VAO2.Bind();
	VBO VBO2(secondTriangle, sizeof(secondTriangle));
	VAO2.LinkVBO(VBO2, 0);

	// Generates Element Buffer Object and links it to indices
	// EBO EBO1(indices, sizeof(indices));

	// Unbind all to prevent accidentally modifying them

	//VAO1.Unbind();
	//VAO2.Unbind();

	//VBO1.Unbind();
	//VBO2.Unbind();

	// EBO1.Unbind();
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		shaderProgram.setFloat("testfloat", 1.0f);
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		// Draw primitives, number of indices, datatype of indices, index of indices
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}
	// Delete all the objects we've created
	VAO1.Delete();
	VAO2.Delete();

	VBO1.Delete();
	VBO2.Delete();

	//EBO1.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}