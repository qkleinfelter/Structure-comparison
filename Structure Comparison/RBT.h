/*
	RBT.h
	Author: Quinn Kleinfelter
	EECS 2510-001 Non Linear Data Structures Spring 2020
	Dr. Thomas
	Last Edited: 4/1/2020
	Header for RBT.cpp
*/

#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <string.h>
#include <time.h>
using namespace std;
class RBT
{
public:
	RBT(); // Constructor
	~RBT(); // Destructor
	void insert(const char word[50]); // Inserts the word into our RBT
	void list(); // Lists out the RBT nicely
	unsigned long long getHeight(); // Returns the height of the tree
	void displayStatistics(); // Displays statistics about the work we did on the tree
private:
	struct node
	{
		char word[50] = {}; // Variable to hold the word for each node
		int count = 1; // Variable to hold the count of how many times the word in the node has appeared
		node* left = nullptr; // Left child pointer variable
		node* right = nullptr; // Right child pointer variable
		node* parent = nullptr; // Parent pointer variable
		bool color = RED; // Variable to keep track of what color our node is, true = BLACK, false = RED
	};

	// Constants so we can use BLACK and RED instead of true and false
	const static bool BLACK = true; 
	const static bool RED = false;

	node* nil = nullptr; // Nil node, that replaces nullptr in the RBT
	node* root = nullptr; // Root node, initially nullptr
	unsigned long long recolorings = 0;		// Variable to track the number of recolorings we made
	unsigned long long ptrChanges = 0;		// Variable to track the number of pointer changes we made
	unsigned long long keyComparisons = 0;  // Variable to track the number of key comparisons we made
	unsigned long long noFixesNeeded = 0;   // Variable to track the number of times we didn't need to fix anything when inserting
	unsigned long long leftRot = 0;
	unsigned long long rightRot = 0;
	unsigned long long case1Fix = 0;		// Variable to track the number of case 1 fixups we made
	unsigned long long case2Fix = 0;		// Variable to track the number of case 2 fixups we made
	unsigned long long case3Fix = 0;		// Variable to track the number of case 3 fixups we made
	unsigned long long treeHeight = 0;		// Variable to track the height of the tree

	void traverse(int& index, node* n);		// Helper method for printing out the tree nicely

	void deleteNode(node* n);				// Helper method that deletes the subtrees of n and then n itself

	void leftRotation(node* x);				// Helper method that runs a left rotation about node x
	void rightRotation(node* x);			// Helper method that runs a right rotation about node x
	void insertFixup(node* z);				// Helper method that does any necessary fixes on node z
	void calculateHeight(node* currNode, unsigned long long pathHeight);  // Helper method to calculate the height of the tree
	void calculateWords(unsigned long long& numWords, unsigned long long& numUniqueWords); // Helper method to calculate the number of words & unique words in the tree
	void calculateWords(node* start, unsigned long long& numWords, unsigned long long& numUniqueWords); // Helper method to calculate the number of words & unique words in the subtree starting at a given node
};