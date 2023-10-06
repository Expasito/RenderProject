
#include <Render/Render.h>
#include <chrono>
#include <Soil/SOIL.h>


// This supposively makes the program run faster by telling the gpu to run in performance mode?
#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) uint32_t NvOptimusEnablement = 1;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#ifdef __cplusplus
}
#endif

/*
*  Use WASD to move, mouse to look and B to change wireframe mode
*/






std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
	os << vec.x << " " << vec.y << " " << vec.z << " ";
	return os;
}




/*
* 
* Note: For importing models, rename the extention to .rpo so the linker does not mistake the .obj file as a c object file. 
* 
*/





























/*
* 
* This checks for errors in the program and catches them 
* 
*/
void checkErrors() {
	GLenum err;


	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "ERROR: " << err << "\n";
		exit(1);
	}
}





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


class DirectionalLight {
public:
	unsigned int depthTexture;
	unsigned int fbo;
	unsigned int width, height;

	float nearClip, farClip;

	glm::mat4 lightProjection;
	glm::mat4 lightView;
	glm::mat4 lightSpaceMatrix;

	glm::vec3 position;
	glm::vec3 lookat;

	DirectionalLight(int width_, int height_, float nearClip_, float farClip_, glm::vec3 position_, glm::vec3 lookat_, float lw) {

		width = width_;
		height = height_;
		nearClip = nearClip_;
		farClip = farClip_;


		
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);


		position = position_;
		lookat = lookat_;

		lightProjection = glm::ortho(-lw, lw, -lw, lw, nearClip, farClip);

		lightView = glm::lookAt(position,
			lookat,
			glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;

	}

	void update(glm::vec3 position_, glm::vec3 lookat_) {
		position = position_;
		lookat = lookat_;
		lightView = glm::lookAt(position,
			lookat,
			glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;

	}



private:
};




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
	Render::init(800, 800, false);
	Render::camera.baseSpeed = 30;











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
	for (int i = 0; i < sizeof(ebo_) / sizeof(unsigned int); i++) {
		std::cout << i << "\n";
		ebo.push_back(ebo_[i]);
	}

	//unsigned int test_, test2_;
	//glGenBuffers(1, &test_);
	//glBindBuffer(GL_ARRAY_BUFFER, test_);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Render::vertex) * positions.size(), &positions[0], GL_STATIC_DRAW);

	//glGenBuffers(1, &test2_);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, test2_);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ebo.size(), &ebo[0], GL_STATIC_DRAW);




	Render::addModel("assets/Cube3.rpo", "CUBE", 100000000, 10000000);











/*
* 
* 

	// has to be at least 2
	// this represents the number of 'chunks' in each direction
	const int width = 33;
	const int height = 33;

	int h_ = 50;
	int arr[width][height];
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			//h_ = rand() % 
			arr[i][j] = (rand() / (float)RAND_MAX) * h_;
		}
	}

	int counter__ = 0;

	h_ = 50;

	// this is the size of a 'chunk'
	int blocks = 16;
	//srand(0);

	// generate a cool world with terrain
	for (int i = 0; i < width - 1; i++) {
		for (int j = 0; j < height - 1; j++) {
			// this is the width and length
			float slopex = (arr[i + 1][j] - arr[i][j]) / (float)blocks;
			float slopez = (arr[i][j + 1] - arr[i][j]) / (float)blocks;
			for (int k = 0; k < blocks; k++) {
				for (int l = 0; l < blocks; l++) {

					// take each point out of the 4 and multiply by the situation where when the k and l values are
					// at its location, the value at that point is used
					float h = (arr[i][j]) * (1 - (float)k / blocks) * (1 - (float)l / blocks) +
						(arr[i + 1][j + 1]) * ((float)k / blocks) * ((float)l / blocks) +
						(arr[i + 1][j]) * ((float)k / blocks) * (1 - (float)l / blocks) +
						(arr[i][j + 1]) * (1 - (float)k / blocks) * ((float)l / blocks)
						;


					// put the cubes in the right y axis location
					h = round(h) * 2;
					//std::cout << "H: " << h << "\n";
					// h should always be even due to the *2 so we add 2 each layer
					for (int m = h - 2 - 2 - 2 - 2; m <= h; m += 2) {
						counter__++;
						// add a white cube for snow
						if (h > h_ * 2 * .75) {
							Render::addInstance("CUBE", { 2*(i * blocks + k),m,2*(j * blocks + l) }, { 0,0,0 }, { 1,1,1 }, { .75,.75,.75 });
						}
						else if (h < h_ * 2 * .25) {
							Render::addInstance("CUBE", { 2*(i * blocks + k),m,2*(j * blocks + l) }, { 0,0,0 }, { 1,1,1 }, { 0,0,.75 });
						}
						else {
							Render::addInstance("CUBE", { 2*(i * blocks + k),m,2*(j * blocks + l) }, { 0,0,0 }, { 1,1,1 }, { 0,.75,0 });
						}

						//Render::addInstance("CUBE", { 0,0,0 }, { 0,0,0 }, { 1,1,1 }, { .5,.5,.5 });
					}

					//Render::addInstance("CUBE", {2*(i*blocks+k),h,2*(j*blocks+l)}, {0,0,0}, {1,1,1}, {.5,.4,.6});
				}
			}
			// right here, calculate the slope between the xs and zs and then add all of the points
		}
	}


	std::cout << "Origional draws: " << counter__ << " entities\n";
	//exit(1);



*/



	/*
	*
	* 
	* Keep the old one above so we can not mess it up. This will be used with the chunking system for testing
	* Lets try some terrain generation to really test the shadows!
	*
	* Plan: try to replicate a form of perlin noise and generate cubes at each point with the correct height
	*
	*/






// this is a copy of the pointers so we can reset if we need



	// has to be at least 2
	// this represents the number of 'chunks' in each direction
	const int width = 33;
	const int height = 33;

std::vector<FillerArray*> fillers(width*height);

	int h_ = 50;
	int arr[width][height];
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			//h_ = rand() % 
			arr[i][j] = (rand() / (float)RAND_MAX) * h_;
		}
	}

	int counter__ = 0;

	// this is the size of a 'chunk'
	int blocks = 16;
	//srand(0);

	// generate a cool world with terrain
	for (int i = 0; i < width - 1; i++) {
		for (int j = 0; j < height - 1; j++) {
			// this is the width and length
			float slopex = (arr[i + 1][j] - arr[i][j]) / (float)blocks;
			float slopez = (arr[i][j + 1] - arr[i][j]) / (float)blocks;

			// Init the filler array and add to the list of arrays
			FillerArray* fa = new FillerArray(blocks * blocks * h_ * 2, blocks * blocks * h_ * 2);
			fillers.push_back(fa);

			for (int k = 0; k < blocks; k++) {
				for (int l = 0; l < blocks; l++) {

					// take each point out of the 4 and multiply by the situation where when the k and l values are
					// at its location, the value at that point is used
					float h = (arr[i][j]) * (1 - (float)k / blocks) * (1 - (float)l / blocks) +
						(arr[i + 1][j + 1]) * ((float)k / blocks) * ((float)l / blocks) +
						(arr[i + 1][j]) * ((float)k / blocks) * (1 - (float)l / blocks) +
						(arr[i][j + 1]) * (1 - (float)k / blocks) * ((float)l / blocks)
						;


					// put the cubes in the right y axis location
					h = round(h) * 2;
					//std::cout << "H: " << h << "\n";
					// h should always be even due to the *2 so we add 2 each layer
					for (int m = h - 2 - 2 - 2 - 2; m <= h; m += 2) {
						counter__++;


						// add a white cube for snow
						if (h > h_ * 2 * .75) {
							fa->add({ 2 * (i * blocks + k),m,2 * (j * blocks + l) }, { 0,0,0 }, { 1,1,1 }, { .75,.75,.75});
						}
						else if (h < h_ * 2 * .25) {
							fa->add({ 2 * (i * blocks + k),m,2 * (j * blocks + l) }, { 0,0,0 }, { 1,1,1 }, { 0,0,.75 });
						}
						else {
							fa->add({ 2 * (i * blocks + k),m,2 * (j * blocks + l) }, { 0,0,0 }, { 1,1,1 }, { 0,.75,0 });
						}

					}

				}
			}
		}
	}



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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);




	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width__, height__, 0, GL_RGB, GL_UNSIGNED_BYTE, data_);


	glGenerateMipmap(GL_TEXTURE_2D);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
	auto t2 = glGetUniformLocation(Render::renderShader, "t2");


	std::cout << "loc1: " << t << "  Loc2: " << t2 << "\n";

	glUniform1i(t, 0);
	glUniform1i(t2, 1);
	glUniform1i(glGetUniformLocation(Render::renderShader, "depth1"), 2);
	glUniform1i(glGetUniformLocation(Render::renderShader, "depth2"), 3);






	int index_ = 14;

	float milis = 0;
	float dist = 0;

	float lightLocx = 0;

	glm::vec3 light_position(0, 0, 0);
	glm::vec3 light_ambient(0, 0, 0);
	glm::vec3 light_diffuse(0, 0, 0);
	glm::vec3 light_specular(0, 0, 0);

	glm::vec3 material_ambient(.5, .5, .5);
	glm::vec3 material_diffuse(.5, .75, .5);
	glm::vec3 material_specular(.9, .9, .9);
	float material_shininess = 500.0;


	struct Light {
		alignas(16) glm::vec3 position;
		alignas(16) glm::vec3 ambient;
		alignas(16) glm::vec3 diffuse;
		alignas(16) glm::vec3 specular;
	};

	Light lights[] = {
		{{0,0,0},{1,1,1},{1,1,1},{1,1,1}},
		//{{-20,10,0},{1,1,1},{.2,.25,.25},{1,1,1}},
		//{{0,10,20},{0,0,1},{.5,0,.25},{.5,.5,1}}
	};

	int numLights = sizeof(lights) / sizeof(Light);
	//int numLights = 3;
	std::cout << sizeof(glm::vec3) << " size here <-" << "\n";
	std::cout << sizeof(int) << " " << sizeof(short) << " " << sizeof(float) << "\n";


	// we will test shader storage buffer objects here
	unsigned int ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(lights) + sizeof(int) * 4, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(int), &numLights);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16, sizeof(Light) * numLights, lights);



	// test drawing other stuff
	float vertices[] = {
		// x, y ,z, u, v

			// do the top right
			1,1,0,1,1,
			1,-1,0,1,0,
			-1,1,0,0,1,

			// do the bottom triangle
			1,-1,0,1,0,
			-1,1,0,0,1,
			-1,-1,0,0,0,

			//// and bottom left
			//0,0,0,0,0,
			//0,1,0,0,1,
			//1,0,0,1,0,


			//0,1,0,0,1,
			//1,0,0,1,0,
			//1,1,0,1,1,

			//// bottom right triangle
			//0,0,0,0,1,
			//0,-1,0,0,0,
			//1,0,0,1,1,

			//0,-1,0,0,0,
			//1,0,0,1,1,
			//1,-1,0,1,0,


	};
	unsigned int screen;
	glGenBuffers(1, &screen);
	glBindBuffer(GL_ARRAY_BUFFER, screen);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	unsigned int depth, color;
	glGenTextures(1, &depth);
	glGenTextures(1, &color);

	glBindTexture(GL_TEXTURE_2D, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
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

	//DirectionalLight dl(1024, 1024, 0.0f, 1000.0f, {20,50,20}, {0,0,0});

	DirectionalLight dls[2] = {
		DirectionalLight(4096, 4096, 0.0f, 1000.0f, {0,41,0}, {0,40,1},1000),
		DirectionalLight(32, 32, 0.0f, 1000.0f, {0.00001,50,0.001}, {0,0,0},10)
	};


	//dls[0].position;
	//Render::addInstance("CUBE", dls[0].position, { 0,0,0 }, { 10,10,10 }, { 1,1,1 });
	//Render::addInstance("CUBE", dls[0].lookat, {0,0,0}, {10,10,10}, {1,1,1});







	/*
	*
	*
	* Test creating a bunch of textures and add them to an array
	*
	*/

	// 4x4 image
	char text1[] = {
		255, 255, 255, 255,   255, 255, 255, 255,   255, 255, 000, 255,   000, 255, 255, 255,
		255, 000, 255, 255,   255, 255, 255, 255,   255, 255, 000, 255,   000, 255, 255, 255,
		255, 000, 255, 255,   255, 255, 255, 255,   255, 000, 255, 255,   255, 000, 255, 255,
		000, 000, 255, 255,   255, 255, 000, 255,   000, 255, 255, 255,   255, 255, 255, 255,
	};

	char text2[] = {
		000, 255, 000, 255,   255, 255, 255, 255,   255, 255, 000, 255,   000, 255, 255, 255,
		255, 000, 255, 255,   255, 255, 255, 255,   000, 255, 000, 255,   000, 255, 255, 255,
		255, 000, 000, 255,   255, 255, 255, 255,   255, 000, 255, 255,   255, 000, 255, 255,
		000, 000, 255, 255,   255, 255, 000, 255,   000, 255, 255, 255,   255, 255, 255, 255,
	};

	unsigned int text1Texture;
	glGenTextures(1, &text1Texture);
	glBindTexture(GL_TEXTURE_2D, text1Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, text1);
	glGenerateMipmap(GL_TEXTURE_2D);


	unsigned int text2Texture;
	glGenTextures(1, &text2Texture);
	glBindTexture(GL_TEXTURE_2D, text2Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, text2);
	glGenerateMipmap(GL_TEXTURE_2D);








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

	glm::vec3 position = glm::vec3(0, 20, 20);
	glm::vec3 direction = glm::vec3(0.0001, -1, 0);
	float fov = 80.0f;

	//int key = Render::addInstance("Test", position-direction, { 0,0,0 }, { 1,1,1 }, { 1,1,1 });





	const char* path = "Assets/Cube3.rpo";

	Render::Model m = Render::loadModel(path);
	for (int i = 0; i < m.indices.size(); i++) {
		//std::cout << m.indices.at(i) << "\n";
	}


	/*
	*
	* We are going to try adding frustrum culling by setting up a chunking system.
	* Basically, when adding object to the scene, theo object is also added to the chunks it resides in
	* When updating, it is double checked that it is still in those chunks and alos if its in new ones.
	* We then will iterate over the list of chunks to remove chunks that are out of the viewing range.
	* 
	* This could alos add occlusion culling if we figure that out.
	* 
	*/

	/*

	int chunks = 16;

	std::vector<FillerArray*> fillers;

	int blocksPerChunk = 32;

	// this is a copy of the pointers so we can reset if we need
	std::vector<FillerArray**> fillersAll(chunks* chunks);


	for (int i = 0; i < chunks; i++) {
		for (int j = 0; j < chunks; j++) {
			//std::cout << i << "\n";
			FillerArray* fa = new FillerArray(blocksPerChunk*blocksPerChunk*5*2, blocksPerChunk * blocksPerChunk * 5*2);
			fillers.push_back(fa);
			fillersAll.push_back(&fa);
			for (int k = 0; k < blocksPerChunk; k++) {
				for (int l = 0; l < blocksPerChunk; l++) {
					for (int m = 0; m < 5; m++) {
						fa->add({ 2*(i * blocksPerChunk + k),2*m,2*(j * blocksPerChunk + l) }, { 0,0,0 }, { 1,1,1 }, { 1,1,1 });
					}
					
				}
			}
			
			//char name[7] = "Orbj";
			//name[4] = '0' + i;
			//name[5] = '0' + j;
			//std::cout << name << "\n";
			//Render::objects.push_back(new Render::Object(name, path, m.positions, m.ebo, m.indices.size() - 3, fillers.at(fillers.size()-1)));
		}
		
	}

	*/
	

	//FillerArray* fa = new FillerArray(100, 100);

	//Render::objects.push_back(new Render::Object("Orbj", path, m.positions, m.ebo, m.indices.size() - 3,fa));

	//for (Render::Object* o : Render::objects) {
	//	if (o->name.compare("Orbj") == 0) {
	//		return o->insts->add(pos, rot, scal, colo);

	//	}

	//}

	//fa->add({ 0,0,0 }, { 0,0,0 }, { 1,1,1 }, { 1,1,1 });




	struct Output {
		// cap of the array
		int capacity;
		// num elements
		int elements;
		unsigned int buff;
	};

	Output output;
	output.capacity = blocks*blocks*50*width*height;
	//output.capacity = chunks * chunks * 2;
	output.elements = 0;
	glGenBuffers(1, &output.buff);
	glBindBuffer(GL_ARRAY_BUFFER, output.buff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FillerArray::Element)* output.capacity, NULL, GL_DYNAMIC_DRAW);

	//FillerArray::Element data = { 0,{0,0,0},{0,0,0},{1,1,1}, {1,1,1} };
	//glBufferSubData(GL_ARRAY_BUFFER, output.elements * sizeof(FillerArray::Element), sizeof(FillerArray::Element), &data);
	//output.elements++;
	

	// this is the main fillerarray to draw from
	//FillerArray* ARRAY = new FillerArray(100000, 10000);
	Render::Object* o = Render::objects.at(0);

	std::cout << o->name << "\n";
	//exit(1);


	unsigned int read;
	glGenBuffers(1, &read);






	
	while (Render::keepWindow) {
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		float currentFrame = glfwGetTime();
		Render::dt = currentFrame - Render::lastFrame;
		Render::lastFrame = currentFrame;
		Render::camera.speed = Render::camera.baseSpeed * Render::dt;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
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



		/*
		* 
		* 
		*  NOTE: WE DISABLED LIGHTING FOR NOW SINCE IT IS VERY PERFORMANCE INTENSIVE WITH LOTS OF OBJECTS
		* 
		* 
		*/


		for (DirectionalLight dl : dls) {
			//continue;
			glBindFramebuffer(GL_FRAMEBUFFER, dl.fbo);
			//glCullFace(GL_FRONT);
			glViewport(0, 0, dl.width, dl.height);
			glClear(GL_DEPTH_BUFFER_BIT);

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(1, 1);

			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);

			glUseProgram(Render::depthShader);

			glActiveTexture(GL_TEXTURE2);
			//glBindTexture(GL_TEXTURE_2D, texture1);
			glBindTexture(GL_TEXTURE_2D, dl.depthTexture);





			//glUniformMatrix4fv(glGetUniformLocation(Render::depthShader, "model"), 1, GL_FALSE, glm::value_ptr(model));
			//glUniformMatrix4fv(glGetUniformLocation(Render::depthShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(Render::depthShader, "projection"), 1, GL_FALSE, glm::value_ptr(dl.lightSpaceMatrix));


			// give the ebo to the gpu
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o->ebo);

			// bind the position buffer and send the vertices to gpu
			glBindBuffer(GL_ARRAY_BUFFER, o->positions);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), 0);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), (void*)(sizeof(glm::vec3) * 1));
			glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), (void*)(sizeof(glm::vec3) * 2));
			glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), (void*)(sizeof(glm::vec3) * 3));

			for (int j = 0; j < fillers.size(); j++) {
				FillerArray* f = fillers.at(j);
				if (f == NULL) {
					continue;

				}

				int elementsToCopy = f->da->elements;

				// load in the buffer of all instances
				glBindBuffer(GL_ARRAY_BUFFER, f->buffer);

				// this is translation
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int)));

				// this is rotation
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(glm::vec3) + sizeof(int)));

				// this is for scalation
				glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int) + 2 * sizeof(glm::vec3)));

				// this is for color
				glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int) + 3 * sizeof(glm::vec3)));

				// finally, send the draw command to the gpu
				glDrawElementsInstanced(GL_TRIANGLES, o->eboSize, GL_UNSIGNED_INT, 0, elementsToCopy);


			}

		}




		/*
		* 
		* We are going to be doing frustrum culling here first
		* 
		* 
		*/







		//glBindFramebuffer(GL_FRAMEBUFFER, 0);


		//
		//
		// This is actual rendering
		//
		//

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glCullFace(GL_BACK);
		glViewport(0, 0, Render::width, Render::height);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glUseProgram(Render::renderShader);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, dls[0].depthTexture);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, dls[1].depthTexture);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, dl.depthTexture);

		//glBindTexture(GL_TEXTURE_2D, color);
		// 
		// update material information
		glUniform3f(glGetUniformLocation(Render::renderShader, "material.ambient"), material_ambient.x, material_ambient.y, material_ambient.z);
		glUniform3f(glGetUniformLocation(Render::renderShader, "material.diffuse"), material_diffuse.x, material_diffuse.y, material_diffuse.z);
		glUniform3f(glGetUniformLocation(Render::renderShader, "material.specular"), material_specular.x, material_specular.y, material_specular.z);
		glUniform1f(glGetUniformLocation(Render::renderShader, "material.shininess"), material_shininess);

		glUniformMatrix4fv(glGetUniformLocation(Render::renderShader, "lightSpaceMatrix1"), 1, GL_FALSE, glm::value_ptr(dls[0].lightSpaceMatrix));
		glUniformMatrix4fv(glGetUniformLocation(Render::renderShader, "lightSpaceMatrix2"), 1, GL_FALSE, glm::value_ptr(dls[1].lightSpaceMatrix));



		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(1, 1);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		
		glUniformMatrix4fv(glGetUniformLocation(Render::renderShader, "model"), 1, GL_FALSE, glm::value_ptr(Render::model));
		glUniformMatrix4fv(glGetUniformLocation(Render::renderShader, "view"), 1, GL_FALSE, glm::value_ptr(Render::view));
		glUniformMatrix4fv(glGetUniformLocation(Render::renderShader, "projection"), 1, GL_FALSE, glm::value_ptr(Render::projection));
		glUniform3fv(glGetUniformLocation(Render::renderShader, "camPos"), 1, glm::value_ptr(Render::camera.cameraPos));
		glUniform3fv(glGetUniformLocation(Render::renderShader, "camFront"), 1, glm::value_ptr(Render::camera.cameraFront));


		Render::camera.translate(Render::left, Render::right, Render::up, Render::down, Render::forward, Render::backward);
		Render::view = glm::lookAt(Render::camera.cameraPos, Render::camera.cameraPos + Render::camera.cameraFront, Render::camera.cameraUp);
		Render::projection = glm::perspective(glm::radians(Render::camera.fov), (float)(800.0 / 800.0), .01f, 10000.0f);


		//Render::draw();


		


		/*
		* 
		* Go through each element and order them front to back so the gpu overdraws the least
		* 
		*/

		output.elements = 0;

		
		/*
		for (int j = 0; j < fillers.size(); j++) {
			FillerArray* f = fillers.at(j);
			if (f == NULL) {
				continue;

			}
			glBindBuffer(GL_ARRAY_BUFFER, f->buffer);

			int elementsToCopy = f->da->elements;

			// now copy the buffers
			glBindBuffer(GL_COPY_READ_BUFFER, read);
			glBufferData(GL_COPY_READ_BUFFER, sizeof(FillerArray::Element) * elementsToCopy, NULL, GL_DYNAMIC_DRAW);
			glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_READ_BUFFER, 0, 0, sizeof(FillerArray::Element) * elementsToCopy);

			
			glBindBuffer(GL_ARRAY_BUFFER, output.buff);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, output.elements * sizeof(FillerArray::Element), sizeof(FillerArray::Element) * elementsToCopy);



			//output.elements += f->da->elements;
			output.elements += elementsToCopy;

			//break;



		}
		*/






		
		/*
		
		int elements = output.elements;
		//std::cout << "OUTPUT ELEMENTS: " << elements << "\n";

			//std::cout << "     O: " << fa << "   Elements: " << elements << "\n";
			//std::cout << elements << "\n";
			// make sure there are elements and not a null pointer
			if (elements <= 0) {
				//break;
			}

			//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

			// bind the position buffer and send the vertices to gpu
			glBindBuffer(GL_ARRAY_BUFFER, o->positions);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), 0);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), (void*)(sizeof(glm::vec3) * 1));
			glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), (void*)(sizeof(glm::vec3) * 2));
			glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), (void*)(sizeof(glm::vec3) * 3));


			// load in the buffer of all instances
			glBindBuffer(GL_ARRAY_BUFFER, output.buff);


			// this is translation
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int)));

			// this is rotation
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(glm::vec3) + sizeof(int)));

			// this is for scalation
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int) + 2 * sizeof(glm::vec3)));

			// this is for color
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int) + 3 * sizeof(glm::vec3)));




			// give the ebo to the gpu
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o->ebo);

			// finally, send the draw command to the gpu
			glDrawElementsInstanced(GL_TRIANGLES, o->eboSize, GL_UNSIGNED_INT, 0, elements);


		


		// Now bind the default frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		*/



		long totalElements = 0;


	// These can be done outside of the loop since the object is the same:

	// give the ebo to the gpu
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o->ebo);

	// bind the position buffer and send the vertices to gpu
	glBindBuffer(GL_ARRAY_BUFFER, o->positions);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), 0);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), (void*)(sizeof(glm::vec3) * 1));
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), (void*)(sizeof(glm::vec3) * 2));
	glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(Render::vertex), (void*)(sizeof(glm::vec3) * 3));

	for (int j = 0; j < fillers.size(); j++) {
		FillerArray* f = fillers.at(j);
		if (f == NULL) {
			continue;

		}

		int elementsToCopy = f->da->elements;
		totalElements += elementsToCopy;

		// load in the buffer of all instances
		glBindBuffer(GL_ARRAY_BUFFER, f->buffer);

		// this is translation
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int)));

		// this is rotation
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(glm::vec3) + sizeof(int)));

		// this is for scalation
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int) + 2 * sizeof(glm::vec3)));

		// this is for color
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int) + 3 * sizeof(glm::vec3)));

		// finally, send the draw command to the gpu
		glDrawElementsInstanced(GL_TRIANGLES, o->eboSize, GL_UNSIGNED_INT, 0, elementsToCopy);


	}





	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	std::cout << "Elements: " << totalElements << "\n";


			/*
			* 
			* 
			* Our goal is to beat 1,310,720 at 21 fps using the new method
			* 
			* Old Method: 21 fps using the idea of drawing all at once, no culling and no chunking
			* 
			* New Method 1: 8 fps with copying the good chunks into a large buffer and drawing all
			* 
			* New Method 2: 21 fps with drawing each chunk and no buffer copying
			* 
			* So now we implement culling and stuff like that while reducing the number of draw calls overall
			* 
			*/

		
		//Render::drawDebug(milis);
		glfwSwapBuffers(Render::window);
		glfwPollEvents();
		checkErrors();
		Render::keepWindow = !glfwWindowShouldClose(Render::window);

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		milis = (end - begin).count() / 1000000.0;
		std::cout << "Total Time difference = " << milis << "[ms]" << " FPS: " << 1000.0 / milis << "\n";

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