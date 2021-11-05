// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

// Include GLEW. 
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM.
#include <glm/glm.hpp>
using namespace glm;

// #include <shader.hpp>


GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders (Vertex / Fragment)
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


int main()
{
	// Initialise GLFW
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	// Open a window 
	GLFWwindow* window; 
	window = glfwCreateWindow(800, 800, u8"2Δ Σχήματα", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
	glfwMakeContextCurrent(window); 
	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Purple background
	glClearColor(0.5f, 0.0f, 0.5f, 0.0f);
	

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Call shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	// run the first triangles all the time on background and check if "n" key is pressed 
	while (glfwGetKey(window, GLFW_KEY_C) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_N) != GLFW_PRESS)
	{
		// An array of 3 vectors which represents 3 vertices
		static const GLfloat g_vertex_buffer_data[] = {

			//1st 
			0.0f, -1.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			1.0f,  -1.0f, 0.0f,

			//2nd 
			1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f,

			//3rd 
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,

			//4th 
			-1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			-1.0f,  -1.0f, 0.0f,

		};



		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// use shader from the files 
			glUseProgram(programID);

			// Clear the screen.
			//glClear(GL_COLOR_BUFFER_BIT);

			// 1st attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size 
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			// 4 Triangles
			glDrawArrays(GL_TRIANGLES, 0, 12);

			// 12 vertices total -> 4 triangles
			glDisableVertexAttribArray(0);


			// Swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();

		

		// Check if the "n" key was pressed to exit or the window was closed or "c" key was hold!
		//while (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS &&
			//glfwWindowShouldClose(window) == 0);

		//if "c" button is pressed, change the triangles as long as it's pressed
		while (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		{
			// New array for the different triangles when "c" is pressed
			static const GLfloat g_vertex_buffer_data[] = {

				//1st new
				1.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				-1.0f,  1.0f, 0.0f,

				//2nd new
				1.0f, -1.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				-1.0f,  -1.0f, 0.0f,

				//3rd new
				-1.0f, 0.5f, 0.0f,
				0.0f, 0.0f, 0.0f,
				-1.0f,  -0.5f, 0.0f,

				//4th new
				1.0f, 0.5f, 0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f,  -0.5f, 0.0f,

			};



			GLuint vertexbuffer;
			glGenBuffers(1, &vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

			do {
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// use shader from the files 
				glUseProgram(programID);

				//glClear(GL_COLOR_BUFFER_BIT);

				// 1st attribute buffer : vertices
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
				glVertexAttribPointer(
					0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
					3,                  // size 
					GL_FLOAT,           // type
					GL_FALSE,           // normalized?
					0,                  // stride
					(void*)0            // array buffer offset
				);
				// 4 Triangles 
				glDrawArrays(GL_TRIANGLES, 0, 12);

				// 12 vertices total -> 4 triangle
				glDisableVertexAttribArray(0);


				// Swap buffers
				glfwSwapBuffers(window);
				glfwPollEvents();

			}

			// Check if the "n" key was pressed to shut down or the window was closed or "c" is released
			while (glfwGetKey(window, GLFW_KEY_C) != GLFW_PRESS  &&
				glfwGetKey(window, GLFW_KEY_N) != GLFW_PRESS &&
				glfwWindowShouldClose(window) == 0);
		}
	}



	// Cleanup VBO
	//glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
