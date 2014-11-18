// pathfinding.h : ...

#ifndef pathfinding___
#define pathfinding___

#include <iostream>
#include <fstream>
#include "linkedlists.h"
#include <GL/freeglut.h>
using namespace std;

// Map class : holds the all relevant data (content + size) of a given map
struct Map
{
	// data members
	bool* data;
	int width, height;

	// constructor and destructor
	Map()
	{
		// initializes all default values
		data = nullptr;
		width = 0;
		height = 0;

		// writes out a message to the console to indicate it worked
		cout << "Map container created successfully." << endl;
	}
	~Map()
	{
		// deletes all the data from heap memory
		delete[] data;

		// writes a message out to the console to indicate it worked
		cout << "Map data cleared from memory." << endl;
	}

	// write() method : used to write the map information out to console for debugging purposes
	void write()
	{
		// for every row...
		for ( int i = 0; i < height; i++ )
		{
			// and every column...
			for ( int j = 0; j < width; j++ )
			{
				// write out the specific information to console
				cout << data[(i * width) + j] << " ";
			}
			cout << endl;
		}
	}

	// operator()(x, y) method : used for returning specific map position data
	bool operator()(int x, int y)
	{
		if ( x < 0 || x >= width || y < 0 || y >= height ) return true;
		return data[(y * width) + x];
	}
};

// MapPos class : holds specific data for a specific map co-ordinate, used in pathfinding
struct MapPos
{
	// data members
	int X, Y;					// the actual position, x and y
	int G, H, F;				// used for pathfinding calculation (current cost, remaining cost (est.), total cost)
	int parentX, parentY;		// the parent node's X and Y positions

	// constructor(s)
	MapPos()
	{
		G = 0; H = 0; F = 0;
		X = 0; Y = 0;
		parentX = -1; parentY = -1;
	}
	MapPos(int x, int y)
	{
		G = 0; H = 0; F = 0;
		X = x; Y = y;
		parentX = -1; parentY = -1;
	}
	MapPos(int x, int y, int g, int h)
	{
		G = g; H = h; F = G + H;
		X = x; Y = y;
		parentX = -1; parentY = -1;
	}

	// setParent() method : used to set the parent details based on passed MapPos object data
	void setParent(MapPos p)
	{
		parentX = p.X;
		parentY = p.Y;
	}

	// operator methods : used to comparison and equality operations, ensuring deep copy for the latter and making sure that MapPos objects are seen as "equal" only if their map co-ordinates are the same
	void operator=(const MapPos& b)
	{
		X = b.X;
		Y = b.Y;
		G = b.G;
		H = b.H;
		F = b.F;
		parentX = b.parentX;
		parentY = b.parentY;
	}
	bool operator==(const MapPos& b)
	{
		// nodes are found to be equal if their positions are the same
		if ( X == b.X && Y == b.Y )
			return true;
		return false;
	}
};

// findPath() method : this is the A* pathfinding method, and takes all the passed information whilst returning the shortest path to the goal in the form of a LinkedList object
LinkedList<MapPos>* findPath(Map& map, int startX, int startY, int goalX, int goalY, bool drawProcess)
{
	// checks that all passed values are correct
	if ( startX < 0 || startX > map.width - 1 || startY < 0 || startY > map.height - 1 )
		return nullptr;
	if ( goalX < 0 || goalY > map.width - 1 || goalY < 0 || goalY > map.height - 1 )
		return nullptr;

	// create new lists for later use
	LinkedList<MapPos> openList;
	LinkedList<MapPos> closedList;
	LinkedList<MapPos> adjacentSquares;

	MapPos holderPos;
	MapPos currentSquare;
	MapPos goalSquare(goalX, goalY);

	Node<MapPos>* checkSquare1 = nullptr;
	Node<MapPos>* checkSquare2 = nullptr;
	LinkedList<MapPos>* finalPath = nullptr;

	// adds original position to the open list
	currentSquare.X = startX;
	currentSquare.Y = startY;
	openList.push(currentSquare);
	
	do
	{
		// grabs the square with the lowest F score from the open list
		for ( int i = 0; i < openList.size; i++ )
		{
			// grabs the pointer to the head of the list, for reference
			if ( i == 0 )
			{
				checkSquare1 = openList.first;
				checkSquare2 = checkSquare1;
				continue;
			}
			// compares all subsequent positions in the list and stores lowest
			else
			{
				checkSquare2 = checkSquare2->nextNode;
				if ( checkSquare2 != nullptr && checkSquare2->data.F < checkSquare1->data.F )
					checkSquare1 = checkSquare2;
			}
		}
		currentSquare = checkSquare1->data;

		// adds the current square to the closed list
		closedList.push(currentSquare);

		// remove it from the open list
		openList.remove(currentSquare);

		// if the goal square has been placed in the closed list, finish
		if ( closedList.search(goalSquare) != -1 )
		{
			finalPath = new LinkedList<MapPos>[1];
			break;
		}

		// adds all appropriate adjacent squares to the open list
		for ( int i = 0; i < 4; i++ )
		{
			holderPos = currentSquare;

			switch(i)
			{
			case 0:
				if ( map(currentSquare.X + 1, currentSquare.Y) ) continue;
				holderPos.X += 1;
				break;
			case 1:
				if ( map(currentSquare.X, currentSquare.Y + 1) ) continue;
				holderPos.Y += 1;
				break;
			case 2:
				if ( map(currentSquare.X - 1, currentSquare.Y) ) continue;
				holderPos.X -= 1;
				break;
			case 3:
				if ( map(currentSquare.X, currentSquare.Y - 1) ) continue;
				holderPos.Y -= 1;
				break;
			}

			holderPos.G += 1;
			holderPos.H = abs(goalSquare.X - holderPos.X) + abs(goalSquare.Y - holderPos.Y);
			holderPos.F = holderPos.G + holderPos.H;

			adjacentSquares.push(holderPos);
		}

		// if there are adjacent squares that are walkable...
		checkSquare1 = adjacentSquares.first;
		while ( checkSquare1 != nullptr )
		{
			// if it's already in the closed list... ignore it
			if ( closedList.search(checkSquare1->data) != -1 )
			{
				// move onto the next node in the list
				checkSquare1 = checkSquare1->nextNode;
				continue;
			}
			// if it's NOT in the open list...
			if ( openList.search(checkSquare1->data) == -1 )
			{
				// set the parent to the current square, and add to open list
				checkSquare1->data.setParent(currentSquare);
				openList.push(checkSquare1->data);
			}
			// if it IS in the open list already...
			else
			{
				// grab the address of the square in the open list
				checkSquare2 = openList.getAddressAt(openList.search(checkSquare1->data));

				// compare the scores, see if this new path makes it easier
				if ( (checkSquare1->data.G + checkSquare2->data.H) < (checkSquare2->data.G + checkSquare2->data.H) )
				{
					// if it does, update the parent and G scores
					checkSquare2->data.G = checkSquare1->data.G;
					checkSquare2->data.setParent(currentSquare);
				}
			}
			
			// move onto the next node in the list
			checkSquare1 = checkSquare1->nextNode;
		}

		// clears adjacent square list
		adjacentSquares.clear();

		// if the caller has said there should be a drawn path...
		if ( drawProcess == true )
		{
			// draw the open list nodes
			for ( checkSquare1 = openList.first; checkSquare1 != nullptr; checkSquare1 = checkSquare1->nextNode )
			{
				glPushMatrix();
					glTranslatef((float)checkSquare1->data.X * 21.0f, (float)checkSquare1->data.Y * 21.0f, -0.1f);
					glBegin(GL_POLYGON);
						glColor3f(0.0f, 1.0f, 0.0f);
						glVertex3f(0, 20.0f, 0);
						glVertex3f(20.0f, 20.0f, 0);
						glVertex3f(20.0f, 0, 0);
						glVertex3f(0, 0, 0);
					glEnd();
				glPopMatrix();
			}

			// draw the closed list nodes
			for ( checkSquare1 = closedList.first; checkSquare1 != nullptr; checkSquare1 = checkSquare1->nextNode )
			{
				glPushMatrix();
					glTranslatef((float)checkSquare1->data.X * 21.0f, (float)checkSquare1->data.Y * 21.0f, -0.1f);
					glBegin(GL_POLYGON);
						glColor3f(1.0f, 0.0f, 0.0f);
						glVertex3f(0, 20.0f, 0);
						glVertex3f(20.0f, 20.0f, 0);
						glVertex3f(20.0f, 0, 0);
						glVertex3f(0, 0, 0);
					glEnd();
				glPopMatrix();
			}

			// push the images to the screen
			glFlush();
			glutSwapBuffers();

			// delay, so we can see the process
			Sleep(10);
		}
		
	} while ( openList.size > 0 );

	// if the path to the goal was found, calculate the path
	if ( finalPath != nullptr )
	{
		// first push the goal node onto the list
		Node<MapPos>* test = closedList.getAddressAt(closedList.search(goalSquare));
		holderPos = closedList.getAddressAt(closedList.search(goalSquare))->data;
		finalPath->push(holderPos);

		// then push the parent of the goal node onto the list
		while ( holderPos.parentX != -1 )
		{
			holderPos.X = holderPos.parentX;
			holderPos.Y = holderPos.parentY;
			holderPos = closedList.getAddress(holderPos)->data;
			finalPath->push(holderPos);
		}
	}
	
	// reassign pointers (just in case) - no deletion, since the nodes are still used by the linked lists
	checkSquare1 = nullptr;
	checkSquare2 = nullptr;

	return finalPath;
}

// LoadMap() method : loads in and creates a Map object based on a text file's contents
Map* LoadMap(const char* filePath)
{
	// create a pointer for the map
	Map* newMap = nullptr;

	// attempt to open the file
	ifstream file(filePath);

	// if it opened correctly...
	if ( file != NULL )
	{
		newMap = new Map[1];
		file >> newMap->width; file >> newMap->height;
		newMap->data = new bool[newMap->width * newMap->height];

		for ( int i = 0; i < newMap->width * newMap->height; i++ )
		{
			file >> newMap->data[i];
		}
		
		file.close();

		cout << "Map data successfully loaded." << endl;
	}
	// otherwise...
	else
	{
		cout << "File 'testMap->txt' did not open correctly." << endl;
	}

	// return the pointer to the caller
	return newMap;
}

#endif