/*
	File: Skiplist.cpp - Skiplist Implementation
	c.f.: Skiplist.h

	This class implements the Skiplist
	as declared in Skiplist.h. Each node contains a word, the number
	of times the word has appeared, a pointer to its parent and to the
	left, right, up, and down, and whether or not the node is a sentinel.
	This data structure doesn't adjust balance of nodes that are inserted
	using rotations like AVL or RBT, but instead uses pure chance to
	occasionally add nodes to a "fast" lane above the bottom lane where all 
	words reside, allowing for quicker searching.

	Author: Quinn Kleinfelter
	Class: EECS 2510-001 Non Linear Data Structures Spring 2020
	Instructor: Dr. Thomas
	Last Edited: 3/28/20
	Copyright: Copyright 2020 by Quinn Kleinfelter. All rights reserved.

*/

#include "Skiplist.h"
#include <time.h>
#include <iostream>

Skiplist::Skiplist()
{
	// Constructor
	startTime = clock();		// Start work on the skiplist
	head = createSentinelNode();// Head is a sentinel node
	tail = createSentinelNode();// Tail is a sentinel node

	head->right = tail;			// Tail is to the right of the head
	tail->left = head;			// and head is to the left of the tail

	height = 1;					// At this point the height is 1 (1 lane)
	numItems = 0;				// and we have no items in the list, we don't count sentinels

	coin = mt19937(time(NULL)); // Initialize out Mersenne Twister PRNG
}

Skiplist::~Skiplist()
{
	// Destructor
	node* p = head;				// Start at the head
	while (p != nullptr)		// While p isn't nullptr
	{
		node* q = p->right;		// Q is the node directly to p's right
		while (q != nullptr)	// While q isn't nullptr
		{
			node* del = q;		// Keep track of the node we want to delete
			q = q->right;		// Move q to the right
			delete del;			// and delete del, which holds the original q
		}
		node* del = p;			// Keep track of the node we want to delete
		p = p->down;			// Move p downwards
		delete del;				// and delete del, which holds the original p
	}
}

Skiplist::node* Skiplist::createSentinelNode()
{
	// Helper method that creates and returns a sentinel node
	node* newNode = new node;	// Create a node
	newNode->isSentinel = true; // Make it a sentinel
	return newNode;				// and return it
}

Skiplist::node* Skiplist::find(const char word[50], bool& found)
{
	// Method to find a node with the given word in the skiplist
	// Returns either the node with the word in it, or its predecessor node
	// If found is set to true, the return value is the node with the word in it
	// otherwise, its the predecessor node
	node* p = head; // Start at p
	while (true)	// Loop forever
	{
		while (!p->right->isSentinel) // While the node to our right is not a sentinel
		{
			int compare = strcmp(p->right->word, word); // Compare the word in the node to our right with the word we want to insert
			keyComparisons++;
			if (compare < 0)	// Move to the right if our comparison was negative
			{
				p = p->right;
			}
			else if (compare == 0) // Otherwise, if the node to the right contains our word
			{
				p = p->right;	   // Move p to the right
				while (p->down != nullptr) // While we can go down from p
				{
					p = p->down; // Go down
				}
				found = true; // Set the found reference parameter to true
				return p;	  // Return the node p (which contains the word we were looking for, and is in the slow lane)
			}
			else
			{
				break; // Otherwise, break out of the inner while
			}
		}

		if (p->down == nullptr) // If we can't go down any further, we are in the slow lane
		{
			found = false;		// Set found to be false
			return p;			// And return p, which is the predecessor to the node we want
		}
		else
		{
			p = p->down;		// Otherwise go down
		}
	}
}

void Skiplist::insert(const char word[50])
{
	// Method to insert the word into our skiplist
	bool found = false;	// Setup a variable to keep track of if we find the word
	node* p = find(word, found); // Look for our word in the skiplist

	if (found) // If we found it
	{
		p->count++; // Increment the count in p
		return;		// and return
	}
	node* newNode = new node;	// Otherwise create a new node
	strcpy(newNode->word, word);// Copy our word into the new node

	newNode->left = p;			// Our node goes to the right of p, hence new node's left is p
	newNode->right = p->right;	// New node's right becomes p's old right
	ptrChanges += 2;
	newNode->isSentinel = false;// New node isn't a sentinel

	p->right->left = newNode;   // P's old right's left becomes the new node
	p->right = newNode;			// And p's right gets moved to the new node
	ptrChanges += 2;

	numItems++;					// Increment the number of items

	int currentHeight = 1;		// our current height is 1 (since we haven't built anything up yet)

	while (coin() & 1)			// While our coin flip gives us an odd number
	{
		currentHeight++;		// Increment the current height (we need to add a node on top)
		coinTosses++;			// And we "tossed heads" so increment that counter

		node* stackNode = new node;   // Node that gets stacked on top of the current node
		strcpy(stackNode->word, word);// Copy the word into our new node that gets stacked up
		stackNode->down = newNode;	  // The down pointer of our stack node is the newNode we just added
		newNode->up = stackNode;	  // And the new node we added's up pointer needs to go to the stack node
		ptrChanges += 2;

		if (currentHeight > height)	  // If the current height is greater than the height of our tree, we need to make a new lane
		{
			node* negInf = createSentinelNode(); // Create a new negative infinity sentinel node
			node* posInf = createSentinelNode(); // Create a new positive infinity sentinel node

			negInf->down = head;				 // Our new negative infinity points down to the current head
			head->up = negInf;					 // and our current head points up to the negative infinity
			ptrChanges += 2;

			posInf->down = tail;				 // Our new positive infinity points down to the current tail
			tail->up = posInf;					 // and our current tail points up to the positive infinity
			ptrChanges += 2;

			negInf->right = stackNode;			 // Our new negative infinity points right to the stacknode
			posInf->left = stackNode; 			 // Our new positive infinity points left to the stacknode
			stackNode->left = negInf;			 // Our stacknode points left to the negative infinity
			stackNode->right = posInf;			 // Our stacknode points right to the positive infinity
			ptrChanges += 4;

			head = negInf;						 // And our negative infinity node becomes the head
			tail = posInf;						 // Also our positive infinity node becomes the tail
			ptrChanges += 2;

			height++;							 // And the height of our tree gets incremented

		}
		else // Otherwise, we don't need to add a new level, but our stacknode still needs to get added on top
		{
			node* leftNode = newNode->left;		 // Start at the left of our new node
			while (leftNode->up == nullptr)		 // While we can't go up from there
			{
				leftNode = leftNode->left;		 // Go left
			}

			node* rightNode = newNode->right;	 // Start at the right of our new node
			while (rightNode->up == nullptr)	 // While we can't go up from there
			{
				rightNode = rightNode->right;	 // Go right
			}

			stackNode->left = leftNode->up;		 // Now our stacknode needs to point left to the left node's up pointer
			leftNode->up->right = stackNode;	 // And our left node's up pointer needs to point right to the stacknode
			stackNode->right = rightNode->up;	 // Our stacknode needs to point right to the right node's up pointer
			rightNode->up->left = stackNode;	 // And our right node's up pointer needs to point left to the stacknode
			ptrChanges += 4;
		}
		newNode = stackNode;					 // Finally, our new node becomes the stacknode, so we can stack on top of it appropriately
	}
}

void Skiplist::list()
{
	// Method to list out our skiplist nicely
	node* start = head; // Start at the head
	while (start->down != nullptr)
	{
		start = start->down; // Go down as far as we can here
	}
	
	node* end = tail; // Also keep track of the end
	while (end->down != nullptr)
	{
		end = end->down; // Go down as far as we can here
	}

	start = start->right; // The real start node is one to the right of where we are now
	int index = 0;		  // Our index is currently 0
	cout << "Set contains: ";

	while (start != end)  // While our start hasn't hit the end yet
	{
		cout << "(" << ++index << ") " << start->word << " " << start->count; // Print out the index and the word with its count

		if (index != numItems)	// If we aren't at the last item, print a comma
		{
			cout << ", ";
		}
		
		start = start->right; // Move start to the right
 	}
}

void Skiplist::stackedList()
{
	// This method prints out our skiplist stacked
	// With the new lanes to the right
	node* p = head; // Start at the head
	while (p->down != nullptr)
	{
		p = p->down; // Move p down as far as we can go
	}
	p = p->right; // and over one to get to our first actual node

	while (!p->isSentinel) // While we aren't at a sentinel node
	{
		node* q = p; // q is where p currently is
		do
		{
			cout << q->word << " "; // Print out the contents of q
			q = q->up;	// Move q upwards
		} while (q != nullptr); // and do it until q is nullptr
		cout << endl;
		p = p->right; // Next, move p to the right
	}
}

void Skiplist::displayStatistics()
{
	// This method is used to print out various statistics about
	// the work our Skiplist did
	clock_t endTime = clock(); // If we're displaying stats, we can finish out the timer since we aren't working on the tree anymore
	double secondsElapsed = (endTime - startTime) / 1000.0; // Calculate the elapsed time in seconds between the start and the end

	cout << "---------------------------" << endl;
	cout << "SKIPLIST STATISTICS" << endl;
	cout << "Pointer Changes: " << ptrChanges << endl;
	cout << "Key Comparisons: " << keyComparisons << endl;
	cout << "Coin Tosses that came up heads: " << coinTosses << endl;
	cout << "Height of list: " << height << endl;

	unsigned long long numWords, numUniqueWords; // Create variables to store the number of words we have
	calculateWords(numWords, numUniqueWords);	 // Calculate the words using the variables we just made

	cout << "Number of words: " << numWords << endl;
	cout << "Number of unique words / Number of Slow Lane Nodes: " << numUniqueWords << endl;
	cout << "Total number of nodes: " << countTotalNodes() << endl;
	cout << "Elapsed Time: " << secondsElapsed << " seconds." << endl;
	cout << "END SKIPLIST STATISTICS" << endl;
	cout << "---------------------------" << endl;
}

void Skiplist::calculateWords(unsigned long long& numWords, unsigned long long& numUniqueWords)
{
	// This is a helper method used to calculate the total number of words & unique words in the skiplist
	numWords = 0;		// Make sure our variables are initialized to 0
	numUniqueWords = 0;
	
	node* p = head;	// Start at the head

	while (p->down != nullptr) p = p->down; // Go down as far as we can
	while (!p->right->isSentinel) // While whats to the right isn't a sentinel
	{
		numWords += p->count;	  // The total number of words gets incremented by the count of p
		numUniqueWords++;		  // and our uniques go up by 1 because we are at a node we haven't seen yet
		
		p = p->right;			  // Then move to the right because we know its not a sentinel
	}
}

unsigned long long Skiplist::countTotalNodes()
{
	// This is a helper method used to count the total number of nodes in our skiplist
	node* p = head; // Start at the head
	unsigned long long totalNodes = 0; // Variable to keep track of the total nodes
	
	while (p != nullptr) // While p isn't null
	{
		node* q = p;	// Start q at p
		while (!q->right->isSentinel) // While q's right isn't a sentinel
		{
			totalNodes++;	// Increment toal nodes
			q = q->right;	// And move q to the right
		}

		p = p->down;	// Then move p down a layer
	}
	return totalNodes; // Finally, return the total number of nodes
}