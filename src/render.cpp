#include <vector>
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <common/shader.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "objloader.h"
#include "chess.h"
#include "render.h"

struct Obj
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::vec3 color;
	GLuint vertexBuffer;
	GLuint normalBuffer;
};

GLFWwindow* window;

void render3D(int windowX, int windowY, Chess::Board* board, void (*frameCallback)())
{
	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(windowX, windowY, "3D Chess", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glewInit();

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID = LoadShaders("shaders/VertexShader.vert", "shaders/FragmentShader.frag");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	std::vector<std::string> objectImports =
	{
		"models/square.obj",
		"models/pawn.obj",
		"models/knight.obj",
		"models/bishop.obj",
		"models/rook.obj",
		"models/queen.obj",
		"models/king.obj"
	};

	std::vector<Obj> objects;

	for (const std::string& s : objectImports)
	{
		Obj object;
		loadOBJ(s, object.vertices, object.normals);
		glGenBuffers(1, &object.vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, object.vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, object.vertices.size() * sizeof(glm::vec3), &object.vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &object.normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, object.normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, object.normals.size() * sizeof(glm::vec3), &object.normals[0], GL_STATIC_DRAW);

		objects.push_back(object);
	}

	glUseProgram(programID);
	GLuint LightPositionID = glGetUniformLocation(programID, "LightPosition_worldspace");
	GLuint LightColorID = glGetUniformLocation(programID, "LightColor");
	GLuint LightPowerID = glGetUniformLocation(programID, "LightPower");
	GLuint ObjectColorID = glGetUniformLocation(programID, "ObjectColor");

	static float bgColor[3] = { 0.1f, 0.01f, 0.3f };

	static float camPos[3] = { 0.25f, 0.25f, 0.50f };

	static float lightColor[4] = { 1.0f, 1.0f, 1.0f, 0.5f };

	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	do {

		glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);
		glm::mat4 ProjectionMatrix = glm::perspective(45.0f, (float) (windowX / windowY), 0.1f, 100.0f);
		glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(20 * camPos[2] * sin(camPos[0] * glm::pi<float>()) * cos(camPos[1] * 2 * glm::pi<float>()) + 3.5f, 20 * camPos[2] * sin(camPos[0] * glm::pi<float>()) * sin(camPos[1] * 2 * glm::pi<float>()) + 3.5f, 20 * camPos[2] * cos(camPos[0] * glm::pi<float>())), glm::vec3(3.5f, 3.5f, 0), glm::vec3(0, 0, 1));
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		glUniform3f(LightPositionID, 3.5f, 3.5f, 10.0f);

		glUniform3f(LightColorID, lightColor[0], lightColor[1], lightColor[2]);

		glUniform1f(LightPowerID, lightColor[3] * 200);

		glm::vec3 objectColor;
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, objects[0].vertexBuffer);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, objects[0].normalBuffer);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

				ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(x, y, 0));
				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				objectColor = ((x+y)%2) ? glm::vec3(1.00f, 0.66f, 0.37f) : glm::vec3(0.40f, 0.26f, 0.16f);
				glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
				glUniform3f(ObjectColorID, objectColor.r, objectColor.g, objectColor.b);
				glDrawArrays(GL_TRIANGLES, 0, objects[0].vertices.size());

				Chess::Piece* piece = board->getPieceAtPos(x, y);
				Chess::Color pieceColor = piece->getColor();
				Chess::Type modelIndex = piece->getType();

				ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(x, y, 0));

				if (pieceColor == Chess::Color::WHITE)
				{
					glUniform3f(ObjectColorID, 1.000f, 1.000f, 0.941f);
				}
				else if (pieceColor == Chess::Color::BLACK)
				{
					glUniform3f(ObjectColorID, 0.050f, 0.035f, 0.050f);
					ModelMatrix = glm::rotate(ModelMatrix, glm::pi<float>(), glm::vec3(0, 0, 1));
				}

				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, objects[(int) modelIndex].vertexBuffer);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, objects[(int) modelIndex].normalBuffer);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

				MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
				glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

				glDrawArrays(GL_TRIANGLES, 0, objects[(int) modelIndex].vertices.size());

			}
		}

		frameCallback();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Settings");

		ImGui::SliderFloat3("Camera Position", camPos, 0.001f, 1.0f);
		ImGui::ColorEdit3("Background Color", bgColor);
		ImGui::ColorEdit4("Light Color", lightColor);


		ImGui::End();

		ImGui::Begin("Moves");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	for (const Obj& o : objects)
	{
		glDeleteBuffers(1, &o.vertexBuffer);
	}
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	glfwTerminate();

	return;
}