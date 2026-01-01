#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* pWindow, int width, int height);
void processInput(GLFWwindow* pWindow);
void prepareShader(const unsigned int shaderProgram);
void prepareVertexShader(const unsigned int vertexShader);
void prepareFragmentShader(const unsigned int fragmentShader);
void prepareVertex(const unsigned int VAO, const unsigned int VBO, const unsigned int EBO);
void drawTriangle(const unsigned int shaderProgram, const unsigned int VAO);

static const unsigned int SCREEN_WIDTH = 800;
static const unsigned int SCREEN_HEIGHT = 600;
static const unsigned int LOG_LENGTH = 512;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* pWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (nullptr == pWindow)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(pWindow);
	glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		glfwTerminate();
		return -1;
	}

	unsigned int shaderProgram = glCreateProgram();
	prepareShader(shaderProgram);

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	prepareVertex(VAO, VBO, EBO);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Wireframe 적용

	while (!glfwWindowShouldClose(pWindow))
	{
		processInput(pWindow);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		drawTriangle(shaderProgram, VAO);

		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* pWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* pWindow)
{
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(pWindow, true);
	}
}

void prepareShader(const unsigned int shaderProgram)
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	prepareVertexShader(vertexShader);
	glAttachShader(shaderProgram, vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	prepareFragmentShader(fragmentShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	int success = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char log[LOG_LENGTH];
		glGetProgramInfoLog(shaderProgram, LOG_LENGTH, NULL, log);
		std::cout << "shader link fail! " << log << '\n';
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void prepareVertexShader(const unsigned int vertexShader)
{
	static const char* vertexShaderSource = R"(
		#version 330 core

		layout (location = 0) in vec3 aPos;

		void main()
		{
			gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
		})";

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char log[LOG_LENGTH];
		glGetShaderInfoLog(vertexShader, LOG_LENGTH, NULL, log);
		std::cout << "vertext shader compile fail! " << log << '\n';
	}
}

void prepareFragmentShader(const unsigned int fragmentShader)
{
	static const char* fragmentShaderSource = R"(
		#version 330 core

		out vec4 FragColor;

		void main()
		{
			FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		})";

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int success = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char log[LOG_LENGTH];
		glGetShaderInfoLog(fragmentShader, LOG_LENGTH, NULL, log);
		std::cout << "fragment shader compile fail! " << log << '\n';
	}
}

void prepareVertex(const unsigned int VAO, const unsigned int VBO, const unsigned int EBO)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	const float vertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	const unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void drawTriangle(const unsigned int shaderProgram, const unsigned int VAO)
{
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
