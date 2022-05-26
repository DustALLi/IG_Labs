#include<stdio.h>
#include<iostream>
#include <assert.h>
#include <math.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
using namespace glm;

//�������������� ������� - ����������� 3� ��������������

GLuint VBO;
GLuint gWorldLocation;

static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWorld;                                                                \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWorld * vec4(Position, 1.0);                                     \n\
}";

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = vec4(1.0, 0.0, 1.0, 0.0);                                           \n\
}";	


static void RenderSceneCB(){

	static float Scale = 0.0f;
	
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 1.0f);

	mat4 World1;
	World1[0][0] = 1.0f;			World1[0][1] = 0.0f;			World1[0][2] = 0.0f;			World1[0][3] = sinf(Scale);
	World1[1][0] = 0.0f;		    World1[1][1] = 1.0f;			World1[1][2] = 0.0f;			World1[1][3] = 0.0f;
	World1[2][0] = 0.0f;			World1[2][1] = 0.0f;			World1[2][2] = 1.0f;			World1[2][3] = 0.0f;
	World1[3][0] = 0.0f;			World1[3][1] = 0.0f;			World1[3][2] = 0.0f;			World1[3][3] = 1.0f;

	mat4 World2;
	World2[0][0] = cosf(Scale);		World2[0][1] = -sinf(Scale);	World2[0][2] = 0.0f;			World2[0][3] = 0.0f;
	World2[1][0] = sinf(Scale);		World2[1][1] = cosf(Scale);		World2[1][2] = 0.0f;			World2[1][3] = 0.0f;
	World2[2][0] = 0.0f;			World2[2][1] = 0.0f;			World2[2][2] = 1.0f;			World2[2][3] = 0.0f;
	World2[3][0] = 0.0f;			World2[3][1] = 0.0f;			World2[3][2] = 0.0f;			World2[3][3] = 1.0f;

	mat4 World3;
	World3[0][0] = sinf(Scale);		World3[0][1] = 0.0f;			World3[0][2] = 0.0f;			World3[0][3] = 0.0f;
	World3[1][0] = 0.0f;			World3[1][1] = cosf(Scale);		World3[1][2] = 0.0f;			World3[1][3] = 0.0f;
	World3[2][0] = 0.0f;			World3[2][1] = 0.0f;			World3[2][2] = sinf(Scale);		World3[2][3] = 0.0f;
	World3[3][0] = 0.0f;			World3[3][1] = 0.0f;			World3[3][2] = 0.0f;			World3[3][3] = 1.0f;

	mat4 WorldFinal = World1 * World2 * World3;

	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &WorldFinal[0][0]);	

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);

	Scale += 0.001f;

	glDisableVertexAttribArray(0);
	
	glutSwapBuffers();
}

static void InitializeGlutCallbacks(){
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
}

static void CreateVertexBuffer(){
	vec3 Vertices[3];
	Vertices[0] = vec3(-0.5f, -0.5f, 0.0f);
	Vertices[1] = vec3(0.5f, -0.5f, 0.0f);
	Vertices[2] = vec3(0.0f, 0.5f, 0.0f);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	const GLchar* p[1];
	
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}
	glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
	AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	glUseProgram(ShaderProgram);
	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	assert(gWorldLocation != 0xFFFFFFFF);
}

int main(int argc, char** argv) 
{
	glutInit(&argc, argv);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_RGB);
	
	glutCreateWindow("Tutorial 06");

	glutDisplayFunc(RenderSceneCB);

	vec3 Vertices[4];
	Vertices[0] = glm::vec3(-0.5f, 0.0f, 0.0f);
	Vertices[1] = glm::vec3(0.0f, 0.5f, 0.0f);
	Vertices[2] = glm::vec3(0.5f, 0.0f, 0.0f);
	Vertices[3] = glm::vec3(0, 0, 0);

	InitializeGlutCallbacks();

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	CreateVertexBuffer();
	CompileShaders();
	glutMainLoop();

	return 0;
}