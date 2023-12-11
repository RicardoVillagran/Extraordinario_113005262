#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 1600, HEIGHT = 1200;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-90.0f, 15.0f, 20.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(-95.0f, 0.35f, -45.0f);
bool active;

//Animación del toroide1
float movKitY1 = 0.0;
float movKitX1 = 0.0;
//Animación del toroide2
float movKitY2 = 0.0;
float movKitX2 = 0.0;
//Animación del toroide3
float movKitY3 = 0.0;
float movKitX3 = 0.0;

bool circuito = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;
bool recorrido6 = false;
bool recorrido7 = false;
bool recorrido8 = false;
bool recorrido9 = false;
bool recorrido10 = false;
bool recorrido11 = false;
bool recorrido12 = false;
bool recorrido13 = false;
bool recorrido14 = false;
bool recorrido15 = false;
bool recorrido16 = false;
bool recorrido17 = false;
bool recorrido18 = false;
bool recorrido19 = false;
bool recorrido20 = false;
bool recorrido21 = false;
bool recorrido22 = false;
bool recorrido23 = false;
bool recorrido24 = false;

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Animación Compleja", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");

	Model Toroide1((char*)"modelos/toroide1.obj");
	Model Toroide2((char*)"modelos/toroide2.obj");
	Model Toroide3((char*)"modelos/toroide3.obj");
	Model base1((char*)"modelos/base1.obj");
	Model base2((char*)"modelos/base2.obj");
	Model base3((char*)"modelos/base3.obj");
	Model palo1((char*)"modelos/paloBase1.obj");
	Model palo2((char*)"modelos/paloBase2.obj");
	Model palo3((char*)"modelos/paloBase3.obj");
	Model Piso((char*)"modelos/Piso.obj");
	Model plano((char*)"modelos/plano.obj");

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);



		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);

		//Carga de modelo 
		//Toroide 1
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX1, movKitY1, 0));
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Toroide1.Draw(lightingShader);

		//Toroide2
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX2, movKitY2, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Toroide2.Draw(lightingShader);

		//Toroide3
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX3, movKitY3, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Toroide3.Draw(lightingShader);

		//Palo1
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(0, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		palo1.Draw(lightingShader);

		//Base1
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(0, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		base1.Draw(lightingShader);

		//Palo2
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(0, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		palo2.Draw(lightingShader);

		//Base2
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(0, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		base2.Draw(lightingShader);

		//Palo3
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(0, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		palo3.Draw(lightingShader);

		//Base3
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(0, 0, 0));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		base3.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(40, 10, 8));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		plano.Draw(lightingShader);

		//Piso
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}

void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, 2 * deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, 2 * deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, 2 * deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, 2 * deltaTime);
	}
	if (keys[GLFW_KEY_I])
	{
		circuito = true;
	}
	if (keys[GLFW_KEY_O])
	{
		recorrido22 = true;
	}

}


void animacion()
{
	//Movimiento del coche
	if (circuito)
	{
		//paso1	
		if (recorrido1)
		{
			movKitY3 += 0.05f;
			if (movKitY3 > 10)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			movKitX3 += 0.05f;
			if (movKitX3 > 24.85)
			{
				recorrido2 = false;
				recorrido3 = true;

			}
		}
		if (recorrido3)
		{
			movKitY3 -= 0.05f;
			if (movKitY3 < -4.60f)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}
		//paso2
		if (recorrido4)
		{
			movKitY2 += 0.05f;
			if (movKitY2 > 10)
			{
				recorrido4 = false;
				recorrido5 = true;
			}
		}
		if (recorrido5)
		{
			movKitX2 += 0.05f;
			if (movKitX2 > 12.425)
			{
				recorrido5 = false;
				recorrido6 = true;
			}
		}

		if (recorrido6)
		{
			movKitY2 -= 0.05f;
			if (movKitY2 < -2.55)
			{
				recorrido6 = false;
				recorrido7 = true;
			}
		}
		//paso3
		if (recorrido7)
		{
			movKitY3 += 0.05f;
			if (movKitY3 > 10)
			{
				recorrido7 = false;
				recorrido8 = true;
			}
		}
		if (recorrido8)
		{
			movKitX3 -= 0.05f;
			if (movKitX3 < 12.425)
			{
				recorrido8 = false;
				recorrido9 = true;
			}
		}
		if (recorrido9)
		{
			movKitY3 -= 0.05f;
			if (movKitY3 < -2.55)
			{
				recorrido9 = false;
				recorrido10 = true;
			}
		}
		//paso4
		if (recorrido10)

		{
			movKitY1 += 0.05f;
			if (movKitY1 > 10)
			{
				recorrido10 = false;
				recorrido11 = true;
			}
		}
		if (recorrido11)

		{
			movKitX1 += 0.05f;
			if (movKitX1 > 24.85)
			{
				recorrido11 = false;
				recorrido12 = true;
			}
		}
		if (recorrido12)

		{
			movKitY1 -= 0.05f;
			if (movKitY1 < 0.1)
			{
				recorrido12 = false;
				recorrido13 = true;
			}
		}
		//paso5
		if (recorrido13)

		{
			movKitY3 += 0.05f;
			if (movKitY3 > 10)
			{
				recorrido13 = false;
				recorrido14 = true;
			}
		}
		if (recorrido14)

		{
			movKitX3 -= 0.05f;
			if (movKitX3 < 0)
			{
				recorrido14 = false;
				recorrido15 = true;
			}
		}
		if (recorrido15)

		{
			movKitY3 -= 0.05f;
			if (movKitY3 < -4.6)
			{
				recorrido15 = false;
				recorrido16 = true;
			}
		}
		//paso 6
		if (recorrido16)

		{
			movKitY2 += 0.05f;
			if (movKitY2 > 10)
			{
				recorrido16 = false;
				recorrido17 = true;
			}
		}
		if (recorrido17)

		{
			movKitX2 += 0.05f;
			if (movKitX2 > 24.85)
			{
				recorrido17 = false;
				recorrido18 = true;
			}
		}
		if (recorrido18)

		{
			movKitY2 -= 0.05f;
			if (movKitY2 < 0.1)
			{
				recorrido18 = false;
				recorrido19 = true;
			}
		}
		//paso7
		if (recorrido19)

		{
			movKitY3 += 0.05f;
			if (movKitY3 > 10)
			{
				recorrido19 = false;
				recorrido20 = true;
			}
		}
		if (recorrido20)

		{
			movKitX3 += 0.05f;
			if (movKitX3 > 24.85)
			{
				recorrido20 = false;
				recorrido21 = true;
			}
		}
		if (recorrido21)

		{
			movKitY3 -= 0.05f;
			if (movKitY3 < 0)
			{
				recorrido21 = false;
				recorrido22 = false;
			}
		}
		if (recorrido22)

		{
			movKitY1 += 0.05f;
			movKitY2 += 0.05f;
			movKitY3 += 0.05f;
			if (movKitY3 > 10)
			{
				recorrido22 = false;
				recorrido23 = true;
			}
		}
		if (recorrido23)

		{
			movKitX1 -= 0.05f;
			movKitX2 -= 0.05f;
			movKitX3 -= 0.05f;
			if (movKitX3 < 0)
			{
				recorrido23 = false;
				recorrido24 = true;
			}
		}
		if (recorrido24)

		{
			movKitY1 -= 0.05f;
			movKitY2 -= 0.05f;
			movKitY3 -= 0.05f;
			if (movKitY3 < 0)
			{
				recorrido24 = false;
				recorrido1 = true;
			}
		}
	}
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

