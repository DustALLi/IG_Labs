#include<stdio.h>
#include<iostream>
#include<GL/freeglut.h>

//�������������� ������� - ������� �����;

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	
	glutCreateWindow("Tutorial 01");

	glutDisplayFunc(RenderSceneCB);

	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glutSwapBuffers();
	glutMainLoop();

	return 0;
}