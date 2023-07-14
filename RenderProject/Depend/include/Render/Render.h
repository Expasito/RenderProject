#pragma once
#include "Headers.h"
#include "Camera.h"


class Render
{
public:
	class Model {
	public:
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indices;
		Model(std::vector<glm::vec3> verts, std::vector<unsigned int> inds);
		Model();
	private:

	};
	struct object {
		Model m;
		std::string name;
		std::vector<glm::vec3>* translations;
		std::vector<glm::vec3>* rotations;
		std::vector<glm::vec3>* scalations;
		std::vector<glm::vec3>* colors;
		
	};
	static std::vector<object> objects;
	static GLFWwindow* window;

	static unsigned int program1, program2;
	static bool left, right, down, up, forward, backward;
	static bool leftMouseButton, rightMouseButton, middleMouseButton;
	static Camera camera;
	static glm::mat4 model;
	static glm::mat4 view;
	static glm::mat4 projection;
	static float dt;
	static float lastFrame;
	static bool keepWindow;
	static void init();
	static void exit();
	static void addModel(const char* path, std::string name);
	static int addInstance(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,glm::vec3 colors);
	static void removeInstances(std::string name);
	static void removeAllInstances();
	static void draw();
	static void renderAll();
	
	
private:
	static unsigned int VAO, positionBuffer, translationBuffer, rotationBuffer, scalationBuffer, colorBuffer, EBO;
	static char* loadShader(const char* filepath);
	static void shaderBuildStatus(unsigned int shader, int result);
	static void compileShader(const char* filepath1, const char* filepath2, unsigned int* program);
	static std::vector<std::string> strsplit(std::string str, char dem);
	static Render::Model loadModel(const char* path);
	static void prepBuffers();
};

