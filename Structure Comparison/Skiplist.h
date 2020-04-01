/*
	Skiplist.h
	Author: Quinn Kleinfelter
	EECS 2510-001 Non Linear Data Structures Spring 2020
	Dr. Thomas
	Last Edited: 4/1/2020
	Header for Skiplist.cpp
*/

#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <string.h>
#include <time.h>
#include <random>
using namespace std;
class Skiplist
{
public:
	Skiplist(); // Constructor
	~Skiplist();// Destructor
	void insert(const char word[50]); // Inserts word into our skiplist
	void list(); // Lists out the skiplist
	void stackedList(); // Lists out the skiplist showing the different lanes (stacked to the side instead of up)
	void displayStatistics(); // Display statistics about the work our Skiplist has done
private:
	struct node
	{
		// Node struct used to build out the skiplist
		char word[50]; // Variable to hold the word for each node
		int count = 1; // Variable to hold the number of times we have seen the word in a given node
		node* left = nullptr;	// Left pointer variable
		node* right = nullptr;	// Right pointer variable
		node* up = nullptr;		// Up pointer variable
		node* down = nullptr;	// Down pointer variable
		bool isSentinel = false;// Variable to keep track of whether a given node is a sentinel
	};

	mt19937 coin; // Variable used for our Mersenne Twister PRNG
	node* head = nullptr; // Head of our skiplist, nullptr at first
	node* tail = nullptr; // Tail of our skiplist, nullptr at first
	int height;			  // Height of the skiplist
	int numItems;		  // Number of items in the skiplist
	unsigned long long ptrChanges = 0;		// Variable to keep track of the number of pointer changes we make
	unsigned long long keyComparisons = 0;	// Variable to keep track of the number of key comparisons we make
	unsigned long long coinTosses = 0;		// Variable to keep track of the number of coin tosses that come up heads

	node* find(const char word[50], bool& found);	// Find a given node in the skiplist, if we don't find it return the node before it

	node* createSentinelNode(); // Helper method to create a sentinel node

	void calculateWords(unsigned long long& numWords, unsigned long long& numUniqueWords); // Helper method to calculate the number of words and unique words in the skiplist
	unsigned long long countTotalNodes(); // Counts the total number of nodes in the skiplist
};