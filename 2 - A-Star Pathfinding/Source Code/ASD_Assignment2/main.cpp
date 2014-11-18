#include <iostream>
#include <time.h>
#include "GL/freeglut.h"
#include "linkedlists.h"			// linked lists
#include "pathfinding.h"			// pathfinding functions
using namespace std;

// global variables
struct
{
	string Title;
	int Width, Height;
	bool done;
} application;			// application window/execution information
Map *testMap;							// map info container
LinkedList<MapPos> *path;				// path info container
MapPos startPos, goalPos;				// start and goal position containers

int blockSize = 20;

// function prototypes
void InitSettings(int*, char**);
void Render();
void ReadKeyboardUp(unsigned char, int, int);
void ReadMouseDown(int, int, int, int);

// program entry point
int main(int argc, char** argv)
{
	// load in map from file
	testMap = LoadMap("testmap.txt");

	// initialise all FreeGLUT settings
	InitSettings(&argc, argv);

	// basic starting position input
	startPos.X = -1; startPos.Y = -1;
	goalPos.X = -1; goalPos.Y = -1;

	// main event loop (runs until manual exit)
	application.done = false;
	while (!application.done) glutMainLoopEvent();

	// clean up memory
	delete[] path; path = nullptr;
	delete[] testMap; testMap = nullptr;

	// finish program execution with no errors
	return 0;
}

// initialisation and rendering method(s)
void InitSettings(int* argc, char **argv)
{
	// initialise FreeGLUT window settings
	application.Title = "A* Pathfinding";
	application.Width = (testMap->width * blockSize) + 40;
	application.Height = (testMap->height * blockSize) + 30;

	// initialise window settings (size, position, title, etc.)
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(application.Width, application.Height);
	glutInitWindowPosition(10, 10);
	glutCreateWindow(application.Title.c_str());

	// link processes for later use (graphics rendering and input)
	glutDisplayFunc(Render);
	glutKeyboardUpFunc(ReadKeyboardUp);
	glutMouseFunc(ReadMouseDown);

	// setup orthographic viewport (i.e. 2D)
	glViewport(0, 0, application.Width, application.Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, application.Width, application.Height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
};
void Render()
{
	// if the application is still running...
	if ( application.done == false )
	{
		// declare holder values for a node pointer and display list
		Node<MapPos> *holder;
		GLuint block = glGenLists(1);

		// clear the graphics buffer with a light shade of blue
		glClearColor(0.7f, 0.7f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
	
		// initialise display list information (basic 30x30 2D square)
		glNewList(block, GL_COMPILE);
			glBegin(GL_POLYGON);
				glVertex2f(0.0f, 0.0f);
				glVertex2f((float)blockSize, 0.0f);
				glVertex2f((float)blockSize, (float)blockSize);
				glVertex2f(0.0f, (float)blockSize);
			glEnd();
		glEndList();

		// checks to see if the map was actually loaded before trying to draw it
		if ( testMap != nullptr )
		{
			// traverses each horizontal node in the list...
			for ( int i = 0; i < testMap->width; i++ )
			{
				// traverses each vertical node in the list...
				for ( int j = 0; j < testMap->height; j++ )
				{
					glPushMatrix();
						// moves to the correct position for the specific node
						glTranslatef(i * ((float)blockSize+1), j * ((float)blockSize+1), 0.0f);
						// set the color to black for impassable node, or white for passable
						if ( (*testMap)(i, j) == true ) glColor3f(0.0f, 0.0f, 0.0f);
						else glColor3f(1.0f, 1.0f, 1.0f);
						// call the display list defined before, drawn to the correct position
						glCallList(block);
					glPopMatrix();
				}
			}
		}
	
		// if a valid path has been calculated...
		if ( path != nullptr )
		{
			// point to the first node in the path linked list
			holder = path->first;
			// while there's still a node to draw...
			while ( holder != nullptr )
			{
				glPushMatrix();
					// move to the correct position
					glTranslatef(holder->data.X * ((float)blockSize+1), holder->data.Y * ((float)blockSize+1), 0.0f);
					// set the color to gray (to help differentiate it from the map)
					glColor3f(0.5f, 0.5f, 0.5f);
					// call the display list, drawn to the correct position
					glCallList(block);
				glPopMatrix();

				// pointer to the next node in the list
				holder = holder->nextNode;
			}
		}

		// draw the start and goal positions
		glPushMatrix();
			glTranslatef(startPos.X * 21.0f, startPos.Y * 21.0f, -0.1f);
			glColor3f(1.0f, 1.0f, 0.0f);
			glCallList(block);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(goalPos.X * 21.0f, goalPos.Y * 21.0f, -0.1f);
			glColor3f(0.5f, 0.5f, 0.0f);
			glCallList(block);
		glPopMatrix();

		// remove the display list from memory now that we aren't using it
		glDeleteLists(block, 1);

		// empty the current buffers
		glFlush();
		// swap the new information in for displaying
		glutSwapBuffers();
	}
}

// input methods
void ReadKeyboardUp(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27:
		// exit the program (escape key)
		application.done = true;
		break;
	case 'r':
		// delete the old map and path information (they're no longer relevant)
		delete[] testMap; testMap = nullptr;
		delete[] path; path = nullptr;
		// reload the map from the text file
		testMap = LoadMap("testmap.txt");
		break;
	case 'c':
		// clear the map of all obstacles
		for ( int i = 0; i < testMap->width * testMap->height; i++ )
			testMap->data[i] = 0;
		// delete the old path (as it might no longer be the shortest path to the goal)
		delete[] path; path = nullptr;
		break;
	}

	// redraw the screen
	glutPostRedisplay();
}
void ReadMouseDown(int button, int state, int x, int y)
{
	// left mouse click : moves the start position to selected node
	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{
		// assign the new goal position based on mouse position
		x -= (x % (blockSize + 1)); x /= (blockSize + 1); startPos.X = x;
		y -= (y % (blockSize + 1)); y /= (blockSize + 1); startPos.Y = y;
		
		// clear old path
		delete[] path;

		// if they're holding down the shift key, then draw the search process out for them to see
		if ( glutGetModifiers() == GLUT_ACTIVE_CTRL )
		{
			if ( (path = findPath(*testMap, startPos.X, startPos.Y, goalPos.X, goalPos.Y, false)) != nullptr )
			{
				delete[] path;
				path = findPath(*testMap, startPos.X, startPos.Y, goalPos.X, goalPos.Y, true);
			}
		}
		// otherwise just create the path
		else path = findPath(*testMap, startPos.X, startPos.Y, goalPos.X, goalPos.Y, false);
	}

	// right mouse click : moves the goal position to selected node
	if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
	{
		// assign the new goal position based on mouse position
		x -= (x % (blockSize + 1)); x /= (blockSize + 1); goalPos.X = x;
		y -= (y % (blockSize + 1)); y /= (blockSize + 1); goalPos.Y = y;

		// clear old path
		delete[] path;

		// if they're holding down the shift key, then draw the search process out for them to see
		if ( glutGetModifiers() == GLUT_ACTIVE_CTRL )
		{
			if ( (path = findPath(*testMap, startPos.X, startPos.Y, goalPos.X, goalPos.Y, false)) != nullptr )
			{
				delete[] path;
				path = findPath(*testMap, startPos.X, startPos.Y, goalPos.X, goalPos.Y, true);
			}
		}
		// otherwise just create the path
		else path = findPath(*testMap, startPos.X, startPos.Y, goalPos.X, goalPos.Y, false);
	}

	// middle mouse click : adds/removes obstacles in the map
	if ( button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN )
	{
		// find the correct node being selected
		x -= (x % (blockSize + 1)); x /= (blockSize + 1);
		y -= (y % (blockSize + 1)); y /= (blockSize + 1);
		// flip the value for that node in the map
		testMap->data[(y * testMap->width) + x] = !testMap->data[(y * testMap->width) + x];
		// delete the current path (since it might not work with the new map)
		delete[] path; path = nullptr;
	}
}