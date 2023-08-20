
#include <Render/Render.h>
#include <chrono>
#include <Soil/SOIL.h>


/*
*  Use WASD to move, mouse to look and B to change wireframe mode
*/























// For when choosing if we want to use diffuse/specular maps or use vec3s, have either option and then calculate yes or no in the vertex shader
// so the fragment has less work to do. So when creating an object, have it pas in which textures to use/ which values for specular and diffuse.




























#include <Render/utils/FillerArray.h>

float test_ = 0.0;




// I want to test passing function pointers for a generic hashtable hashing algorithm

/*
* The hash function must take in two parameters. The first is of type T, the second is a size parameter
*/
template <typename T>
class HashTable {
public:
	int size, elements;
	int (*hash)(T, int);
	T* arr;

	HashTable(int size_, int (*hash_)(T,int)) {
		size = size_;
		elements = 0;
		arr = new T(size_);
		hash = hash_;

	};

	void add(T obj) {
		int index = hash(obj, size);
		arr[index] = obj;
		elements++;
	}

	void see() {
		std::cout << "Size: " << size << "\n";
		std::cout << "Elements: " << elements << "\n";
		for (int i = 0; i < size; i++) {
			std::cout << arr[i] << "\n";
		}
	}
	~HashTable() {
		delete arr;

	}
private:

};

int hash(int val, int size) {
	return val % size;
}

int main() {

	printf("%c", 178);

	int width__, height__, channels__;

	unsigned char* data_ = SOIL_load_image("textures/Texture.png", &width__, &height__, &channels__, 0);

	printf("%p\n", data_);

	std::cout << channels__ << "\n";
	std::cout << width__ << " " << height__ << "\n";




	//width__ = 10;
	//height__ = 5;
	//channels__ = 4;

	//// this will be a temporary array. If the value is 0, then it will be black and transparent. If 1, then white and not
	//unsigned char data__[] = {
	//	0,1,1,0,0,1,1,1,0,0,
	//	1,0,0,1,0,1,0,0,1,0,
	//	1,1,1,1,0,1,1,1,0,0,
	//	1,0,0,1,0,1,0,0,1,0,
	//	1,0,0,1,0,1,1,1,0,0
	//};

	//unsigned char* data_ = (unsigned char*)malloc(sizeof(unsigned char) * channels__ * width__ * height__);
	//int c = 0;
	//for (int i = 0; i < width__*height__; i++) {
	//	if (data__[i] == 1) {
	//		data_[c++] = 255;
	//		data_[c++] = 255;
	//		data_[c++] = 255;
	//		data_[c++] = 255;
	//	}
	//	else {
	//		data_[c++] = 0;
	//		data_[c++] = 0;
	//		data_[c++] = 0;
	//		data_[c++] = 0;
	//	}
	//}


	

	//exit(1);


	//exit(1);


	//stbi_image_free(data_);



	srand(time(0));
	Render::init(800,800,false);
	Render::camera.baseSpeed = 10;











	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




	unsigned int ebo_[] = {
		// front face
		0,2,1,
		2,3,1,

		// back face
		4,5,6,
		6,5,7,

		// left face
		2,6,7,
		7,3,2,

		// right face
		0,5,4,
		5,0,1,

		//// top face
		1,3,5,
		5,3,7,

		//// bottom face
		0,4,2,
		4,6,2,
		


	};

	std::vector<Render::vertex> positions;
	positions.push_back({ glm::vec3(-1,-1,1),glm::vec3(1,1,1),glm::vec3(-1,-1,1),glm::vec2(0,0) }); // 0 front bottom left
	positions.push_back({ glm::vec3(-1,1,1),glm::vec3(1,1,1),glm::vec3(-1,1,1),glm::vec2(0,0) });   // 1 front top left
	positions.push_back({ glm::vec3(1,-1,1),glm::vec3(1,1,1),glm::vec3(1,-1,1),glm::vec2(1,0) });   // 2 front bottom right
	positions.push_back({ glm::vec3(1,1,1),glm::vec3(1,1,1),glm::vec3(1,1,1),glm::vec2(1,0) });     // 3 front top right
	positions.push_back({ glm::vec3(-1,-1,-1),glm::vec3(1,1,1),glm::vec3(-1,-1,-1),glm::vec2(0,0) }); // 4 back bottom left
	positions.push_back({ glm::vec3(-1,1,-1),glm::vec3(1,1,1),glm::vec3(-1,1,-1),glm::vec2(0,1) });   // 5 back top left
	positions.push_back({ glm::vec3(1,-1,-1),glm::vec3(1,1,1),glm::vec3(1,-1,-1),glm::vec2(1,0) });   // 6 back bottom right
	positions.push_back({ glm::vec3(1,1,-1),glm::vec3(1,1,1),glm::vec3(1,1,-1),glm::vec2(1,1) });     // 7 back top right


	std::vector<unsigned int> ebo;
	for (int i = 0; i < sizeof(ebo_)/sizeof(unsigned int); i++) {
		std::cout << i << "\n";
		ebo.push_back(ebo_[i]);
	}

	unsigned int test_, test2_;
	glGenBuffers(1, &test_);
	glBindBuffer(GL_ARRAY_BUFFER, test_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Render::vertex)*positions.size(), &positions[0], GL_STATIC_DRAW);

	glGenBuffers(1, &test2_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, test2_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ebo.size(), &ebo[0], GL_STATIC_DRAW);


	Render::objects.push_back(new Render::Object("Test", "..", test_, test2_, ebo.size(), new FillerArray(100, 100)));

	//Render::addModel("assets/sphere.obj", "Cube",100,100);
	//Render::addModel("assets/sphere.obj", "Cube");
	//Render::addModel("assets/monkey.obj", "Monkey");

	//Render::addModel("assets/room.obj", "Room", 10000, 10000);

	//Render::addInstance("Room", { 20,0,0 }, { 0,0,0 }, { 1,1,1 }, { .5,.5,.5 });

	Render::addModel("assets/Cube3.obj", "CUBE", 100, 100);
	Render::addModel("assets/sphere.obj", "Sphere", 100, 100);

	//Render::addInstance("CUBE", { 0,-10,0 }, { 0,0,0 }, { 10,1,10 }, {1,1,1});
	Render::addInstance("Test", { 0,-2,0 }, { 0,0,0 }, { 1,1,1 }, {1,1,1});
	Render::addInstance("Test", { 0,-10,0 }, { 0,0,0 }, { 20,1,20 }, { 1,1,1 });

	//Render::addInstance("CUBE", { -10,-10,0 }, { 0,0,0 }, { 1,1,1 }, { 1,1,1 });

	//long key = Render::addInstance("Sphere", {0,1,0}, {0,0,0}, {1,1,1}, {1,1,1});


	// this will be 5 by 5

	glUseProgram(Render::renderShader);
	
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);




	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width__, height__, 0, GL_RGB, GL_UNSIGNED_BYTE, data_);



	glGenerateMipmap(GL_TEXTURE_2D);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	char data2_[] = {
		255,255,255,255,255,255,255,255,
		255,255,255,255,000,000,000,000,
		255,255,255,255,000,000,000,000,
		255,255,255,255,000,000,000,000,
		000,000,000,000,000,000,000,000,
		000,000,000,000,000,000,000,000,
		000,000,000,000,000,000,000,000,
		000,000,000,000,000,000,000,000
	};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 8, 8, 0, GL_RED, GL_UNSIGNED_BYTE, data2_);
	glGenerateMipmap(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);



	auto t = glGetUniformLocation(Render::renderShader, "t1");
	auto t2 =glGetUniformLocation(Render::renderShader, "t2");

	std::cout << "loc1: " << t << "  Loc2: " << t2 << "\n";

	glUniform1i(t, 0);
	glUniform1i(t2, 1);


	int index_ = 14;

	float milis=0;
	float dist = 0;
		
	float lightLocx = 0;

	glm::vec3 light_position(0, 0, 0);
	glm::vec3 light_ambient(0, 0, 0);
	glm::vec3 light_diffuse(0, 0, 0);
	glm::vec3 light_specular(0,0,0);

	glm::vec3 material_ambient(.25, .114, .06);
	glm::vec3 material_diffuse(.4, .23, .103);
	glm::vec3 material_specular(.77, .45, .2);
	float material_shininess = 76.8;


	struct Light {
		alignas(16) glm::vec3 position;
		alignas(16) glm::vec3 ambient;
		alignas(16) glm::vec3 diffuse;
		alignas(16) glm::vec3 specular;
	};

	Light lights[] = {
		{{5,5,0},{1,1,1},{1,1,1},{1,1,1}},
		{{-20,10,0},{1,1,1},{.2,.25,.25},{1,1,1}},
		{{0,10,20},{0,0,1},{.5,0,.25},{.5,.5,1}}
	};

	int numLights = sizeof(lights) / sizeof(Light);
	std::cout << sizeof(glm::vec3) << " size here <-" << "\n";
	std::cout << sizeof(int) << " " << sizeof(short) << " " << sizeof(float) << "\n";


	// we will test shader storage buffer objects here
	unsigned int ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(lights)+sizeof(int)*4, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(int), &numLights);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16, sizeof(Light)*numLights, lights);



	// test drawing other stuff
	float vertices[] = {
		// x, y ,z, u, v
		// do the bottom left triangle
			0,0,0,1,1,
			0,-1,0,1,0,
			-1,0,0,0,1,
			

			0,-1,0,1,0,
			-1,0,0,0,1,
			-1,-1,0,0,0,

			// top right triangle
			0,0,0,0,0,
			0,1,0,0,1,
			1,0,0,1,0,


			0,1,0,0,1,
			1,0,0,1,0,
			1,1,0,1,1,

			// bottom right triangle
			0,0,0,0,1,
			0,-1,0,0,0,
			1,0,0,1,1,

			0,-1,0,0,0,
			1,0,0,1,1,
			1,-1,0,1,0,

			//// bottom right triangle
			//-1,-1,0,0,0,
			//-1,1,0,0,1,
			//1,-1,0,1,0,

			//-1,1,0,0,1,
			//1,1,0,1,1,
			//1,-1,0,1,0,

	};
	unsigned int screen;
	glGenBuffers(1, &screen);
	glBindBuffer(GL_ARRAY_BUFFER, screen);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	unsigned int depth,color;
	glGenTextures(1, &depth);
	glGenTextures(1, &color);

	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800,0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);


	glBindTexture(GL_TEXTURE_2D, depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 800, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);


	unsigned int depthFbo;
	glGenFramebuffers(1, &depthFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFbo);

	unsigned int depthMap;
	unsigned int swidth = 1024, sheight = 1024;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, swidth, sheight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	//glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_2D, texture);


	glUseProgram(Render::screenShader);

	auto t3 = glGetUniformLocation(Render::screenShader, "framebuffer");
	auto t4 = glGetUniformLocation(Render::shadowShader, "framebuffer");


	std::cout << "loc3: " << t3 << "\n";
	std::cout << "loc4: " << t4 << "\n";


	glUniform1i(t3, 2);

	glUseProgram(Render::shadowShader);
	glUniform1i(t4, 2);

	glUseProgram(Render::screenShader);

	Render::initDebugScreen();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depth);


	auto d = glGetUniformLocation(Render::screenShader, "isDepth");

	auto d2 = glGetUniformLocation(Render::shadowShader, "isDepth");


	float angle = 0;

	
	while (Render::keepWindow) {
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		float currentFrame = glfwGetTime();
		Render::dt = currentFrame - Render::lastFrame;
		Render::lastFrame = currentFrame;
		Render::camera.speed = Render::camera.baseSpeed * Render::dt;

		float* buff = new float[800 * 800 * 4];
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,GL_FLOAT,buff);

	/*	int c = 0;
		for (int i = 0; i < 50; i++) {
			for (int j = 0; j < 50; j++) {
				std::cout << (int)buff[c++] << " ";

			}
			std::cout << "\n";
		}*/

		//float out = ((1.0 / buff[0]) - (1.0 / .01)) / (1 / 1000.0 - 1 / .01);

		float out = (buff[0]) * (1 / 1000.0 - 1 / .01) + 1 / .01;

		std::cout << 1/out << "\n";

		//std::cout << "\n\n";

		//std::cout << (int)buff[4 * 100 * 50] << "\n";

		delete[] buff;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH);
		glUseProgram(Render::screenShader);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, color);



		glBindBuffer(GL_ARRAY_BUFFER, screen);
		glVertexAttribDivisor(1, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float)*3));

		// draw origional image
		glUniform1i(d, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// draw depths
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		glUniform1i(d, 1);

		glDrawArrays(GL_TRIANGLES, 6, 6);

		// draw shadows
		glUseProgram(Render::shadowShader);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		glUniform1i(d2, 0);

		// give cam pos
		glUniform3f(glGetUniformLocation(Render::shadowShader,"camPos"),Render::camera.cameraPos.x, Render::camera.cameraPos.y, Render::camera.cameraPos.z);



		glDrawArrays(GL_TRIANGLES, 12,6);



		//
		//
		// This is getting shadow information
		//
		//

		glBindFramebuffer(GL_FRAMEBUFFER, depthFbo);
		glViewport(0, 0, swidth, sheight);
		glClear(GL_DEPTH_BUFFER_BIT);

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(1, 1);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH);

		glUseProgram(Render::depthShader);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glm::mat4 model(1.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(0,20,.0001), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		angle += .01;

		float near_plane = 1.0f, far_plane = 10.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f, 40, 0.0f),
			glm::vec3(0.01f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;


		std::cout << "Position: " << Render::camera.cameraPos.x << " " << Render::camera.cameraPos.y << " " << Render::camera.cameraPos.z << "\n";
		std::cout << "Front: " << Render::camera.cameraFront.x << " " << Render::camera.cameraFront.y << " " << Render::camera.cameraFront.z << "\n";
		std::cout << "Up: " << Render::camera.cameraUp.x << " " << Render::camera.cameraUp.y << " " << Render::camera.cameraUp.z << "\n";




		glUniformMatrix4fv(glGetUniformLocation(Render::depthShader, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(Render::depthShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(Render::depthShader, "projection"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));



		
		for (Render::Object* o : Render::objects) {
			int elements = o->insts->da->elements;
			//std::cout << elements << "\n";
			if (elements <= 0) {
				continue;
			}


			// bind the position buffer and send the vertices to gpu
			glBindBuffer(GL_ARRAY_BUFFER, o->positions);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), 0);
			//glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), (void*)(sizeof(glm::vec3) * 1));
			//glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), (void*)(sizeof(glm::vec3) * 2));
			//glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), (void*)(sizeof(glm::vec3) * 3));


			// load in the buffer of all instances
			glBindBuffer(GL_ARRAY_BUFFER, o->insts->buffer);


			// this is translation
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int)));

			// this is rotation
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(glm::vec3) + sizeof(int)));

			// this is for scalation
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int) + 2 * sizeof(glm::vec3)));

			// this is for color
			//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int) + 3 * sizeof(glm::vec3)));




			// give the ebo to the gpu
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o->ebo);



			// finally, send the draw command to the gpu
			glDrawElementsInstanced(GL_TRIANGLES, o->eboSize, GL_UNSIGNED_INT, 0, elements);


		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		//
		//
		// This is actual rendering
		//
		//

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glViewport(0, 0, 800, 800);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glUseProgram(Render::renderShader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		//glBindTexture(GL_TEXTURE_2D, color);
		// 
		// update material information
		glUniform3f(glGetUniformLocation(Render::renderShader, "material.ambient"), material_ambient.x, material_ambient.y, material_ambient.z);
		glUniform3f(glGetUniformLocation(Render::renderShader, "material.diffuse"), material_diffuse.x, material_diffuse.y, material_diffuse.z);
		glUniform3f(glGetUniformLocation(Render::renderShader, "material.specular"), material_specular.x, material_specular.y, material_specular.z);
		glUniform1f(glGetUniformLocation(Render::renderShader, "material.shininess"), material_shininess);

		glUniformMatrix4fv(glGetUniformLocation(Render::renderShader, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));


		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(1, 1);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH);
		
		glUniformMatrix4fv(glGetUniformLocation(Render::renderShader, "model"), 1, GL_FALSE, glm::value_ptr(Render::model));
		glUniformMatrix4fv(glGetUniformLocation(Render::renderShader, "view"), 1, GL_FALSE, glm::value_ptr(Render::view));
		glUniformMatrix4fv(glGetUniformLocation(Render::renderShader, "projection"), 1, GL_FALSE, glm::value_ptr(Render::projection));
		glUniform3fv(glGetUniformLocation(Render::renderShader, "camPos"), 1, glm::value_ptr(Render::camera.cameraPos));
		glUniform3fv(glGetUniformLocation(Render::renderShader, "camFront"), 1, glm::value_ptr(Render::camera.cameraFront));


		Render::camera.translate(Render::left, Render::right, Render::up, Render::down, Render::forward, Render::backward);
		Render::view = glm::lookAt(Render::camera.cameraPos, Render::camera.cameraPos + Render::camera.cameraFront, Render::camera.cameraUp);
		Render::projection = glm::perspective(glm::radians(Render::camera.fov), (float)(800.0 / 800.0), .01f, 1000.0f);


		Render::draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);




		




		
		Render::drawDebug(milis);
		glfwSwapBuffers(Render::window);
		glfwPollEvents();
		Render::keepWindow = !glfwWindowShouldClose(Render::window);

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		milis = (end - begin).count() / 1000000.0;
		//std::cout << "Total Time difference = " << milis << "[ms]" << " FPS: " << 1000.0 / milis << "\n";

	}

	

	//exit(1);

	Render::exit();



	return 0;
}


/*
* 
* 
		if (Render::getKey(GLFW_KEY_UP)) {
			light_position.z += .01;
		}

		if (Render::getKey(GLFW_KEY_DOWN)) {
			light_position.z -= .01;
		}

		if (Render::getKey(GLFW_KEY_LEFT)) {
			light_position.x += .01;
		}

		if (Render::getKey(GLFW_KEY_RIGHT)) {
			light_position.x -= .01;
		}

		if (Render::getKey(GLFW_KEY_ENTER)) {
			light_position.y += .01;
		}

		if (Render::getKey(GLFW_KEY_RIGHT_SHIFT)) {
			light_position.y -= .01;
		}

		if (Render::getKey(GLFW_KEY_R)) {
			light_ambient += .001;
		}

		if (Render::getKey(GLFW_KEY_F)) {
			light_ambient -= .001;
		}

		if (Render::getKey(GLFW_KEY_T)) {
			light_diffuse += .001;
		}

		if (Render::getKey(GLFW_KEY_G)) {
			light_diffuse -= .001;
		}

		if (Render::getKey(GLFW_KEY_Y)) {
			light_specular += .001;
		}

		if (Render::getKey(GLFW_KEY_H)) {
			light_specular -= .001;
		}

		if (Render::getKey(GLFW_KEY_U)) {
			material_ambient += .001;
		}

		if (Render::getKey(GLFW_KEY_J)) {
			material_ambient -= .001;
		}

		if (Render::getKey(GLFW_KEY_I)) {
			material_diffuse += .001;
		}

		if (Render::getKey(GLFW_KEY_K)) {
			material_diffuse -= .001;
		}

		if (Render::getKey(GLFW_KEY_O)) {
			material_specular += .001;
		}

		if (Render::getKey(GLFW_KEY_L)) {
			material_specular -= .001;
		}

		if (Render::getKey(GLFW_KEY_P)) {
			material_shininess += .1;
		}

		if (Render::getKey(GLFW_KEY_SEMICOLON)) {
			material_shininess -= .1;
		}

	*/