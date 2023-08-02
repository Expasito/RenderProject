#include "Render.h"



GLFWwindow* Render::window = NULL;
unsigned int Render::program1 = -1;
unsigned int Render::program2 = -1;
std::vector<Render::Object*> Render::objects;
glm::mat4 Render::model = glm::mat4(1.0f);
glm::mat4 Render::view = glm::mat4(1.0f);
glm::mat4 Render::projection = glm::mat4(1.0f);

unsigned long long Render::instanceCounter = 0;

float Render::dt;
float Render::lastFrame = 0;
bool Render::keepWindow = true;
unsigned int Render::VAO = -1;
Camera Render::camera(0, 0, -10);

bool Render::left = false, Render::right = false, Render::down = false, Render::up = false, Render::forward = false, Render::backward = false;
bool Render::leftMouseButton = false, Render::rightMouseButton = false, Render::middleMouseButton = false;



Render::Model::Model(std::vector<glm::vec3> verts, std::vector<unsigned int> inds) {
	vertices = verts;
	indices = inds;
	glGenBuffers(1, &positions);
	glBindBuffer(GL_ARRAY_BUFFER, positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
}



void Render::addModel(const char* filepath, std::string name, int hashtablesize, int dynamicarraysize) {
	Render::Model m = loadModel(filepath);
	objects.push_back(new Render::Object(name, m.positions, m.ebo, m.indices.size()-3, new FillerArray(hashtablesize, dynamicarraysize)));
}



// the returned value is the unique id for the number
long Render::addInstance(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, glm::vec3 color) {
	for (Render::Object* o : Render::objects) {
		if (o->name.compare(name)==0) {
			return o->insts->add(pos, rot, scal, color);

		}
		
	}
}

void Render::editInstance(std::string name, unsigned long long id, glm::vec3 pos, glm::bvec3 rot, glm::vec3 scal, glm::vec3 color) {

}

//void Render::removeInstances(std::string name) {
//	for (object o : Render::objects) {
//		if (o.name.compare(name) == 0) {
//			o.translations->clear();
//			o.rotations->clear();
//			o.scalations->clear();
//			o.colors->clear();
//		}
//	}
//}
//
//void Render::removeAllInstances() {
//	for (object o : Render::objects) {
//		o.translations->clear();
//		o.rotations->clear();
//		o.scalations->clear();
//		o.colors->clear();
//	}
//}

void Render::prepBuffers() {
	// create and bind the vao
	glGenVertexArrays(1, &Render::VAO);
	glBindVertexArray(Render::VAO);


	// set the divisors for each vertex attribute
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);


	// enable each vertex attribute
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

}


//TODO: rewrite so we set the buffer data and vertex attributes at hte same time for speed to not bind buffers as often
void Render::draw() {

	float milis;

	for (Render::Object* o : Render::objects) {
		int elements = o->insts->da->elements;
#ifdef DEBUG
		std::cout << "Rendering: " << o->name << "\n";
		std::cout << "  Elements: " << elements << "\n";
#endif
		//int elements = o.insts->table->elements;
		//std::cout << "Elements: " << elements <<" Size: " << o.insts->table->size  << " load: " << o.insts->table->load << "\n";
		if (elements <= 0) {
			continue;
		}

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		

		// bind the position buffer and send the vertices to gpu
		glBindBuffer(GL_ARRAY_BUFFER, o->positions);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

#ifdef DEBUG
		std::chrono::steady_clock::time_point posBuff = std::chrono::steady_clock::now();
		milis = (posBuff - begin).count() / 1000000.0;
		std::cout << "       Position Buffer: " << milis << "[ms]\n";
		posBuff = std::chrono::steady_clock::now();
#endif

		// load in the buffer of all instances
		glBindBuffer(GL_ARRAY_BUFFER, o->insts->buffer);

#ifdef DEBUG
		std::chrono::steady_clock::time_point instBuff = std::chrono::steady_clock::now();
		milis = (instBuff - posBuff).count() / 1000000.0;
		std::cout << "       Instance Buffer: " << milis << "[ms]\n";
		instBuff = std::chrono::steady_clock::now();
#endif

		
		// this is translation
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int)));
		
		// this is rotation
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(glm::vec3)+sizeof(int)));
		
		// this is for scalation
		 glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int) + 2 * sizeof(glm::vec3)));
		 
		 // this is for color
		 glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int) + 3 * sizeof(glm::vec3)));
		 
#ifdef DEBUG
		 std::chrono::steady_clock::time_point vertexattr = std::chrono::steady_clock::now();
		 milis = (vertexattr - instBuff).count() / 1000000.0;
		 std::cout << "         Vertex Attrib: " << milis << "[ms]\n";
		 vertexattr = std::chrono::steady_clock::now();
#endif


		 // give the ebo to the gpu
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o->ebo);
		
#ifdef DEBUG
		std::chrono::steady_clock::time_point ebo = std::chrono::steady_clock::now();
		milis = (ebo - vertexattr).count() / 1000000.0;
		std::cout << "              EBO bind: " << milis << "[ms]\n";
		ebo = std::chrono::steady_clock::now();
#endif

		// finally, send the draw command to the gpu
		glDrawElementsInstanced(GL_TRIANGLES, o->eboSize, GL_UNSIGNED_INT, 0, elements);

#ifdef DEBUG
		std::chrono::steady_clock::time_point drawelements = std::chrono::steady_clock::now();
		milis = (drawelements - ebo).count() / 1000000.0;
		std::cout << "         Draw Elements: " << milis << "[ms]\n";
		std::cout << "         Draw Elements: " << milis << "[ms]\n";
#endif


	}
}

void Render::renderAll() {
	float milis;
	// this is the start time 
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	Render::keepWindow = !glfwWindowShouldClose(Render::window);
	float currentFrame = glfwGetTime();
	Render::dt = currentFrame-Render::lastFrame;
	Render::lastFrame = currentFrame;
	Render::camera.speed = Render::camera.baseSpeed * Render::dt;
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(glGetUniformLocation(Render::program1, "model"), 1, GL_FALSE, glm::value_ptr(Render::model));
	glUniformMatrix4fv(glGetUniformLocation(Render::program1, "view"), 1, GL_FALSE, glm::value_ptr(Render::view));
	glUniformMatrix4fv(glGetUniformLocation(Render::program1, "projection"), 1, GL_FALSE, glm::value_ptr(Render::projection));
	glUniform3fv(glGetUniformLocation(Render::program1, "camPos"), 1, glm::value_ptr(Render::camera.cameraPos));
	glUniform3fv(glGetUniformLocation(Render::program1, "camFront"), 1, glm::value_ptr(Render::camera.cameraFront));

	Render::camera.translate(Render::left, Render::right, Render::up, Render::down, Render::forward, Render::backward);
	Render::view = glm::lookAt(Render::camera.cameraPos, Render::camera.cameraPos + Render::camera.cameraFront, Render::camera.cameraUp);
	Render::projection = glm::perspective(glm::radians(Render::camera.fov), (float)(800.0 / 800.0), .01f, 1000.0f);

# ifdef DEBUG
	std::chrono::steady_clock::time_point uniforms = std::chrono::steady_clock::now();
	milis = (uniforms - begin).count() / 1000000.0;
	std::cout << "   Uniforms: " << milis << "[ms]\n";
#endif

	Render::draw();

#ifdef DEBUG
	std::chrono::steady_clock::time_point draw = std::chrono::steady_clock::now();
	milis = (draw - uniforms).count() / 1000000.0;
	std::cout << "   Draw: " << milis << "[ms]\n";
#endif

	glfwSwapBuffers(Render::window);

#ifdef DEBUG
	std::chrono::steady_clock::time_point swap = std::chrono::steady_clock::now();
	milis = (swap - draw).count() / 1000000.0;
	std::cout << "   Swap Buffers: " << milis << "[ms]\n";
#endif
	
	glfwPollEvents();

#ifdef DEBUG
	std::chrono::steady_clock::time_point poll = std::chrono::steady_clock::now();
	milis = (poll - swap).count() / 1000000.0;
	std::cout << "   Poll Events: " << milis << "[ms]\n";
#endif
}


char* Render::loadShader(const char* filepath) {
	FILE* fragment = fopen(filepath, "r");
	int size = 100;
	int index = 0;
	char* stream = (char*)malloc(sizeof(char) * size);
	while (!feof(fragment)) {
		char in;
		fscanf(fragment, "%c", &in);
		stream[index] = in;
		index++;
		if (index == size) {
			stream = (char*)realloc(stream, size + size);
			size = size + size;
		}
	}

	stream[index - 1] = '\0';
	fclose(fragment);


	return stream;
}

void Render::shaderBuildStatus(unsigned int shader, int result) {
	std::cout << "Result" << result << "\n";
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)(alloca(length * (sizeof(char))));
		glGetShaderInfoLog(shader, length, &length, message);
		std::cout << "Failed to compile shader--\n";
		std::cout << message << "\n";
		glDeleteShader(shader);
		return;

	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
bool wireframe = false;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	Render::left = glfwGetKey(window, GLFW_KEY_A);
	Render::right = glfwGetKey(window, GLFW_KEY_D);
	Render::forward = glfwGetKey(window, GLFW_KEY_W);
	Render::backward = glfwGetKey(window, GLFW_KEY_S);
	Render::up = glfwGetKey(window, GLFW_KEY_SPACE);
	Render::down = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		wireframe = !wireframe;
		
	}
	if (wireframe == true) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


}

void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		Render::leftMouseButton = action;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		Render::middleMouseButton = action;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		Render::rightMouseButton = action;
	}

}

void mouseCallBack(GLFWwindow* window, double xpos, double ypos) {
	Render::camera.rotate(xpos, ypos, Render::leftMouseButton);
}

void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
	Render::camera.zoom(xoffset, yoffset);

}

void Render::init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	Render::window = glfwCreateWindow(800, 800, "Game", NULL, NULL);
	if (Render::window == NULL) {
		printf("Failed to create window\n");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(Render::window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initalize GLAD\n");
		return;
	}

	glViewport(0, 0, 800, 800);

	glfwSetFramebufferSizeCallback(Render::window, framebuffer_size_callback);

	glfwSetKeyCallback(Render::window, key_callback);
	glfwSetCursorPosCallback(Render::window, mouseCallBack);
	glfwSetMouseButtonCallback(Render::window, mouseButtonCallBack);
	glfwSetScrollCallback(Render::window, scrollCallBack);

	compileShader("shaders/fragment.shader", "shaders/vertex.shader", &Render::program1);
	glUseProgram(Render::program1);
	
	Render::prepBuffers();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

}

void Render::exit() {
	glfwTerminate();
	//for (object o : Render::objects) {
	//	//free(o.insts->table);
	//}

	glfwDestroyWindow(Render::window);
}

void Render::compileShader(const char* fragment, const char* vertex, unsigned int* program) {
	char* fragmentShaderSource = loadShader(fragment);
	char* vertexShaderSource = loadShader(vertex);

	unsigned int vertexShader;
	unsigned int fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	int result;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	shaderBuildStatus(vertexShader, result);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	shaderBuildStatus(fragmentShader, result);

	*program = glCreateProgram();
	glAttachShader(*program, vertexShader);
	glAttachShader(*program, fragmentShader);

	//// add geometry shader here
	//char* geometryShaderSource = loadShader("shaders/geometry.shader");
	//unsigned int geometryShader;
	//geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	//glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
	//glCompileShader(geometryShader);

	//glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &result);
	//shaderBuildStatus(geometryShader, result);
	//glAttachShader(*program, geometryShader);

	glLinkProgram(*program);

	glUseProgram(*program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	free(vertexShaderSource);
	free(fragmentShaderSource);
}


std::vector<std::string> Render::strsplit(std::string str, char dem) {
	std::vector<std::string> strs;
	//std::vector<std::string>* strs = new std::vector<std::string>();
	while (str.size() != 0) {
		//std::cout << str << "\n";

		size_t pos = str.find(dem);
		//std::cout << "pos: " << pos << "\n";
		// dem does exist
		if (pos != std::string::npos) {
			std::string temp = str.substr(0, pos);
			strs.push_back(temp);
			str.erase(0, pos + 1);
		}
		else {
			std::string temp = str;
			strs.push_back(temp);
			break;
		}
	}
	return strs;
}



Render::Model Render::loadModel(const char* path) {
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Failed to open\n");
	}
	while (!feof(file)) {

		char stuff[1000];
		fgets(stuff, 1000, file);
		std::string buff = stuff;
		// is vertex data
		if (buff[0] == 'v' && buff[1] == ' ') {
			std::vector<std::string> sub = strsplit(buff.substr(2, buff.size() - 1), ' ');
			float f1 = stof(sub[0]);
			float f2 = stof(sub[1]);
			float f3 = stof(sub[2]);
			vertices.push_back({f1,f2,f3});
		}
		// is indice data
		if (buff[0] == 'f' && buff[1] == ' ') {
			std::vector<std::string> sub = strsplit(buff.substr(2, buff.size() - 1), ' ');
			float i1, i2, i3;
			std::vector<std::string> subi1 = strsplit(sub[0], '/');
			i1 = stoi(subi1[0]);
			std::vector<std::string> subi2 = strsplit(sub[1], '/');
			i2 = stoi(subi2[0]);
			std::vector<std::string> subi3 = strsplit(sub[2], '/');
			i3 = stoi(subi3[0]);
			indices.push_back(i1 - 1);
			indices.push_back(i2 - 1);
			indices.push_back(i3 - 1);

		}

		char type;

	}
	fclose(file);
	Render::Model ret(vertices, indices);
	return ret;
}