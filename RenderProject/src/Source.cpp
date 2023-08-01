
#include <Render/Render.h>
#include <chrono>


/*
*  Use WASD to move, mouse to look and B to change wireframe mode
*/


#include <Render/utils/FillerArray.h>

float test_ = 0.0;

void update() {
	glBindBuffer(GL_ARRAY_BUFFER, Render::allBuffer);
	FillerArray::Element test = { 1,{1,test_,1},{0,0,0},{1,1,1},{.5,.5,.5} };
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(FillerArray::Element), &test);
	test_ += .01;
}

void draw() {

	float milis;

	for (Render::object o : Render::objects) {
		int elements = o.insts->da->elements;
		std::cout << "Elements: " << elements << "/" << o.insts->da->size << "\n";
		//std::cout << "Elements: " << elements << "\n";
		//int elements = o.insts->table->elements;
		//std::cout << "Elements: " << elements <<" Size: " << o.insts->table->size  << " load: " << o.insts->table->load << "\n";
		if (elements <= 0) {
			continue;
		}



		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		// fill the position buffer and send the data to the gpu
		glBindBuffer(GL_ARRAY_BUFFER, Render::positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * o.m.vertices.size(), &o.m.vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		std::chrono::steady_clock::time_point posBuff = std::chrono::steady_clock::now();
		milis = (posBuff - begin).count() / 1000000.0;
		std::cout << "       Position Buffer: " << milis << "[ms]\n";
		posBuff = std::chrono::steady_clock::now();

		// load in the buffer of all data
		glBindBuffer(GL_ARRAY_BUFFER, o.insts->buffer);
		std::cout << "Buffer Number: " << o.insts->buffer << "\n";
		// we are changing GL_STATIC_DRAW to GL_DYNAMIC_DRAW due to the high refresh rate
		//glBufferData(GL_ARRAY_BUFFER, sizeof(FillerArray::Element) * elements, o.insts->da->arr, GL_DYNAMIC_DRAW);

		std::chrono::steady_clock::time_point instBuff = std::chrono::steady_clock::now();
		milis = (instBuff - posBuff).count() / 1000000.0;
		std::cout << "       Instance Buffer: " << milis << "[ms]\n";
		instBuff = std::chrono::steady_clock::now();


		// this is translation
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int)));
		// this is rotation
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(glm::vec3) + sizeof(int)));
		// this is for scalation
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int) + 2 * sizeof(glm::vec3)));
		// this is for color
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(FillerArray::Element), (void*)(sizeof(int) + 3 * sizeof(glm::vec3)));

		std::chrono::steady_clock::time_point vertexattr = std::chrono::steady_clock::now();
		milis = (vertexattr - instBuff).count() / 1000000.0;
		std::cout << "         Vertex Attrib: " << milis << "[ms]\n";
		vertexattr = std::chrono::steady_clock::now();


		// give the index buffer to the gpu
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Render::EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * o.m.indices.size(), &o.m.indices[0], GL_STATIC_DRAW);

		std::chrono::steady_clock::time_point ebo = std::chrono::steady_clock::now();
		milis = (ebo - vertexattr).count() / 1000000.0;
		std::cout << "              EBO bind: " << milis << "[ms]\n";
		ebo = std::chrono::steady_clock::now();



		glDrawElementsInstanced(GL_TRIANGLES, o.m.indices.size() - 3, GL_UNSIGNED_INT, 0, elements);

		std::chrono::steady_clock::time_point drawelements = std::chrono::steady_clock::now();
		milis = (drawelements - ebo).count() / 1000000.0;
		std::cout << "         Draw Elements: " << milis << "[ms]\n";


	}
}


int main() {



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
	Render::init();	
	Render::camera.baseSpeed = 100;

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	

	Render::addModel("assets/cube2.obj", "Cube");
	//Render::addModel("assets/sphere.obj", "Cube");
	//Render::addModel("assets/monkey.obj", "Monkey");


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

	long key = Render::addInstance("Cube",1,
		{ 110,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,.75,.01 }

	);

	Render::addInstance("Cube",2,
		{ 10,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ 0,.75,0 }

	);

	Render::addInstance("Cube",3,
		{ 20,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,0,0 }

	);

	Render::addInstance("Cube",4,
		{ -10,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,.75,0 }

	);

	Render::addInstance("Cube",5,
		{ -20,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ 0,.75,0 }

	);

	Render::addInstance("Cube",6,
		{ 0,10,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,0,0 }

	);

	Render::addInstance("Cube",7,
		{ 0,20,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,.75,0 }

	);

	Render::addInstance("Cube",8,
		{ 0,-10,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ 0,.75,0 }

	);

	Render::addInstance("Cube",9,
		{ 0,-20,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,0,0 }

	);

	Render::addInstance("Cube",10,
		{ 0,0,10 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,0,0 }

	);

	Render::addInstance("Cube",11,
		{ 0,0,20 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,.75,0 }

	);

	Render::addInstance("Cube",12,
		{ 0,0,-10 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ 0,.75,0 }

	);

	Render::addInstance("Cube",13,
		{ 0,0,-20 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,0,0 }

	);

	//Render::removeInstances("Cube");
	//Render::removeInstances("Cube");
	//Render::addInstance("Monkey", {1,1,1}, {90,1,1}, {2,1,4});
	//Render::removeAllInstances();

	//std::vector<float> time(10000);

	int index_ = 14;

	float milis;
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

	Render::object o = Render::objects[0];
	int elements = o.insts->da->elements;


	

	// load in the buffer of all data
	glBindBuffer(GL_ARRAY_BUFFER, Render::allBuffer);
	// we are changing GL_STATIC_DRAW to GL_DYNAMIC_DRAW due to the high refresh rate
	//glBufferData(GL_ARRAY_BUFFER, sizeof(FillerArray::Element)* o.insts->da->size, o.insts->da->arr, GL_DYNAMIC_DRAW);
	std::cout << o.insts->da->elements << "\n";


	FillerArray::Element temp = o.insts->get(key);
	std::cout << temp.key << " " << temp.translations.x << "\n";


	//exit(1);

	for (int i = 0; i < 1000000; i++) {
		Render::addInstance("Cube", index_,
			//{ index_ / 10.0,0,0 },
			{ (rand() - RAND_MAX / 2) * dist,(rand() - RAND_MAX / 2) * dist ,(rand() - RAND_MAX / 2) * dist },
			{ 0,0,0 },
			{ 1,1,1 },
			{ rand() / (float)RAND_MAX,rand() / (float)RAND_MAX,rand() / (float)RAND_MAX }
		);
	}


	index_ = 0;
	while (Render::keepWindow) {
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		//Render::addInstance("Monkey", { 1,1,1 }, { 90,1,1 }, { 2,1,4 }, {.5,.5,.8});
		//for (int i = 0; i < 100;i++) {
		//	Render::addInstance("Cube", index_,
		//		{ index_ / 10.0,0,0 },
		//		{ 0,0,0 },
		//		{ 1,1,1 },
		//		{ .75,0,0 }
		//	);
		//	index_++;
		//}

		//update();

		

		
		//index_++;


	std::chrono::steady_clock::time_point add = std::chrono::steady_clock::now();
	milis = (add - begin).count() / 1000000.0;
	std::cout << "Add time: " << milis << "[ms]\n";
	add = std::chrono::steady_clock::now();

		std::cout << "Elements: " << Render::objects[0].insts->da->elements << "\n";
		

	/*
	* 
	* This is where the rendering code goes
	* 
	* 
	*/
	//float milis;
	// this is the start time 
	//std::chrono::steady_clock::time_point begin_ = std::chrono::steady_clock::now();

	//Render::keepWindow = !glfwWindowShouldClose(Render::window);
	//float currentFrame = glfwGetTime();
	//Render::dt = currentFrame - Render::lastFrame;
	//Render::lastFrame = currentFrame;
	//Render::camera.speed = Render::camera.baseSpeed * Render::dt;
	//glClearColor(0, 0, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glUniformMatrix4fv(glGetUniformLocation(Render::program1, "model"), 1, GL_FALSE, glm::value_ptr(Render::model));
	//glUniformMatrix4fv(glGetUniformLocation(Render::program1, "view"), 1, GL_FALSE, glm::value_ptr(Render::view));
	//glUniformMatrix4fv(glGetUniformLocation(Render::program1, "projection"), 1, GL_FALSE, glm::value_ptr(Render::projection));
	//glUniform3fv(glGetUniformLocation(Render::program1, "camPos"), 1, glm::value_ptr(Render::camera.cameraPos));
	//glUniform3fv(glGetUniformLocation(Render::program1, "camFront"), 1, glm::value_ptr(Render::camera.cameraFront));

	//Render::camera.translate(Render::left, Render::right, Render::up, Render::down, Render::forward, Render::backward);
	//Render::view = glm::lookAt(Render::camera.cameraPos, Render::camera.cameraPos + Render::camera.cameraFront, Render::camera.cameraUp);
	//Render::projection = glm::perspective(glm::radians(Render::camera.fov), (float)(800.0 / 800.0), .01f, 1000.0f);

	//std::chrono::steady_clock::time_point uniforms = std::chrono::steady_clock::now();
	//milis = (uniforms - add).count() / 1000000.0;
	//std::cout << "   Uniforms: " << milis << "[ms]\n";
	//uniforms = std::chrono::steady_clock::now();

	//// Now call the draw function
	//draw();
	////std::cout << "here\n";

	//std::chrono::steady_clock::time_point draw = std::chrono::steady_clock::now();
	//milis = (draw - uniforms).count() / 1000000.0;
	//std::cout << "   Draw: " << milis << "[ms]\n";
	//draw = std::chrono::steady_clock::now();

	//glfwSwapBuffers(Render::window);

	//std::chrono::steady_clock::time_point swap = std::chrono::steady_clock::now();
	//milis = (swap - draw).count() / 1000000.0;
	//std::cout << "   Swap Buffers: " << milis << "[ms]\n";
	//swap = std::chrono::steady_clock::now();

	//glfwPollEvents();

	//std::chrono::steady_clock::time_point poll = std::chrono::steady_clock::now();
	//milis = (poll - swap).count() / 1000000.0;
	//std::cout << "   Poll Events: " << milis << "[ms]\n";
	//poll = std::chrono::steady_clock::now();




	/*
	* 
	* 
	*  This is where the rendering code ends
	* 
	*/

		Render::renderAll();

		std::chrono::steady_clock::time_point rend = std::chrono::steady_clock::now();
		milis = (rend - add).count() / 1000000.0;
		std::cout << "Render*: " << milis << "[ms]\n";



		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		milis = (end - begin).count() / 1000000.0;
		//time.push_back(milis);
		std::cout << "Total Time difference = " << milis << "[ms]" << " FPS: " << 1000.0/milis << "\n";
		
		std::cout << "\n";
		
		//float sum = 0.0;
		//for (int i = 0; i < time.size(); i++) {
		//	sum += time.at(i);
		//}
		//std::cout << "Average: " << sum / time.size() << "\n";
	}

	Render::exit();



	return 0;
}