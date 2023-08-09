
#include <Render/Render.h>
#include <chrono>
#include <Soil/SOIL.h>


/*
*  Use WASD to move, mouse to look and B to change wireframe mode
*/


#include <Render/utils/FillerArray.h>

float test_ = 0.0;


//#define DEBUG 0

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

	//unsigned char* data_ = SOIL_load_image("textures/Test.png", &width__, &height__, &channels__, 0);

	//std::cout << channels__ << "\n";

	width__ = 10;
	height__ = 5;
	channels__ = 4;

	// this will be a temporary array. If the value is 0, then it will be black and transparent. If 1, then white and not
	unsigned char data__[] = {
		0,1,1,0,0,1,1,1,0,0,
		1,0,0,1,0,1,0,0,1,0,
		1,1,1,1,0,1,1,1,0,0,
		1,0,0,1,0,1,0,0,1,0,
		1,0,0,1,0,1,1,1,0,0
	};

	unsigned char* data_ = (unsigned char*)malloc(sizeof(unsigned char) * channels__ * width__ * height__);
	int c = 0;
	for (int i = 0; i < width__*height__; i++) {
		if (data__[i] == 1) {
			data_[c++] = 255;
			data_[c++] = 255;
			data_[c++] = 255;
			data_[c++] = 255;
		}
		else {
			data_[c++] = 0;
			data_[c++] = 0;
			data_[c++] = 0;
			data_[c++] = 0;
		}
	}

	//exit(1);


	//exit(1);


	//stbi_image_free(data_);



	//FILE* file = fopen("textures/Test.png", "rb");
	//unsigned char x;
	//fscanf(file, "%c", &x);
	//char buff[8];
	//fgets(buff, 8, file);
	//if(buff==0x89504e470d0a1a0a)
	//if (x == 0x89) {
	//	std::cout << "testing\n";
	//}
	//while (!feof(file)) {
	//	unsigned char x;
	//	fscanf(file, "%c", &x);
	//	printf("%02x", x);
	//}

	//exit(1);


	// we are going to open a png file and make a way to import them

	//HashTable<int> ht(7, &hash);

	//ht.add(1);

	//ht.see();



	//exit(1);



	/*
	* Look into SSBOs for variable length buffers for lights, triangles, etc.
	* Can be used for shadows and ray casting
	*/
	/*
	* Also, look at making a hash table for adding instances or removing them for models
	* Currently, addInstance and RemoveInstance are O(n) but easily could be O(1) with
	* a hash table
	* 
	* 
	* 
	*/

	srand(time(0));
	Render::init(800,800,false);
	Render::camera.baseSpeed = 10;

	//Render::loadSave("saves/input.rpo");

	//exit(1);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	//Render::addModel("assets/sphere.obj", "Cube",100,100);
	//Render::addModel("assets/sphere.obj", "Cube");
	//Render::addModel("assets/monkey.obj", "Monkey");

	//Render::addModel("assets/room.obj", "Room", 10000, 10000);

	//Render::addInstance("Room", { 20,0,0 }, { 0,0,0 }, { 1,1,1 }, { .5,.5,.5 });

	Render::addModel("assets/room.obj", "CUBE", 100, 100);
	Render::addModel("assets/sphere.obj", "Sphere", 100, 100);

	Render::addInstance("CUBE", { 0,-10,0 }, { 0,0,0 }, { 10,1,10 }, {1,1,1});

	//long key = Render::addInstance("Sphere", {0,1,0}, {0,0,0}, {1,1,1}, {1,1,1});


	// this will be 5 by 5


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//unsigned char data_[2000 * 2000 * 3];
	//unsigned char* data_ = new unsigned char[width__ * height__ * 3];


	//exit(1);


	

	//unsigned char* data_ = (unsigned char*)malloc(sizeof(unsigned char) * width__ * height__ * 3);
	//for (int i = 0; i < width__*height__*3;) {
	//	data_[i++] = 255;
	//	data_[i++] = 255;
	//	data_[i++] = 0;
	//}
	

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width__, height__, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_);
	glGenerateMipmap(GL_TEXTURE_2D);



	

	//float max_dist = 200;
	//for (int i = 0; i < 50000; i++) {
	//	Render::addInstance("Cube",
	//		{ (float)rand() / (float)RAND_MAX * max_dist - max_dist/2.0 ,(float)rand() / (float)RAND_MAX * max_dist - max_dist / 2.0,(float)rand() / (float)RAND_MAX * max_dist - max_dist / 2.0 },
	//		//{0,0,0},
	//		{ (float)rand() / (float)RAND_MAX * 5 , (float)rand() / (float)RAND_MAX * 5 , (float)rand() / (float)RAND_MAX * 5 },
	//		{ (float)rand() / (float)RAND_MAX * 5 - 2.5 , (float)rand() / (float)RAND_MAX * 5 - 2.5 , (float)rand() / (float)RAND_MAX * 5 - 2.5 },
	//		{.5,.5,.5}
	//		//{(float)rand()/(float)RAND_MAX,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX }
	//	);
	//}

	//long key = Render::addInstance("Cube", { 110,0,0 }, { 0,0,0 }, { 1,1,1 }, { .75,.75,.01 });

	//Render::addInstance("Cube", { 10,0,0 }, { 0,0,0 }, { 1,1,1 }, { 0,.75,0 });

	//Render::addInstance("Cube",	{ 20,0,0 }, { 0,0,0 }, { 1,1,1 }, { .75,0,0 });


	//Render::removeInstances("Cube");
	//Render::removeInstances("Cube");
	//Render::addInstance("Monkey", {1,1,1}, {90,1,1}, {2,1,4});
	//Render::removeAllInstances();

	//std::vector<float> time(10000);

	int index_ = 14;

	float milis=0;
	float dist = .01;

	//for (int i = 0; i < 1000000; i++) {
	//	Render::addInstance("Cube", index_,
	//		//{ index_ / 10.0,0,0 },
	//		{(rand()-RAND_MAX/2)/dist,(rand() - RAND_MAX / 2)/dist ,(rand() - RAND_MAX / 2)/dist },
	//		{ 0,0,0 },
	//		{ 1,1,1 },
	//		{ rand()/(float)RAND_MAX,rand() / (float)RAND_MAX,rand() / (float)RAND_MAX }
	//	);
	//	index_++;
	//}





	//exit(1);

	//for (int i = 0; i < 100; i++) {
	//	Render::addInstance("Cube",
	//		//{ index_ / 10.0,0,0 },
	//		{ (rand() - RAND_MAX / 2) * dist,(rand() - RAND_MAX / 2) * dist ,(rand() - RAND_MAX / 2) * dist },
	//		{ 0,0,0 },
	//		{ 1,1,1 },
	//		{ rand() / (float)RAND_MAX,rand() / (float)RAND_MAX,rand() / (float)RAND_MAX }
	//	);
	//}

	int F = 0;
	int T = 0;

	unsigned int debug;
	glGenBuffers(1, &debug);
	glBindBuffer(GL_ARRAY_BUFFER, debug);
	float width = .25;
	float data[] = {
		// x , y , z , u , v 
		-width/2,-1,0, 0,0,
		-width/2,1,0,  0,1,
		width/2,-1,0,  1,0,

		-width/2,1,0, 0,1,
		width/2,1,0,  1,1,
		width/2,-1,0,  1,0

	};



	glBindBuffer(GL_ARRAY_BUFFER, debug);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	const int bars = 1;
	unsigned int index;
	glGenBuffers(1, &index);
	float indexes[bars];
	for (int i = 0; i < bars; i++) {
		indexes[i] = -1 + width / 2 * (i + 1);
	}
	glBindBuffer(GL_ARRAY_BUFFER, index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
	//float indexes[] = { -1+width/2,-1+width, -1+width*3/2};

	unsigned int height;
	glGenBuffers(1, &height);
	float heights[bars];
	for (int i = 0; i < bars; i++) {
		heights[i] = 0;
	}
	glBindBuffer(GL_ARRAY_BUFFER, height);
	glBufferData(GL_ARRAY_BUFFER, sizeof(heights), heights, GL_STATIC_DRAW);
	//float heights[] = {-.5,-1,-.5};
		
	float lightLocx = 0;
	
	while (Render::keepWindow) {
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		glVertexAttribDivisor(3, 1);



		glEnable(GL_CULL_FACE);
		glUseProgram(Render::program1);
		float currentFrame = glfwGetTime();
		Render::dt = currentFrame - Render::lastFrame;
		Render::lastFrame = currentFrame;
		Render::camera.speed = Render::camera.baseSpeed * Render::dt;
		glUniformMatrix4fv(glGetUniformLocation(Render::program1, "model"), 1, GL_FALSE, glm::value_ptr(Render::model));
		glUniformMatrix4fv(glGetUniformLocation(Render::program1, "view"), 1, GL_FALSE, glm::value_ptr(Render::view));
		glUniformMatrix4fv(glGetUniformLocation(Render::program1, "projection"), 1, GL_FALSE, glm::value_ptr(Render::projection));
		glUniform3fv(glGetUniformLocation(Render::program1, "camPos"), 1, glm::value_ptr(Render::camera.cameraPos));
		glUniform3fv(glGetUniformLocation(Render::program1, "camFront"), 1, glm::value_ptr(Render::camera.cameraFront));

		glUniform3f(glGetUniformLocation(Render::program1, "light"), sin(lightLocx),0,cos(lightLocx));

		// update the values of the sphere
		//Render::objects[1]->insts->edit(key, { sin(lightLocx),0,cos(lightLocx)}, {0,0,0}, {1,1,1}, {1,1,1});

		lightLocx += .00;

		Render::camera.translate(Render::left, Render::right, Render::up, Render::down, Render::forward, Render::backward);
		Render::view = glm::lookAt(Render::camera.cameraPos, Render::camera.cameraPos + Render::camera.cameraFront, Render::camera.cameraUp);
		Render::projection = glm::perspective(glm::radians(Render::camera.fov), (float)(800.0 / 800.0), .01f, 1000.0f);

		Render::draw();
		//for(int i=0;i<1;i++)
			//Render::addInstance("Room", { 0,0,0 }, { 1,1,1 }, { 1,1,1 }, { 1,0,1 });



		glBindBuffer(GL_ARRAY_BUFFER, height);
		for (int i = 0; i < bars - 1; i++) {
			heights[i] = heights[i + 1];

		}
		//heights[bars - 1] = -milis / 10;
		heights[bars - 1] = -1.75;

		glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 0, sizeof(float) * (bars), &heights);

		
		glDisable(GL_CULL_FACE);
		glUseProgram(Render::program2);

		glBindBuffer(GL_ARRAY_BUFFER, debug);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(3);
		glVertexAttribDivisor(3, 0);

		// load in indexes

		glBindBuffer(GL_ARRAY_BUFFER, index);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
		glVertexAttribDivisor(1, 1);

		glBindBuffer(GL_ARRAY_BUFFER, height);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
		glVertexAttribDivisor(2, 1);

		glBindTexture(GL_TEXTURE_2D, texture);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, bars);

		
		glfwSwapBuffers(Render::window);
		glfwPollEvents();
		Render::keepWindow = !glfwWindowShouldClose(Render::window);

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		milis = (end - begin).count() / 1000000.0;
		//std::cout << "Total Time difference = " << milis << "[ms]" << " FPS: " << 1000.0 / milis << "\n";

	}

	

	//exit(1);


	index_ = 0;
	while (Render::keepWindow) {
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();


		// adding objects goes here


		std::chrono::steady_clock::time_point add = std::chrono::steady_clock::now();

		#ifdef DEBUG 
		
		milis = (add - begin).count() / 1000000.0;
		std::cout << "Add time: " << milis << "[ms]\n";
		add = std::chrono::steady_clock::now();

		//std::cout << "Elements: " << Render::objects[0]->insts->da->elements << "\n";

		#endif

		// F will save the data
		if (F != Render::getKey(GLFW_KEY_F) && F == 0) {
			Render::createSave("saves/input.rpo");
		}
		F = Render::getKey(GLFW_KEY_F);

		// B goes into wireframe mode

		// T will add an instance at the current position
		if (T != Render::getKey(GLFW_KEY_T) && T == 0) {
			
			Render::addInstance("Cube", { -Render::camera.cameraPos.x,Render::camera.cameraPos.y,Render::camera.cameraPos.z }, { 0,0,0 }, { 1,1,1 }, { 1,1,1 });
		}
		T = Render::getKey(GLFW_KEY_T);

		if (Render::getKey(GLFW_KEY_LEFT_CONTROL)) {
			Render::camera.baseSpeed = 50;
		}
		else {
			Render::camera.baseSpeed = 10;
		}




	/*
	* 
	* 
	*  This is where the rendering code ends
	* 
	*/

		//Render::renderAll();



		#ifdef DEBUG

		std::chrono::steady_clock::time_point rend = std::chrono::steady_clock::now();
		milis = (rend - add).count() / 1000000.0;
		std::cout << "Render*: " << milis << "[ms]\n";

		#endif


		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		milis = (end - begin).count() / 1000000.0;
		//time.push_back(milis);
		std::cout << "Total Time difference = " << milis << "[ms]" << " FPS: " << 1000.0/milis << "\n";
		
		//std::cout << "\n";

	}

	Render::exit();



	return 0;
}