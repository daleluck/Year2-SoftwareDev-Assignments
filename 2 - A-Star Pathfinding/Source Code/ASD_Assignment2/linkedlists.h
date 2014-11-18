// linkedlists.h : ...

#ifndef linkedlists___
#define linkedlists___

#include <iostream>
using namespace std;

// Node<T> class : an individual value, made to be used in a doubly linked list
template <class T> class Node
{
public:
	T data;				// value of the current node, type T
	Node* previous;		// pointer to the previous node in the list
	Node* nextNode;		// pointer to the next node in the list

	// constructor
	Node()
	{
		// initializes the pointers to null, just in case
		previous = nullptr;
		nextNode = nullptr;
	}
};

// LinkedList<T> class : a series of Node objects grouped together, with methods for processing them together
template <class T> class LinkedList
{
public:
	Node<T>* first;			// first member of the list
	int size;				// number of nodes in the list

	// constructor and destructor
	LinkedList()
	{
		first = nullptr;
		size = 0;
	}
	~LinkedList()
	{
		// hold the pointer to the next node being deleted, and the current node being deleted
		Node<T>* currentNode = first;
		Node<T>* nextNode;

		// while there's a node to check...
		while ( currentNode != nullptr )
		{
			// store the address of the node AFTER the current one
			nextNode = currentNode->nextNode;

			// delete the current node
			delete[] currentNode;

			// switch focus to the next node in the list
			currentNode = nextNode;
		}
	}

	// insertion and removal methods
	void push(T value)			// adds a value to the end of the list
	{
		// pointer to the node being checked (and eventually, the current last node in the list)
		Node<T>* currentNode = first;

		if ( currentNode == nullptr )
		{
			first = new Node<T>[1];
			first->data = value;
		}
		else
		{
			// create the new node in memory
			currentNode = new Node<T>[1];
			currentNode->data = value;
			// point the new node to the current first node
			currentNode->previous = nullptr;
			currentNode->nextNode = first;
			// point the current first node to the new node
			first->previous = currentNode;
			// set the new node to be the first
			first = currentNode;
		}

		// adds one to the size holder
		size += 1;
	}
	void insertAt(int index, T value)
	{
		Node<T>* currentNode;
		Node<T>* nextNode;

		// if they're trying to insert at the beginning of the list...
		if ( index == 0 )
		{
			if ( first == nullptr )
			{
				first = new Node<T>[1];
				first->data = value;
				first->previous = nullptr;
				first->nextNode = nullptr;
			}
			else
			{
				currentNode = first;
				first = new Node<T>[1];
				first->data = value;
				first->previous = nullptr;
				first->nextNode = currentNode;
			}
			
			// increments the size holder
			size += 1;
		}
		// or if it's at some other point in the list...
		else if ( index > 0 )
		{
			currentNode = first;

			// get to the right index (if index is out of bounds, will append to end of list)
			// note, the "right index" is the node BEFORE the location we want to add to
			for ( int i = 1; i < index && currentNode->nextNode != nullptr; i++ )
			{
				// store the next node's address
				currentNode = currentNode->nextNode;
			}

			// store the address of the "next node" after the new node (may be null)
			nextNode = currentNode->nextNode;

			// creates the new node after the current node
			currentNode->nextNode = new Node<T>[1];
			currentNode->nextNode->data = value;
			currentNode->nextNode->previous = currentNode;
			currentNode->nextNode->nextNode = nextNode;

			// reassigns pointer in the one following the new node, if it exists
			if ( nextNode != nullptr )
				nextNode->previous = currentNode->nextNode;
			
			// increments the size holder
			size += 1;
		}
	}
	void remove(T value)
	{
		// first finds the value in the list, then removes it at the found index
		removeAt(search(value));
	}
	void removeAt(int index)
	{
		// checks to see if the index actually exists and isn't an invalid number
		if ( index >= 0 && index < size )
		{
			// holders for later use
			Node<T>* prevNode = nullptr;
			Node<T>* currentNode = first;
			Node<T>* nextNode = nullptr;
			
			// if the node you want to remove is the first node in the list...
			if ( index == 0 )
			{
				nextNode = currentNode->nextNode;
				delete[] currentNode;
				first = nextNode;
				size -= 1;
				return;
			}

			// finds the address of node to be removed, and surrounding pointers
			for ( int i = 0; i < index; i++ )
			{
				// stores pointer to the correct node
				currentNode = currentNode->nextNode;
			}

			// stores the surrounding pointers
			prevNode = currentNode->previous;
			nextNode = currentNode->nextNode;

			// deletes the correct node and reassigns surrounding pointers
			delete[] currentNode;
			prevNode->nextNode = nextNode;
			
			if ( nextNode != nullptr )
				nextNode->previous = prevNode;
			
			// decrements the size holder
			size -= 1;
		}
		else return;
	}
	void clear()
	{
		while ( size > 0 )
		{
			removeAt(0);
		}
		/*if ( size > 0 )
		{
			Node<T>* currentNode;
			Node<T>* nextNode;

			if ( size == 1 )
			{
				delete[] first;
				first = nullptr;
			}
			else
			{
				currentNode = first;
				while ( currentNode != nullptr )
				{
					nextNode = currentNode->nextNode;
					delete[] currentNode;
					currentNode = nextNode;
				}
			}

			size = 0;
		}*/
	}

	// search methods
	Node<T>* getAddressAt(int index)
	{
		// if they're looking for the first item, just give them the address
		if ( index == 0 ) return first;

		// iterates a pointer until at the right index
		Node<T>* nodePtr = first;
		while ( index > 0 )
		{
			nodePtr = nodePtr->nextNode;
			index--;
		}

		// return the address the pointer points at
		return nodePtr;
	}
	Node<T>* getAddress(T value)
	{
		// finds the value, then returns the address of the index
		return getAddressAt(search(value));
	}
	int search(T value)
	{
		// a holder for the next node address in the list
		Node<T>* currentNode = first;

		// search each node in the list until the value is found
		for ( int i = 0; i < size; i++ )
		{
			if ( currentNode->data == value ) return i;
			else currentNode = currentNode->nextNode;
		}

		// returns -1 if the value wasn't found
		return -1;
	}

	// output methods (basically for debugging)
	void LinkedList<T>::write()
	{
		// creates a pointer, pointing at the first address
		Node<T>* currentNode = first;

		// writes out the data to console (if possible) for every node
		while ( currentNode != nullptr )
		{
			cout << currentNode->data;
			currentNode = currentNode->nextNode;
			if ( currentNode != nullptr ) cout << " , ";
		}

		// starts a new line
		cout << endl;
	}
};

#endif