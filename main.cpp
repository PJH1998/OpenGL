#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

GLuint programID;

GLint dot; //Atrribute
GLint colorLoc; //Atrribute
GLint psize; //Atrribute
GLuint matTrans;
GLuint viewTrans;
GLuint proTrans;
GLuint x_rot;
GLuint y_rot;
GLuint f_rot;
GLuint s_rot;
GLuint t_rot;
GLint light_pos;
GLint light_col;
GLint light_dir;

GLuint VAO;
GLuint Buffers[2];
float wx = 200.0, wy = 200.0;
float wsx = 480.0, wsy = 480.0;

//GLfloat points[2][4] = { 0 };
GLfloat p_color[24][4] = {
	 1.0, 0.0, 0.0, 1.0,
	 1.0, 0.0, 0.0, 1.0,
	 1.0, 0.0, 0.0, 1.0,
	 1.0, 0.0, 0.0, 1.0,

	 0.0, 1.0, 0.0, 1.0,
	 0.0, 1.0, 0.0, 1.0,
	 0.0, 1.0, 0.0, 1.0,
	 0.0, 1.0, 0.0, 1.0,

	 0.0, 0.0, 1.0, 1.0,
	 0.0, 0.0, 1.0, 1.0,
	 0.0, 0.0, 1.0, 1.0,
	 0.0, 0.0, 1.0, 1.0,

	 1.0, 1.0, 0.0, 1.0,
	 1.0, 1.0, 0.0, 1.0,
	 1.0, 1.0, 0.0, 1.0,
	 1.0, 1.0, 0.0, 1.0,

	 1.0, 0.0, 1.0, 1.0,
	 1.0, 0.0, 1.0, 1.0,
	 1.0, 0.0, 1.0, 1.0,
	 1.0, 0.0, 1.0, 1.0,

	 0.0, 1.0, 1.0, 1.0,
	 0.0, 1.0, 1.0, 1.0,
	 0.0, 1.0, 1.0, 1.0,
	 0.0, 1.0, 1.0, 1.0,

};
GLfloat cube[24][4] = {
	0.1, 0.1, 0.1, 1.0, //1
	-0.1, 0.1, 0.1, 1.0, //1
	-0.1, 0.1, -0.1, 1.0, //1
	0.1, 0.1, -0.1, 1.0, //1


	0.1, 0.1, 0.1, 1.0, //2
	0.1, -0.1, 0.1, 1.0, //2
	-0.1, -0.1, 0.1, 1.0, //2
	-0.1, 0.1, 0.1, 1.0, //2

	0.1, 0.1, 0.1, 1.0, //3
	0.1, -0.1, 0.1, 1.0, //3
	0.1, -0.1, -0.1, 1.0, //3
	0.1, 0.1, -0.1, 1.0, //3	

	0.1, 0.1, -0.1, 1.0, //4
	0.1, -0.1, -0.1, 1.0, //4
	-0.1, -0.1, -0.1, 1.0, //4
	-0.1, 0.1, -0.1, 1.0, //4

	-0.1, 0.1, 0.1, 1.0, //5
	-0.1, -0.1, 0.1, 1.0, //5
	-0.1, -0.1, -0.1, 1.0, //5
	-0.1, 0.1, -0.1, 1.0, //5

	-0.1, -0.1, 0.1, 1.0, //6
	0.1, -0.1, 0.1, 1.0, //6
	0.1, -0.1, -0.1, 1.0, //6
	-0.1, -0.1, -0.1, 1.0, //6

};
GLuint indices[] = {
	0, 1, 2, 3,
	4, 5, 6, 7,
	8, 9, 10, 11,
	12, 13, 14, 15,
	16, 17, 18, 19,
	20, 21, 22, 23
};

glm::mat4 viewMatrix(1.0);
glm::mat4 projectionMatrix(1.0);
glm::mat4 x_rotateMatrix(1.0);
glm::mat4 y_rotateMatrix(1.0);
glm::mat4 f_rotateMatrix(1.0);
glm::mat4 s_rotateMatrix(1.0);
glm::mat4 t_rotateMatrix(1.0);

glm::vec3 camPos(1.0, 1.5, 1.0);
glm::vec3 camFrontDir(-0.7, -1.2, -1.0);
glm::vec3 worldUpDir(0.0, 1.0, 0.0);

glm::vec3 move_x(0.01, 0.0, 0.0);
glm::vec3 move_y(0.0, 0.01, 0.0);

float x_radi = 0.0;
float y_radi = 0.0;
float cube_radi = 0.0;
float world_radi = 0.0;
float camera_radi = 0.0;

float x_axis = 0.0;
float y_axis = 0.0;

int flag = 0;

bool pos_flag = false;
bool dir_flag = false;

GLfloat matv[] = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

GLfloat moving[10][3] = {
	0.5, 0.0, 0.0,
	0.0, 0.5, 0.0,
	0.0, 0.0, 0.5,
	-0.5, 0.0, 0.0,
	0.0, -0.5, 0.0,
	0.0, 0.0, -0.5,
	0.5, 0.5, 0.0,
	0.5, -0.5, 0.0,
	-0.5, 0.5, 0.0,
	-0.5, -0.5, 0.0
};

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
{
	//create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	//Read the vertex shader code from the file
	string VertexShaderCode;
	ifstream VertexShaderStream(vertex_file_path, ios::in);
	if (VertexShaderStream.is_open())
	{
		string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	//Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	//Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength != 0) {
		vector<char> VertexShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
	}

	//Read the fragment shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path, ios::in);
	if (FragmentShaderStream.is_open())
	{
		string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	//Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	//Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength != 0) {
		vector<char> FragmentShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
	}

	//Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength != 0) {
		vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


void renderScene(void)
{
	//Clear all pixels
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Let's draw something here
	glUniformMatrix4fv(proTrans, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(viewTrans, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	for (int i = 0; i < 10; i++) {
		glBindVertexArray(VAO);
		matv[12] = moving[i][0];
		matv[13] = moving[i][1];
		matv[14] = moving[i][2];
		glUniformMatrix4fv(y_rot, 1, GL_FALSE, glm::value_ptr(y_rotateMatrix));
		glUniformMatrix4fv(x_rot, 1, GL_FALSE, glm::value_ptr(x_rotateMatrix));
		glUniformMatrix4fv(f_rot, 1, GL_FALSE, glm::value_ptr(f_rotateMatrix));
		glUniformMatrix4fv(s_rot, 1, GL_FALSE, glm::value_ptr(s_rotateMatrix));
		glUniformMatrix4fv(t_rot, 1, GL_FALSE, glm::value_ptr(t_rotateMatrix));
		glUniformMatrix4fv(matTrans, 1, GL_FALSE, matv);
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, nullptr);
	}

	//define the size of point and draw a point.
	//glDrawArrays(GL_QUADS, 0, 24);

	

	//Double buffer
	glutSwapBuffers();
}


void LightSet(int num) {

	if (num == 1) {
		programID = LoadShaders("Gouraud.txt", "FS.txt");
	}
	else if (num == 2) {
		programID = LoadShaders("VS.txt", "Phong.txt");
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glUseProgram(programID);

	glGenBuffers(2, Buffers);

	dot = glGetAttribLocation(programID, "vtxPosition");
	colorLoc = glGetAttribLocation(programID, "a_Color");
	matTrans = glGetUniformLocation(programID, "trans");
	viewTrans = glGetUniformLocation(programID, "ViewMat");
	proTrans = glGetUniformLocation(programID, "ProMat");
	x_rot = glGetUniformLocation(programID, "X_CRMat");
	y_rot = glGetUniformLocation(programID, "Y_CRMat");
	f_rot = glGetUniformLocation(programID, "F_Rotate");
	s_rot = glGetUniformLocation(programID, "S_Rotate");
	t_rot = glGetUniformLocation(programID, "T_Rotate");

	light_pos = glGetUniformLocation(programID, "uLightPos");
	light_col = glGetUniformLocation(programID, "LightColor");
	light_dir = glGetUniformLocation(programID, "D_LightDir");
	if (!pos_flag) {
		glUniform3f(light_pos, 1.0, 1.0, 1.0);
	}
	else {
		glUniform3f(light_pos, 1.0, 1.0, -1.0);
	}
	if (num == 3) {
		pos_flag = !pos_flag;
	}
	glUniform3f(light_col, 1.0, 1.0, 1.0);
	if (!dir_flag) {
		glUniform3f(light_dir, -1.0, 0.7, 0.3);
	}
	else {
		glUniform3f(light_dir, -1.0, 0.7, -0.3);
	}
	if (num == 4) {
		dir_flag = !dir_flag;
	}
	


	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 96, cube, GL_STATIC_DRAW);
	glVertexAttribPointer(dot, 4, GL_FLOAT, GL_FALSE, 0, ((void*)(0)));
	glEnableVertexAttribArray(dot);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 96, p_color, GL_STATIC_DRAW);
	glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, 0, ((void*)(0)));
	glEnableVertexAttribArray(colorLoc);

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	
	glutPostRedisplay();

}

void myMouse(int button, int state, int x, int y) {

	/*if ((state == GLUT_DOWN) && (button == GLUT_LEFT_BUTTON)) {
		points[(flag) % 2][0] = (float)(x - (wsx / 2.0)) / (wsx / 2.0);
		points[(flag++) % 2][1] = (float)((wsy / 2.0) - y) / (wsy / 2.0);

	}
	if ((state == GLUT_DOWN) && (button == GLUT_RIGHT_BUTTON)) {
		for (int i = 0; i < 4; i++) {
			p_color[0][i] = rgb[cflag1 % 3][i];
			p_color[1][i] = rgb[cflag2 % 3][i];
		}
		cflag1++;
		cflag2++;
	}*/

};

void myKey(unsigned char key, int x, int y) {

	if (key == 'p') {

		float fov = 90.0f;
		float nearPlane = 0.1f;
		float farPlane = 100.0f;

		projectionMatrix = glm::perspective(glm::radians(fov), 1.0f, nearPlane, farPlane);

	}
	if (key == 'o') {
		
		projectionMatrix = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, 0.1f, 100.0f);

	}

	if (key == 'd') {
		x_radi += 1.0f;
		x_rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(x_radi), glm::vec3(0.0f, 1.0f + glm::radians(y_radi), 0.0f));
	}
	if (key == 'a') {
		x_radi -= 1.0f;
		x_rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(x_radi), glm::vec3(0.0f, 1.0f + glm::radians(y_radi), 0.0f));
	}
	if (key == 'w') {
		y_radi -= 1.0f;
		y_rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(y_radi), glm::vec3(1.0f + glm::radians(x_radi), 0.0f, 0.0f));
	}
	if (key == 's') {
		y_radi += 1.0f;
		y_rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(y_radi), glm::vec3(1.0f + glm::radians(x_radi), 0.0f, 0.0f));
	}

	if (key == 't') {
		
		if (flag == 0) {
			cube_radi += 1.0f;
			f_rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(cube_radi), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (flag == 1) {
			world_radi += 1.0f;
			s_rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(world_radi), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (flag == 2) {
			camera_radi += 1.0f;
			t_rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(camera_radi), glm::vec3(0.0f, 1.0f, 0.0f));

		}
	}
	if (key == 'q') {

		if (flag == 0) {
			cube_radi -= 1.0f;
			f_rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(cube_radi), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (flag == 1) {
			world_radi -= 1.0f;
			s_rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(world_radi), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (flag == 2) {
			camera_radi -= 1.0f;
			t_rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(camera_radi), glm::vec3(0.0f, 1.0f, 0.0f));

		}
	}

	if (key == '1') {
		LightSet(1);
	}
	if (key == '2') {
		LightSet(2);
	}

	if (key == 'n') { //광원 위치 변경
		LightSet(3);
	}
	if (key == 'm') { //광원 방향 변경
		LightSet(4);
	}


	glutPostRedisplay();
	
}

void SpecialKey(int key, int x, int y) {

	if (key == GLUT_KEY_RIGHT) {
		camPos += move_x;
		viewMatrix = glm::lookAt(camPos, camPos + camFrontDir, worldUpDir);
	}
	if (key == GLUT_KEY_LEFT) {
		camPos -= move_x;
		viewMatrix = glm::lookAt(camPos, camPos + camFrontDir, worldUpDir);
	}
	if (key == GLUT_KEY_UP) {
		camPos += move_y;
		viewMatrix = glm::lookAt(camPos, camPos + camFrontDir, worldUpDir);
	}
	if (key == GLUT_KEY_DOWN) {
		camPos -= move_y;
		viewMatrix = glm::lookAt(camPos, camPos + camFrontDir, worldUpDir);
	}

	if (key == GLUT_KEY_F1) { //Cube 중심
		flag = 0;
	}
	if (key == GLUT_KEY_F2) { //World 좌표계 중심
		flag = 1;
	}
	if (key == GLUT_KEY_F3) { //카메라의 중심
		flag = 2;
	}

	glutPostRedisplay();
}


void init()
{
	//initilize the glew and check the errors.
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s' \n", glewGetErrorString(res));
	}

	//select the background color
	glClearColor(1.0, 1.0, 1.0, 1.0);

	viewMatrix = glm::lookAt(camPos, camPos + camFrontDir, worldUpDir);

	float fov = 90.0f;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;

	projectionMatrix = glm::perspective(glm::radians(fov), 1.0f, nearPlane, farPlane);

	//glEnable(GL_POINT_SPRITE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

}


int main(int argc, char** argv)
{
	//init GLUT and create Window
	//initialize the GLUT
	glutInit(&argc, argv);
	//GLUT_DOUBLE enables double buffering (drawing to a background buffer while the other buffer is displayed)
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	//These two functions are used to define the position and size of the window. 
	glutInitWindowPosition(wx, wy);
	glutInitWindowSize(wsx, wsy);
	//This is used to define the name of the window.
	glutCreateWindow("Simple OpenGL Window");

	//call initization function
	init();

	//1.
	//Generate VAO


	LightSet(1);

	glutDisplayFunc(renderScene);


	//glutMouseFunc(myMouse);
	glutKeyboardFunc(myKey);
	glutSpecialFunc(SpecialKey);

	//enter GLUT event processing cycle
	glutMainLoop();

	glDeleteVertexArrays(1, &VAO);

	return 1;
}

