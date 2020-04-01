/*
	AVL.h
	Author: Quinn Kleinfelter
	EECS 2510-001 Non Linear Data Structures Spring 2020
	Dr. Thomas
	Last Edited: 4/1/2020
	Header for AVL.cpp
*/

#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <string.h>
#include <time.h>
using namespace std;
class AVL
{
public:
	AVL(); // Constructor
	~AVL(); // Destructor
	void insert(const char word[50]); // Inserts a word of no more than 49 characters in the tree
	void list(); // List out the tree
	unsigned long long getHeight(); // Get the height of the tree
	void displayStatistics(); // Display statistics about the work we did on the tree
private:
	struct node
	{
		// Node structure for our AVL tree
		char word[50] = {}; // The word for each node
		int count = 1; // Number of times we've seen the word
		node* left = nullptr; // Pointers to its left and right children and its parent, all null originally
		node* right = nullptr;
		node* parent = nullptr;
		int balanceFactor = 0; // Balance factor is initially 0
	};
	
	node* root = nullptr;					// Root is initially nullptr
	unsigned long long bfChanges = 0;		// Variable to keep track of the number of times we change a balance factor
	unsigned long long ptrChanges = 0;		// Variable to keep track of the number of times we change a pointer in a node
	unsigned long long keyComparisons = 0;  // Variable to keep track of the number of times we do a key comparison
	unsigned long long noRotNeeded = 0;		// Variable to keep track of the number of times we don't need a rotation
	unsigned long long llRot = 0;			// Variable to keep track of the number of times we do a LL Rotation
	unsigned long long lrRot = 0;			// Variable to keep track of the number of times we do a LR Rotation
	unsigned long long rlRot = 0;			// Variable to keep track of the number of times we do a RL Rotation
	unsigned long long rrRot = 0;			// Variable to keep track of the number of times we do a RR Rotation
	unsigned long long treeHeight = 0;		// Variable to keep track of the height of the tree
	unsigned long long aToYPasses = 0;
	unsigned long long aToYBfChanges = 0;

	void traverse(int& index, node* n);		// Traverse the tree recursively for our list method

	void deleteNode(node* n);				// Delete the subtrees of n and n itself afterwards

	void calculateHeight(node* currNode, unsigned long long pathHeight); // Method to calculate the height of the tree
	void calculateWords(unsigned long long& numWords, unsigned long long& numUniqueWords); // Method to calculate the number of words and unique words in the tree
	void calculateWords(node* start, unsigned long long& numWords, unsigned long long& numUniqueWords); // Method to calculate the number of words and unique words starting at a given node
};