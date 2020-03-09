#include <iostream>
#include <stdlib.h>
#include "include\GL\glut.h"
#include <Windows.h>
#include <omp.h>


using namespace std;
#include "JGN_Potential.h"

#define JGN_APP_WIDTH 500
#define JGN_APP_HEIGHT 500

void DisplayF();
void PassiveMotionFunc(int x, int y);
void KeyboardFunc(unsigned char key, int x, int y);

void myinit()
{
	vec3 v(-100,-100,-100);
	//JGN_AddPotential(v);
	
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			v = vec3(i*0.1-0.5, j*0.1-0.5, 0);
			JGN_AddPotential(v);
		}
	}
	gravity = vec3(0, -50, 0);
	//JGN_RAMPotential[0]->vel = vec3(0.1, 0, 0);

}




int main(int argc, char** argv)
{
	myinit();


	glutInit(&argc, argv);


	glutInitWindowSize(JGN_APP_WIDTH, JGN_APP_HEIGHT);
	glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH) / 2 - JGN_APP_WIDTH, glutGet(GLUT_SCREEN_HEIGHT) / 2 - JGN_APP_HEIGHT);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	

	glutCreateWindow("Transparent");
	// Set the draw buffer clearing colour to black without modifying the alpha
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Actually clear the draw buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Set the accumulation buffer clearing colour to black without modifying the alpha
	glClearAccum(0.0f, 0.0f, 0.0f, 0.0f);

	// Actually clear the accumulation buffer
	glClear(GL_ACCUM_BUFFER_BIT);
	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	//glEnable(GL_MULTISAMPLE_ARB);

	glutDisplayFunc(DisplayF);
	glutPassiveMotionFunc(PassiveMotionFunc);
	glutKeyboardFunc(KeyboardFunc);
	glPointSize(1);

	glutMainLoop();


	return 0;
}



void DisplayF()
{

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);




	glAccum(GL_RETURN, 0.9);
	glClear(GL_ACCUM_BUFFER_BIT);


	JGN_UpdatePotentialPositions();
	glColor3f(0, 0, 1);
	


	Potential **p = JGN_RAMPotential;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < JGN_NPotential; i++)
	{
		//glColor3f(1, 1 - 0.2*dist(p[i]->acc,vec3(0,0,0)), 1- 0.2*dist(p[i]->acc, vec3(0, 0, 0)));
		glVertex2f(p[i]->pos.x, p[i]->pos.y);
		if (p[i]->pos.y < -0.8)
		{
			p[i]->vel.y = abs(0.9*p[i]->vel.y);
		}
		else if (p[i]->pos.y > 0.8)
		{
			p[i]->vel.y = -abs(0.9*p[i]->vel.y);
		}

		if (p[i]->pos.x < -0.8)
		{
			p[i]->vel.x = abs(0.9*p[i]->vel.x);
		}
		else if (p[i]->pos.x > 0.8)
		{
			p[i]->vel.x = -abs(0.9*p[i]->vel.x);
		}
	}
	glEnd();

	glAccum(GL_ACCUM, 0.95);

	glutSwapBuffers();
	glutPostRedisplay();
	
	return;
}


void PassiveMotionFunc(int x, int y)
{
//	JGN_RAMPotential[0]->pos = vec3((x / (float)JGN_APP_WIDTH) * 2 - 1, -(y / (float)JGN_APP_HEIGHT) * 2 + 1, 0);
//
//	cout << JGN_RAMPotential[0]->pos << endl;
}


void KeyboardFunc(unsigned char key, int x, int y)
{
	Potential **p = JGN_RAMPotential;

	float vel = 50;
	if (key == 'a')
	{
		for (int i = 0; i < JGN_NPotential; i++)
		{
			p[i]->vel = vec3(-vel, 0, 0);
		}
	}
	else if (key == 'd')
	{
		for (int i = 0; i < JGN_NPotential; i++)
		{
			p[i]->vel = vec3(vel, 0, 0);
		}
	}
	
	if (key == 'w')
	{
		for (int i = 0; i < JGN_NPotential; i++)
		{
			p[i]->vel = vec3(0, vel, 0);
		}
	}
	else if (key == 's')
	{
		for (int i = 0; i < JGN_NPotential; i++)
		{
			p[i]->vel = vec3(0, -vel, 0);
		}
	}

}
