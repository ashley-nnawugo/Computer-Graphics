#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


#include <iostream>

#include "shader.h"
#include "window.h"
#include "ObjParser.h"
#include "camera.h"
#include "ModelViewerCamera.h"
#include "FlyThroughCamera.h"
#include "texture.h"

SCamera Camera;
SCamera FlyCamera;

bool model_cam = true;


//DECLARE POSITION VARIABLES HERE
glm::vec3 oct_pos = glm::vec3(0.f, 0.f, -5.0f);
glm::vec3 origin_pos = glm::vec3(0.f, 0.f, 0.f);
glm::vec3 cam_pos = glm::vec3(0.f, 0.5f, 5.0f);

//glm::vec3 Flycam_pos = glm::vec3(0.f, 3.f, 5.f);

glm::vec3 lightDirection = glm::vec3(0.1f, -.81f, -.61f);
glm::vec3 lightColour = glm::vec3(1.f, 1.f, 1.f);
glm::vec3 lightPos = glm::vec3(.1f, .5f, 7.f);

glm::vec3 lightSpotPos = glm::vec3(0.f, 1.f, 1.4f);
glm::vec3 lightSpotDirection = glm::vec3(0.2f, -1.f, -.8f);


float missile_launch = 0.7f;
float move = 0.f;

float ground[] = 
{
	-7.f, 0.f, 7.f,		0.f, 1.f,		0.f, 1.f, 0.f,
	7.f, 0.f, 7.f,		1.f, 1.f,		0.f, 1.f, 0.f,
	7.f, 0.f, -7.f,		1.f, 0.f,		0.f, 1.f, 0.f,

	-7.f, 0.f, 7.f,		0.f, 1.f,		0.f, 1.f, 0.f,
	7.f, 0.f, -7.f,		1.f, 0.f,		0.f, 1.f, 0.f,
	-7.f, 0.f, -7.f,	0.f, 0.f,		0.f, 1.f, 0.f

};

void processKeyboard(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		lightDirection = Camera.Front;
		lightPos = Camera.Position;
	}
	//moves fox towards the center, resets if fox goes past the center
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		if (missile_launch > 0.2f)
		{
			missile_launch -= 0.01f;
		}
		else
		{
			missile_launch = .6f;
		}
	}
	//moves plane left or right
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		if (move < 1.5f)
		{
			move += 0.01f;
		}
		else
		{
			move = 0.f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		if (move > -1.5f)
		{
			move -= 0.01f;
		}
		else
		{
			move = 0.f;
		}
	}


	bool cam_changed = false;
	float x = 0.f, y = 0.f;

	if (model_cam == TRUE) {


		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			x = 1.f;
			y = 0.f;
			cam_changed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			x = 0.f;
			y = -1.f;
			cam_changed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			x = 0.f;
			y = 1.f;
			cam_changed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			x = -1.f;
			y = 0.f;
			cam_changed = true;
		}

		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		{
			cam_dist += 0.1;
			cam_changed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			cam_dist -= 0.1;
			cam_changed = true;
		}
		
	}

	if (cam_changed)
		{
			MoveAndOrientCamera(Camera, origin_pos, cam_dist, x, y);
		}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{	
		model_cam = false;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		model_cam = true;
	}

	if(!model_cam) 
	{
		
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			MoveCamera(FlyCamera, SCamera::FORWARD);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			MoveCamera(FlyCamera, SCamera::BACKWARD);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			MoveCamera(FlyCamera, SCamera::LEFT);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			MoveCamera(FlyCamera, SCamera::RIGHT);
	}
}

bool firstMouse = true;
float prevMouseX;
float prevMouseY;

void processMouse(GLFWwindow* window, double x, double y)
{
	float Sensitivitiy = 0.1f;
	x *= Sensitivitiy;
	y *= Sensitivitiy;
	if (firstMouse)
	{
		prevMouseX = x;
		prevMouseY = y;
		firstMouse = false;
	}

	float dX = x - prevMouseX;
	float dY = y - prevMouseY;

	prevMouseX = x;
	prevMouseY = y ;
	OrientCamera(FlyCamera, dX, dY);
}



int main(int argc, char** argv)
{
	GLFWwindow* window = CreateGLFWWindow(800, 600, "Assesssment 3");
	InitCamera(Camera, 100, 30);
	InitCamera(FlyCamera, 100, 50);

	//FlyCamera.Position = Flycam_pos;
	MoveAndOrientCamera(Camera, origin_pos, cam_dist, 0.f, 0.f);

	float f, g, h;
	f = FlyCamera.Position.x;
	g = FlyCamera.Position.y;
	h = FlyCamera.Position.z;
	printf("%f,%f,%f", f, g, h);

	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	glfwSetCursorPosCallback(window, processMouse);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	
	//OBJECT 1 car
	int indexes0 = 0;
	int fs0 = 0;
	float* Verts0 = (float*)malloc(sizeof(float*) * (3 * 10000));
	Verts0 = read_obj_file("Obj/Car/militarycar.obj", Verts0);
	values(&indexes0, &fs0);
	//face = faces(face);
	//printf("%d", face);

	free(Verts0);
	float* Vertices0 = (float*)malloc(sizeof(float*) * indexes0);
	Vertices0 = read_obj_file("Obj/Car/militarycar.obj", Vertices0);
	//for (int i = 0; i < (indexes0); i++)
	//{
		//printf("%f\n", Vertices0[i]);
	//}
	float sizeVerts0 = (float)fs0 * 24 * sizeof(float);

	// OBJECT 2 plane
	int indexes1 = 0;
	int fs1 = 0;
	float* Verts1 = (float*)malloc(sizeof(float*) * (3 * 10000));
	Verts1 = read_obj_file("Obj/Plane/plane.obj", Verts1);

	values(&indexes1, &fs1);
	//face = faces(face);
	//printf("%d", face);

	free(Verts1);
	float* Vertices1 = (float*)malloc(sizeof(float*) * indexes1);
	Vertices1 = read_obj_file("Obj/Plane/plane.obj", Vertices1);
	
	//for (int f = 0; f < indexes1; f++)
	//{
		//printf("%f\n", Vertices1[f]);
	//}
	float sizeVerts1 = (float)fs1 * 24 * sizeof(float);

	// OBJECT 3 building
	int indexes2 = 0;
	int fs2 = 0;
	float* Verts2 = (float*)malloc(sizeof(float*) * (3 * 10000));
	Verts2 = read_obj_file("Obj/Building/building.obj", Verts2);

	values(&indexes2, &fs2);
	free(Verts2);
	float* Vertices2 = (float*)malloc(sizeof(float*) * indexes2);
	Vertices2 = read_obj_file("Obj/Building/building.obj", Vertices2);

	float sizeVerts2 = (float)fs2 * 24 * sizeof(float);

	//OBJECT 4 missile
	int indexes3 = 0;
	int fs3 = 0;
	float* Verts3 = (float*)malloc(sizeof(float*) * (3 * 10000));
	Verts3 = read_obj_file("Obj/Missile/missile.obj", Verts3);

	values(&indexes3, &fs3);
	free(Verts3);
	float* Vertices3 = (float*)malloc(sizeof(float*) * indexes3);
	Vertices3 = read_obj_file("Obj/Missile/missile.obj", Vertices3);

	float sizeVerts3 = (float)fs3 * 24 * sizeof(float);

	//OBJECT 5  flag
	int indexes4 = 0;
	int fs4 = 0;
	float* Verts4 = (float*)malloc(sizeof(float*) * (3 * 10000));
	Verts4 = read_obj_file("Obj/Flag/flag.obj", Verts4);

	values(&indexes4, &fs4);
	free(Verts4);
	float* Vertices4 = (float*)malloc(sizeof(float*) * indexes4);
	Vertices4 = read_obj_file("Obj/Flag/flag.obj", Vertices4);

	float sizeVerts4 = (float)fs4 * 24 * sizeof(float);

	//OBJECT 6 cube
	int indexes5 = 0;
	int fs5 = 0;
	float* Verts5 = (float*)malloc(sizeof(float*) * (3 * 10000));
	Verts5 = read_obj_file("Obj/Cube/cubes1.obj", Verts5);

	values(&indexes5, &fs5);
	free(Verts5);
	float* Vertices5 = (float*)malloc(sizeof(float*) * indexes5);
	Vertices5 = read_obj_file("Obj/Cube/cubes1.obj", Vertices5);

	float sizeVerts5 = (float)fs5 * 24 * sizeof(float);

	//Object 7 fox
	int indexes6 = 0;
	int fs6 = 0;
	float* Verts6 = (float*)malloc(sizeof(float*) * (3 * 10000));
	Verts6 = read_obj_file("Obj/Fox/foxpet.obj", Verts6);

	values(&indexes6, &fs6);
	free(Verts6);
	float* Vertices6 = (float*)malloc(sizeof(float*) * indexes6);
	Vertices6 = read_obj_file("Obj/Fox/foxpet.obj", Vertices6);

	float sizeVerts6 = (float)fs6 * 24 * sizeof(float);

	unsigned int shaderProgram = LoadShader("texture.vert", "texture.frag");
	

	//car texture
	GLuint texture0 = setup_texture("Obj/Car/car_texture.bmp");
	
	//plane texture
	GLuint texture1 = setup_texture("Obj/Plane/planetex.bmp");

	//building texture
	GLuint texture2 = setup_texture("Obj/Building/Building_tex.bmp");

	//missile texture
	GLuint texture3 = setup_texture("Obj/Missile/missile_tex.bmp");
	
	//flag texture
	GLuint texture4 = setup_texture("Obj/Flag/flag.bmp");

	//cube texture
	GLuint texture5 = setup_texture("Obj/Cube/sanddune.bmp");

	//fox texture
	GLuint texture6 = setup_texture("Obj/Fox/fox_texture.bmp");

	//changes with number of objects
	unsigned int VAO[7];
	glGenVertexArrays(7, VAO);
	unsigned int VBO[7];
	glGenBuffers(7, VBO);

	//car
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts0, Vertices0, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//plane
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts1, Vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//building
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts2, Vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//missile
	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts3, Vertices3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//flag
	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts4, Vertices4, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	//cube
	glBindVertexArray(VAO[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	//fox
	glBindVertexArray(VAO[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts6, Vertices6, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//

	//ENABLE DEPTH TEST HERE
	glEnable(GL_DEPTH_TEST);

	glUseProgram(shaderProgram);
	printf("%d", fs0);


	while (!glfwWindowShouldClose(window))
	{
		processKeyboard(window);

		glClearColor(0.5f, 0.5f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glUniform3f(glGetUniformLocation(shaderProgram, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightSpotDirection"), lightSpotDirection.x, lightSpotDirection.y, lightSpotDirection.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightColour"), lightColour.x, lightColour.y, lightColour.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightSpotPos"), lightSpotPos.x, lightSpotPos.y, lightSpotPos.z);
		if(model_cam)
			glUniform3f(glGetUniformLocation(shaderProgram, "camPos"), Camera.Position.x, Camera.Position.y, Camera.Position.z);
		if(!model_cam)
			glUniform3f(glGetUniformLocation(shaderProgram, "camPos"), FlyCamera.Position.x, FlyCamera.Position.y, FlyCamera.Position.z);


		//Object 1 car
		// MODEL & Binding textures
		
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(0.f, 0.25f, 0.5f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, texture0);
		glUseProgram(shaderProgram);

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, fs0);
		
		//OBject 2 plane
		// MODEL  & Binding textures

		glm::mat4 model1 = glm::mat4(1.f);
		model1 = glm::translate(model1, glm::vec3(move, 1.f, 0.f));
		model1 = glm::scale(model1, glm::vec3(0.15f, 0.15f, 0.15f));
		model1 = glm::rotate(model1, -.2f, glm::vec3(1.f, 0.f, 0.f));
		model1 = glm::rotate(model1, (float)glfwGetTime() * 3 , glm::vec3(0.f, 0.f, 1.f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model1));

		glBindTexture(GL_TEXTURE_2D, texture1);
		glUseProgram(shaderProgram);

		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, fs1);

		//Object 3  building
		// MODEL  & Binding textures

		glm::mat4 model2 = glm::mat4(1.f);
		model2 = glm::translate(model2, glm::vec3(1.f, 0.f, -1.5f));
		model2 = glm::scale(model2, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));

		glBindTexture(GL_TEXTURE_2D, texture2);
		glUseProgram(shaderProgram);

		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES, 0, fs2);

		//Object 4 missile
		// MODEL & Binding textures
		glm::mat4 model3 = glm::mat4(1.f);
		model3 = glm::translate(model3, glm::vec3(0.f, missile_launch, 0.f));
		model3 = glm::scale(model3, glm::vec3(0.07f, 0.07f, 0.07f));
		model3 = glm::rotate(model3, 9.f, glm::vec3(1.f, 0.f, 0.f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model3));

		glBindTexture(GL_TEXTURE_2D, texture3);
		glUseProgram(shaderProgram);

		glBindVertexArray(VAO[3]);
		glDrawArrays(GL_TRIANGLES, 0, fs3);

		//Object 5 flag
		// MODEL  & Binding textures
		glm::mat4 model4 = glm::mat4(1.f);
		model4 = glm::translate(model4, glm::vec3(.7f, .5f, 1.f));
		model4 = glm::scale(model4, glm::vec3(0.01f, 0.01f, 0.01f));
		model4 = glm::rotate(model4, 0.8f, glm::vec3(0.f, 0.f, 1.f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model4));

		glBindTexture(GL_TEXTURE_2D, texture4);
		glUseProgram(shaderProgram);

		glBindVertexArray(VAO[4]);
		glDrawArrays(GL_TRIANGLES, 0, fs4);
		
		//Object6  background
		// MODEL & Binding textures
		glm::mat4 model5 = glm::mat4(1.f);
		model5 = glm::translate(model5, glm::vec3(0.f, .0f, 0.f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model5));

		glBindTexture(GL_TEXTURE_2D, texture5);
		glUseProgram(shaderProgram);

		glBindVertexArray(VAO[5]);
		glDrawArrays(GL_TRIANGLES, 0, fs5);

		//Object6 fox
		// MODEL & Binding textures
		glm::mat4 model6 = glm::mat4(1.f);
		model6 = glm::translate(model6, glm::vec3(-1.0f, .0f, 0.f));
		model6 = glm::scale(model6, glm::vec3(.08f, .08f, .08f));
		model6 = glm::rotate(model6, 1.f, glm::vec3(0.f, 1.f, 0.f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model6));

		glm::mat4 view = glm::mat4(1.f);
		glm::mat4 view2 = glm::mat4(1.f);
		if (model_cam) {
			view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		}	
		if (!model_cam) {
			view2 = glm::lookAt(FlyCamera.Position, FlyCamera.Position + FlyCamera.Front, FlyCamera.Up);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view2));

		}	

		glm::mat4 projection = glm::mat4(1.f);
		projection = glm::perspective(glm::radians(45.f), (float)800 / (float)600, 1.f, 10.f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindTexture(GL_TEXTURE_2D, texture6);
		glUseProgram(shaderProgram);

		glBindVertexArray(VAO[6]);
		glDrawArrays(GL_TRIANGLES, 0, fs6);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	//freeing memory 
	free(Vertices0);
	free(Vertices1);
	free(Vertices2);
	free(Vertices3);
	free(Vertices4);
	free(Vertices5);
	free(Vertices6);
	return 0;
}
