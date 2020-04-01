/*
	File: RBT.cpp - Red-Black Tree Implementation
	c.f.: RBT.h

	This class implements the Red-Black Tree
	as declared in RBT.h. Each node contains a word, the number
	of times the word has appeared, a pointer to its parent and to its left
	and right children, as well as a "color", either Red, or Black
	used to adjust the balance of the tree when nodes are inserted.

	Author: Quinn Kleinfelter
	Class: EECS 2510-001 Non Linear Data Structures Spring 2020
	Instructor: Dr. Thomas
	Last Edited: 3/30/20
	Copyright: Copyright 2020 by Quinn Kleinfelter. All rights reserved.

*/

#include "RBT.h"
#include <iostream>
#include <string.h>

RBT::RBT()
{
	// Constructor
	nil = new node;			// Initialize our nil node 
	strcpy(nil->word, "");	// nil has an empty string as its word
	nil->color = BLACK;		// nil is ALWAYS black
	root = nil;				// We start off with the root being nil
}

RBT::~RBT()
{
	// Destructor
	if (root != nil)
	{
		// If the root isn't nil, call our helper delete on the root
		deleteNode(root);
	}
	delete nil; // Delete the nil node
	nil = nullptr;	// Set the nil node to nullptr
	root = nullptr; // Set the root node to nullptr
}

void RBT::deleteNode(node* n)
{
	// Recursive helper method to delete the subtrees 
	if (n->left != nil)
	{
		// If the left child isn't nil, recursively delete it
		deleteNode(n->left);
	}
	if (n->right != nil)
	{
		// If the right child isn't nil, recursively delete it
		deleteNode(n->right);
	}
	delete n; // Delete the current node
}

void RBT::insert(const char word[50])
{
	// To add a word to the tree, we first traverse the tree, looking for the word
	// If we find it, we increment the counter, print and return; otherwise we store the position where it belongs,
	// create a new node, and append it to the bottom of the tree in the appropriate position
	node* x = root; // Start at the root
	node* y = nil; // Y will lag one step behind x, to ensure we keep track of where we fall off the tree

	// We define this here so that we can use it the last comparison outside
	// of the while loop and avoid extra strcmp calls
	int compareVal;
	while (x != nil)
	{
		compareVal = strcmp(word, x->word);
		keyComparisons++;
		y = x; // Save our current location in y
		if (compareVal == 0)
		{
			// If the word in x matches our word we increment the count, and return
			x->count++;
			return;
		}
		// The word didn't match our word, so we check to see if the word we want to insert should go to the left or right of the current word,
		// and make x the pointer from our current words appropriate direction
		x = compareVal < 0 ? x->left : x->right;
	}
	// We made it here so we must not have found the word in the list
	// Therefore, create a new node to store the word
	node* newNode = new node;
	newNode->count = 1; // Redundantly set the count in the new node to 1
	strcpy(newNode->word, word); // Set the word in the new node to be the word we are adding to the list
	newNode->parent = y;
	ptrChanges++;
	newNode->color = RED; // We always add a new node as RED

	if (y == nil)
	{
		// y is only nil if the root was nil so our new node must be the first in the tree, therefore make it the root
		root = newNode;
		ptrChanges++;
	}
	else if (compareVal < 0)
	{
		// If newNode's word is less than the parents word, make it the left child
		y->left = newNode;
		ptrChanges++;
	}
	else
	{
		// Otherwise, we know it must go to the right of the parent
		y->right = newNode;
		ptrChanges++;
	}
	newNode->left = newNode->right = nil; // Both of the children of our new node must be nil
	insertFixup(newNode); // Insert any fixups we may need
}

void RBT::leftRotation(node* x)
{
	leftRot++;
	node* y = x->right; // y is x's right (non-nil) child
	x->right = y->left; // move y's left subtree into x's right subtree
	ptrChanges++;
	if (y->left != nil)
	{
		y->left->parent = x;
		ptrChanges++;
	}
	y->parent = x->parent; // Link x's parent to y
	ptrChanges++;
	if (x->parent == nil)			// If x has no parent, x was the root
	{								// so y becomes the new root
		root = y;
		ptrChanges++;

	}
	else if (x == x->parent->left)  // Otherwise (x has a parent), the
	{								// spot x used to occupy now
		x->parent->left = y;        
		ptrChanges++;
	}
	else                            // gets taken by y
	{								
		x->parent->right = y;       
		ptrChanges++;
	}
	y->left = x;                    // put x on y's left, which...
	x->parent = y;					// ...makes x's parent be y
	ptrChanges += 2;
}

void RBT::rightRotation(node* x)
{
	rightRot++;
	node* y = x->left;				// y is x's left (non-nil) child
	x->left = y->right;				// move y's right subtree into
	ptrChanges++;					// x's left subtree
	if (y->right != nil)
	{
		y->right->parent = x;
		ptrChanges++;
	}
	y->parent = x->parent;			// Link x's parent to y
	ptrChanges++;
	if (x->parent == nil)			// If has no parent, x was the root
	{								// so y becomes the new root
		root = y;
		ptrChanges++;
	}
	else if (x == x->parent->right)	// Otherwise (x has a parent), the 
	{								// spot x used to occupy now
		x->parent->right = y;
		ptrChanges++;
	}
	else
	{
		x->parent->left = y;		// gets taken by y
		ptrChanges++;
	}
	y->right = x;					// put x on y's right, which...
	x->parent = y;					// ...makes x's parent be y
	ptrChanges += 2;
}

void RBT::insertFixup(node* z)
{
	if (z->parent->color == BLACK) noFixesNeeded++; // If our node's parent is Black, we don't enter the while loop, and therefore don't make any fixes
	while (z->parent->color == RED)
	{
		if (z->parent == z->parent->parent->left)	// is z's parent a left child of its parent?
		{
			node* y = z->parent->parent->right;		// let y be z's (right) uncle
			if (y->color == RED)
			{
				case1Fix++;
				z->parent->color = BLACK;			// Case 1 (re-color only)
				y->color = BLACK;					// Case 1
				z->parent->parent->color = RED;		// Case 1
				recolorings += 3;					// Case 1
				z = z->parent->parent;				// Case 1
			}
			else // Handle Case 2 / 3 issues (we ALWAYS have a Case 3 if we fix a Case 2, but it can also happen independently)
			{
				if (z == z->parent->right)			// Is z the right child of its parent?
				{
					case2Fix++;
					z = z->parent;					// Case 2
					leftRotation(z);				// Case 2
				}
				case3Fix++;							
				z->parent->color = BLACK;			// Case 3
				z->parent->parent->color = RED;		// Case 3
				recolorings += 2;					// Case 3
				rightRotation(z->parent->parent);	// Case 3
			}
		}
		else                                        // Is z's parent a right child of its parent?
		{
			node* y = z->parent->parent->left;		// let y be z's (left) uncle
			if (y->color == RED)
			{
				case1Fix++;
				z->parent->color = BLACK;			// Case 1 (re-color only)
				y->color = BLACK;					// Case 1
				z->parent->parent->color = RED;		// Case 1
				recolorings += 3;					// Case 1
				z = z->parent->parent;				// Case 1
			}
			else // Handle Case 2 / 3 issues (we ALWAYS have a Case 3 if we fix a Case 2, but it can also happen independently)
			{
				if (z == z->parent->left)			// Is z the left child of its parent?
				{
					case2Fix++;
					z = z->parent;					// Case 2
					rightRotation(z);				// Case 2
				}
				case3Fix++;
				z->parent->color = BLACK;			// Case 3
				z->parent->parent->color = RED;		// Case 3
				recolorings += 2;					// Case 3
				leftRotation(z->parent->parent);	// Case 3
			}
		}
	}
	root->color = BLACK;	// Takes care of the potential "rule 2" violation
	recolorings++;
}

void RBT::list()
{
	// This method lists out our nodes in the tree through a recursive in-order traversal
	if (root == nil)
	{
		// If our root is null, then we must have an empty set, so print out as such and exit
		cout << "Set is empty" << endl;
		return;
	}
	// If we do have stuff in the tree, print out "Set contains: " per spec, before setting up an index variable and calling our recursive traversal method
	cout << "Set contains: ";
	int index = 0;
	traverse(index, root); // Starts our recursive traversal on the root, with an initial index of 0 (this is going to be incremented before each print, and we want an initial index of 1)
	cout << endl; // Print out an endline since we don't want to do so inside the traversal
}

void RBT::traverse(int& index, node* n)
{
	// This method does a recursive in-order traversal of the nodes in the tree to print them out with an index and their counts
	// We also pull in an index parameter, passed as a reference, so that no matter which recursive call we are in, the correct index will be printed with proper incrementing
	// This indexing also could have been implemented as a class variable in the BST, but I believe this passing an int by reference makes it cleaner since we don't have a class variable that is barely used
	if (n == nil) return; // If we have a null node, we don't have any work to do, simply return out
	if (n->left != nil)
	{
		// If our left child is not null, we want to recursively traverse that child
		traverse(index, n->left);
		cout << ", "; // We know that there will be something that comes after this, because our current node isn't null, so we want to add a comma after our left child is printed
	}
	cout << "(" << ++index << ") " << n->word << " " << n->count; // Print out the index (pre-incremented), followed by our word and its count
	if (n->right != nil)
	{
		// If our right child is not null, we want to recursively traverse that child
		cout << ", "; // We print the comma here, and not immediately after printing the current child, because we want to be sure that there is something to the right before we do so
		traverse(index, n->right);
	}
}

void RBT::displayStatistics()
{
	// This method is used to print out various statistics about
	// the work our RBT did
	cout << "---------------------------" << endl;
	cout << "RBT STATISTICS" << endl;
	cout << "Recolorings: " << recolorings << endl;
	cout << "Pointer Changes: " << ptrChanges << endl;
	cout << "Key Comparisons: " << keyComparisons << endl;
	cout << "Times no fixups were needed: " << noFixesNeeded << endl;
	cout << "Times we ran a left rotation: " << leftRot << endl;
	cout << "Times we ran a right rotation: " << rightRot << endl;
	cout << "Times we completed a Case 1 Fixup: " << case1Fix << endl;
	cout << "Times we completed a Case 2 Fixup: " << case2Fix << endl;
	cout << "Times we completed a Case 3 Fixup: " << case3Fix << endl;
	cout << "Tree Height: " << getHeight() << endl;

	unsigned long long numWords, numUniqueWords; // Create variables to store the number of words we have
	calculateWords(numWords, numUniqueWords);	 // Calculate the words using the variables we just made
	cout << "Number of words: " << numWords << endl;
	cout << "Number of unique words: " << numUniqueWords << endl;
}

unsigned long long RBT::getHeight()
{
	// This is a public method that calls our private
	// calculateHeight method, on the root, and then
	// returns the height of the tree
	calculateHeight(root, 0);
	return treeHeight;
}

void RBT::calculateHeight(node* currNode, unsigned long long pathHeight)
{
	// This is a recursive helper method used to calculate the height
	// of the tree underneath a given node, with a given pathHeight
	if (currNode->left != nil) calculateHeight(currNode->left, pathHeight + 1);	  // If we can go left, recursively go left and add one to the height
	if (currNode->right != nil) calculateHeight(currNode->right, pathHeight + 1); // If we can go right, recursively go right and add one to the height
	if (pathHeight > treeHeight) treeHeight = pathHeight; // If the current path height is greater than our tree height class variable, then set the tree height to it
}

void RBT::calculateWords(unsigned long long& numWords, unsigned long long& numUniqueWords)
{
	// This is a helper method used to calculate the total number of words & unique words in the tree
	numWords = 0;		// Make sure our variables are initialized to 0
	numUniqueWords = 0;

	// And calculate them using our recursive method on the root
	calculateWords(root, numWords, numUniqueWords);
}

void RBT::calculateWords(node* start, unsigned long long& numWords, unsigned long long& numUniqueWords)
{
	// This is a recursive helper method used to calculate the
	// total number of words & unique words in the tree
	if (start->left != nil) calculateWords(start->left, numWords, numUniqueWords);  // If we can go left, do so
	if (start->right != nil) calculateWords(start->right, numWords, numUniqueWords);// If we can go right, do so

	numWords += start->count; // Add the count in the current node to the total number of words
	numUniqueWords++;		  // Increment the number of unique words by 1 since we are at a new node
}