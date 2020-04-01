/*
	BST.h
	Author: Quinn Kleinfelter
	EECS 2510-001 Non Linear Data Structures Spring 2020
	Dr. Thomas
	Last Edited: 4/1/2020
	Header for BST.cpp
*/

#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <string.h>
#include <time.h>
using namespace std;
class BST
{
public:
	BST(); // Constructor
	~BST(); // Destructor
	void insert(const char word[50]); // Inserts word into the tree
	void list(); // Lists the contents of the tree
	unsigned long long getHeight(); // Returns the height of the tree
	void displayStatistics(); // Displays statistics about the work we did with the tree
private:
	struct node
	{
		// Node struct that makes up the tree
		char word[50]; // The word for the given node
		int count = 1; // The amount of times we've seen the word, 1 by default
		node* left = nullptr;  // The left child pointer
		node* right = nullptr; // The right child pointer
	};
	
	node* root = nullptr; // The root of the tree, nullptr by default
	unsigned long long ptrChanges = 0; // Variable to keep track of the amount of times we change a pointer inside a node
	unsigned long long keyComparisons = 0; // Variable to keep track of the amount of times we make a key comparison
	unsigned long long treeHeight = 0; // Variable to keep track of the height of the tree

	void traverse(int& index, node* n); // Recursive helper function used for printing the tree nicely
	void calculateHeight(node* currNode, unsigned long long pathHeight); // Method to calculate the height of the tree
	void calculateWords(unsigned long long& numWords, unsigned long long& numUniqueWords); // Method to calculate the number of words and the number of unique words in the tree
	void calculateWords(node* start, unsigned long long& numWords, unsigned long long& numUniqueWords); // Method to calculate the number of words and unique words starting at a given node

	void deleteNode(node* n); // Method to delete a node and its subtrees (recursive)
};