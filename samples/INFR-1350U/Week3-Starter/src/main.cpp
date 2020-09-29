#include <Logging.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <json.hpp>
#include <fstream>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArrayObject.h"
#include "Shader.h"

#define LOG_GL_NOTIFICATIONS

/*
	Handles debug messages from OpenGL
	https://www.khronos.org/opengl/wiki/Debug_Output#Message_Components
	@param source    Which part of OpenGL dispatched the message
	@param type      The type of message (ex: error, performance issues, deprecated behavior)
	@param id        The ID of the error or message (to distinguish between different types of errors, like nullref or index out of range)
	@param severity  The severity of the message (from High to Notification)
	@param length    The length of the message
	@param message   The human readable message from OpenGL
	@param userParam The pointer we set with glDebugMessageCallback (should be the game pointer)
*/
void GlDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::string sourceTxt;
	switch (source) {
	case GL_DEBUG_SOURCE_API: sourceTxt = "DEBUG"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceTxt = "WINDOW"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceTxt = "SHADER"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: sourceTxt = "THIRD PARTY"; break;
	case GL_DEBUG_SOURCE_APPLICATION: sourceTxt = "APP"; break;
	case GL_DEBUG_SOURCE_OTHER: default: sourceTxt = "OTHER"; break;
	}
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:          LOG_INFO("[{}] {}", sourceTxt, message); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       LOG_WARN("[{}] {}", sourceTxt, message); break;
	case GL_DEBUG_SEVERITY_HIGH:         LOG_ERROR("[{}] {}", sourceTxt, message); break;
		#ifdef LOG_GL_NOTIFICATIONS
	case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_INFO("[{}] {}", sourceTxt, message); break;
		#endif
	default: break;
	}
}

GLFWwindow* window;
void GlfwWindowResizedCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool initGLFW() {
	if (glfwInit() == GLFW_FALSE) {
		LOG_ERROR("Failed to initialize GLFW");
		return false;
	}

	//Create a new GLFW window
	window = glfwCreateWindow(800, 800, "Samuel Canonaco - 100742837", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, GlfwWindowResizedCallback);

	return true;
}

bool initGLAD() {
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		LOG_ERROR("Failed to initialize Glad");
		return false;
	}
}


GLuint shader_program;


int main() {
	Logger::Init(); // We'll borrow the logger from the toolkit, but we need to initialize it

	//Initialize GLFW
	if (!initGLFW())
		return 1;

	//Initialize GLAD
	if (!initGLAD())
		return 1;

	// Let OpenGL know that we want debug output, and route it to our handler function
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GlDebugMessage, nullptr);

	static const GLfloat points[] = {
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f
	};

	static const GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
	/*
	//VBO - Vertex buffer object
	GLuint pos_vbo = 0;
	glGenBuffers(1, &pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLuint color_vbo = 1;
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	*/
	
	VertexBuffer* posVbo = new VertexBuffer();
	posVbo->LoadData(points, 9);
	VertexBuffer* color_vbo = new VertexBuffer();
	color_vbo->LoadData(colors, 9);
	//						index, size, type, normalize?, stride, pointer
	/*
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);//pos
	glEnableVertexAttribArray(1);//colors
	*/
	VertexArrayObject* vao = new VertexArrayObject();
	vao->AddVertexBuffer(posVbo, {
	{ 0, 3, GL_FLOAT, false, 0, NULL }
		});
	vao->AddVertexBuffer(color_vbo, {
	{ 1, 3, GL_FLOAT, false, 0, NULL }
		});
	static const float interleaved[] = {
	//   X      Y     Z     R     G      B
		 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, 0.5f, 0.5f, 0.3f, 0.2f, 0.5f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
		 -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f
	};

	VertexBuffer* interleaved_vbo = new VertexBuffer();
	interleaved_vbo->LoadData(interleaved, 6 * 4);
	static const uint16_t indices[] = {
		0, 1, 2
	};

	//added indices
	static const uint16_t indices2[] = {
		0,2,3
	};
	IndexBuffer* interleaved_ibo = new IndexBuffer();
	interleaved_ibo->LoadData(indices, 3);

	//second ibo by me
	IndexBuffer* ibo_2 = new IndexBuffer();
	ibo_2->LoadData(indices2, 3);


	size_t stride = sizeof(float) * 6;
	VertexArrayObject* vao2 = new VertexArrayObject();
	vao2->AddVertexBuffer(interleaved_vbo, {
	BufferAttribute(0, 3, GL_FLOAT, false, stride, 0),
	BufferAttribute(1, 3, GL_FLOAT, false, stride, sizeof(float) * 3),
		});
	vao2->SetIndexBuffer(interleaved_ibo);

	//third vao object my me
	VertexArrayObject* vao3 = new VertexArrayObject();
	vao3->AddVertexBuffer(interleaved_vbo, {
	BufferAttribute(0, 3, GL_FLOAT, false, stride, 0),
	BufferAttribute(1, 3, GL_FLOAT, false, stride, sizeof(float) * 3),
		});
	vao3->SetIndexBuffer(ibo_2);;
	// Load our shaders

	/*
	if (!loadShaders())
		return 1;
	*/
	Shader* shader = new Shader();
	shader->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	shader->LoadShaderPartFromFile("shaders/frag_shader.glsl", GL_FRAGMENT_SHADER);
	shader->Link();

	//second shader object
	Shader* shader2 = new Shader();
	shader2->LoadShaderPartFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	shader2->LoadShaderPartFromFile("shaders/frag_shader2.glsl", GL_FRAGMENT_SHADER);
	shader2->Link();

	// GL states
	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Our high-precision timer
	double lastFrame = glfwGetTime();

	///// Game loop /////
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Calculate the time since our last frame (dt)
		double thisFrame = glfwGetTime();
		float dt = static_cast<float>(thisFrame - lastFrame);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/*
		glUseProgram(shader_program);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		*/
		shader->Bind();
		//vao->Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		vao2->Bind();
		glDrawElements(GL_TRIANGLES, interleaved_ibo->GetElementCount(), interleaved_ibo->GetElementType(), nullptr);
		//vao->UnBind();
		vao2->UnBind();
		shader->UnBind();

		// second set of vertices rendered with second shader
		shader2->Bind();
		vao3->Bind();
		glDrawElements(GL_TRIANGLES, ibo_2->GetElementCount(), ibo_2->GetElementType(), nullptr);
		vao3->UnBind();
		shader2->UnBind();


		glfwSwapBuffers(window);
	}
	delete shader;
	delete vao;
	delete posVbo;
	delete color_vbo;
	// Clean up the toolkit logger so we don't leak memory
	Logger::Uninitialize();
	return 0;
}