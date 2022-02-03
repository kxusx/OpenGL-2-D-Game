#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "game.h"
#include "resource_manager.h"
#define GLFW_INCLUDE_GLCOREARB

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action,
				  int mode);

// The Width of the screen
const unsigned int SCREEN_WIDTH = 800;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

void levelGenerator() {
	srand(time(NULL));
	ofstream myfile;
	int noOfEnemies = 4, totalCoins = 5;
    int row =5, col =5;
	int array[row][col] ;

	myfile.open("/Users/khushpatel/Documents/Computer Graphics/CG-Assignment1/levels/one.lvl",ios::out);
    

	for (int i = 0; i < row; i++) {
		int r1 = rand() % col;
		int r2 = rand() % col;
		array[i][r1] = 1;
		array[i][r2] = 1;
	}
	for (int i = 0; i < totalCoins; i++) {
		int x = rand() % row;
		int y = rand() % col;
		array[x][y] = 2;
	}
	for (int i = 0; i < noOfEnemies; i++) {
		int x = rand() % row;
		int y = rand() % col;
		array[x][y] = 3;
	}
	// Put 1 at all the walls of the matrix
	// for (int i = 0; i < row; i++) {
	// 	array[i][0] = 1;
	// 	array[i][col-1] = 1;
	// }
	// for (int i = 0; i < col; i++) {
	// 	array[0][i] = 1;
	// 	array[row-1][i] = 1;
	// }

	// std::fprintf(fp,"%d\n",totalCoins);
	if (myfile.is_open()) {
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				myfile << array[i][j] << " ";
			}
			myfile << "\n";
		}
	} else {
		cout << "NOT OPEN";
	}
     myfile.close();
}

int main(int argc, char* argv[]) {
	levelGenerator();
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, false);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
										  "Breakout", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// OpenGL configuration
	// --------------------
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// initialize game
	// ---------------
	Breakout.Init();

	// deltaTime variables
	// -------------------
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		// calculate delta time
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// manage user input
		// -----------------
		Breakout.ProcessInput(deltaTime);

		// update game state
		// -----------------
		Breakout.Update(deltaTime);

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Breakout.Render();

		glfwSwapBuffers(window);
	}

	// delete all resources as loaded using the resource manager
	// ---------------------------------------------------------
	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action,
				  int mode) {
	// when a user presses the escape key, we set the WindowShouldClose property
	// to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			Breakout.Keys[key] = true;
		else if (action == GLFW_RELEASE)
			Breakout.Keys[key] = false;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width
	// and height will be significantly larger than specified on retina
	// displays.
	glViewport(0, 0, width, height);
}