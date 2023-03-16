#include "InputManager.h"
#include <string.h>
#include "glut.h"


static char keys_current[256];
static char keys_previous[256];


void KeyboardUpFunc(unsigned char key, int x, int y)
{
	keys_current[key] = 0;
}

void KeyboardDownFunc(unsigned char key, int x, int y)
{
	keys_current[key] = 1;
}

//Special keys and the like can go herre
//May want to wrap things in an enum to hide the implementation details.

int IsKeyPressed(char key)
{
	return keys_current[key];
}

int IsKeyReleased(char key)
{
	return !keys_current[key] && keys_previous[key];
}

int IsKeyTriggered(char key)
{

	return keys_current[key] && !keys_previous[key];
}

void UpdateInputManager()
{
	memcpy(keys_previous, keys_current, sizeof(unsigned char) * 256);
}

void InitInputManager()
{
	glutKeyboardUpFunc(KeyboardUpFunc);
	glutKeyboardFunc(KeyboardDownFunc);
}