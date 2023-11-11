#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Vertex Shader 
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader 
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main()
{
	// Intialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	// in this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell GLFW what profile we are using
	// Core profile only includes modern functions
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Vertice coords
	GLfloat verticies[] =
	{
		-0.5f,-0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f,-0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f,0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
	};

	// Create window
	GLFWwindow* window = glfwCreateWindow(800, 800, "renderer", NULL, NULL);
	
	// Error checking
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glClearColor(.1f, .1f, .1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	// Specify the viewport of OpenGL in the window
	glViewport(0,0,800,800);

	// Create Vertex Shader Object and get ref
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the obj
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get ref
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the obj
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile Fragment Shader into machine code
	glCompileShader(fragmentShader);

	// Create shader program object
	GLuint shaderProgram = glCreateProgram();
	// Attach shaders to program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Link all shaders together into shader program
	glLinkProgram(shaderProgram);

	//Delete used objs
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// Create reference containers
	GLuint VAO, VBO;

	// Generate VAO and VBO with only 1 obj
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Make the VAO the current Vertex Array
	glBindVertexArray(VAO);

	// Bind the VBO specifying its a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	// Configure the Vertex Attrib so the OpenGL knows how to read VBO
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0 );
	// Enable the Vertex Attribs so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	// Bind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while(!glfwWindowShouldClose(window))
	{

		glClearColor(.1f, .1f, .1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);

		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete after used
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);


	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}