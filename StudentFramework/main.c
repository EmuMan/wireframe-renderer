#include "FrameBuffer.h"
#include "FrameworkInterface.h"
#include "InputManager.h"
#include <stdio.h>
int winID;

void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseMove(int x, int y);
void mouseIdle(int x, int y);

void loop(void)
{
	glutPostRedisplay();
	UpdateInputManager();
}

void frameworkUpdate(void)
{
	FrameBuffer_Clear(&GlobalFrameBuffer, 255, 255, 255);

	render();

	glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, GlobalFrameBuffer.buffer);
	glutSwapBuffers();
}

void init(void)
{
	InitInputManager();
	FrameBuffer_Init(&GlobalFrameBuffer, WIDTH, HEIGHT);
	Init();
}

//Lock the window to the initial WIDTH and HEIGHT
void reshape(int newX, int newY)
{
	if (newX != WIDTH || newY != HEIGHT)
	{
		glutReshapeWindow(WIDTH, HEIGHT);
	}

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	winID = glutCreateWindow("WINDOOOOOOW");
	glutReshapeFunc(reshape);

	glClearColor(0, 0, 0, 1);

	//glutKeyboardFunc(keyboard);
	glutDisplayFunc(frameworkUpdate);
	glutIdleFunc(loop);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseIdle);
	//glutFullScreen();

	init();

	glutMainLoop();
	return 0;
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case VK_ESCAPE:
		FrameBuffer_Destroy(&GlobalFrameBuffer);
		glutDestroyWindow(winID);
		exit(0);
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	/*switch (button)
	{
	}*/
}

int mouseX = 0;
int mouseY = 0;
void mouseMove(int x, int y)
{
	mouseX = x;
	mouseY = y;
}
void mouseIdle(int x, int y)
{
	mouseX = x;
	mouseY = y;
}
