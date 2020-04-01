/*
	File: AVL.cpp - Adelson-Velsky and Landis Tree Implementation
	c.f.: AVL.h

	This class implements the Adelson-Velsky and Landis Tree
	as declared in AVL.h. Each node contains a word, the number
	of times the word has appeared, a pointer to its parent and to its left
	and right children, as well as a balance factor used to adjust
	the balance of the tree when nodes are inserted.

	Author: Quinn Kleinfelter
	Class: EECS 2510-001 Non Linear Data Structures Spring 2020
	Instructor: Dr. Thomas
	Last Edited: 4/1/20
	Copyright: Copyright 2020 by Quinn Kleinfelter. All rights reserved.

*/

#include "AVL.h"
#include <iostream>

AVL::AVL()
{
	// Constructor
	root = nullptr;		 // and make sure the root of our tree is null (redundant)
}

AVL::~AVL()
{
	// Destructor
	if (root == nullptr) // If the root is null, we don't have anything to delete
	{
		return;
	}
	deleteNode(root); // Otherwise, call deleteNode on the root
	root = nullptr;   // and set the root to null
}

void AVL::deleteNode(node* n)
{
	// Recursive helper function to delete the nodes of the tree
	if (n->left != nullptr) // If the left child isn't null, delete it
	{
		deleteNode(n->left);
	}
	if (n->right != nullptr) // If the right child isn't null, delete it
	{
		deleteNode(n->right);
	}
	delete n; // Finally, safely delete the current node since its children have been deleted
}

void AVL::insert(const char word[50]) // lecture 12 slides 51+
{
	node* Y;				// The new node we insert
	node* A, *B, *F;		// see below...
	node* P, *Q;			// ...
	node* C, * CL, * CR;	// ... for description
	int d;					// displacement; used to adjust BFs

	if (root == nullptr)	// Empty tree? Make a root node and exit!
	{
		Y = new node;		// Make and fill a node
		strcpy(Y->word, word);
		Y->left = Y->right = nullptr; // Leaf --> no children
		Y->balanceFactor = 0; // it is, by definition, balanced
		root = Y;			  // root was NULL, so Y is new root
		ptrChanges++;		  // We changed root, so increment pointer changes
		noRotNeeded++;		  // No rotations were needed here
		return;				  // This was the trivial case
	}

	// Locate insertion point for X.
	// P scans through the tree until it falls off bottom 
	// Q is P's parent (Q lags one step behind P)
	// The new Node Y will be added as either the left or right child of Q
	// A is the last parent above Y with a BF of +/- (before the insert)
	// F is A's parent (F lags one step behind A)
	//

	F = Q = nullptr; // F and Q lag, so they start as nullptr
	A = P = root;    // A and P start at the root

	// We define this here so that we can use it the last comparison outside
	// of the while loop and avoid extra strcmp calls
	int compare;

	while (P != nullptr) // search tree for insertion point
	{
		compare = strcmp(word, P->word); // Do a comparison on the word we are looking for and the word in the current node
		keyComparisons++;
		if (compare == 0)
		{
			// If the words are the same, increment the count and return
			P->count++;
			return;
		}
		if (P->balanceFactor != 0) // Remember the last place we saw
		{						   // a non-zero BF (and its parent)
			A = P;
			F = Q;
		}
		Q = P;					   // Bring Q up to where P is
		P = (compare < 0) ? P->left : P->right; // and then advance P
	}

	// At this point, P is NULL, but Q points at the last node where X
	// belongs (either as Q's left or right child, and Q points at an existing leaf)
	//

	Y = new node;				  // Make a new node
	strcpy(Y->word, word);		  // Put our data in it
	Y->left = Y->right = nullptr; // New nodes are always inserted as leaves
	Y->balanceFactor = 0;		  // Leaves are balanced by definition

	// Will Y be Q's new left or right child?
	if (compare < 0)
	{
		Q->left = Y;
		ptrChanges++;
	}
	else
	{
		Q->right = Y;
		ptrChanges++;
	}

	// So far, except for keeping track of F and A, we have done exactly 
	// the same "dumb" BST insert we've seen before. Now it's time to do
	// the AVL-specific stuff: detect (and fix, if we have) an imbalance

	// Adjust BFs from A to Q
	// A was the LAST ancestor with a BF of +/- 1, (or is still the root,
	// because we never FOUND a BF of +/- 1), so ALL nodes between A and Q
	// must have a BF of 0, and will, therefore, BECOME +/- 1.
	//
	// If the word is inserted in the LEFT subtree of A, then d = +1 (d = -1 means
	// we inserted the word in the RIGHT subtree of A.)

	if (strcmp(word, A->word) > 0) // Which way is the displacement (d)
								   // B is identified as A's child
	{
		keyComparisons++;
		B = P = A->right;
		d = -1;
	}
	else
	{
		keyComparisons++;
		B = P = A->left;
		d = +1;
	}
	
	aToYPasses++;

	while (P != Y)  // P is now one node below A. Adjust from here to the
	{				// insertion point. Don't do anything to new node (Y)
		if (strcmp(word, P->word) > 0) // Adjust balance factors and move forward
		{
			keyComparisons++;
			P->balanceFactor = -1;
			bfChanges++;
			P = P->right;
			aToYBfChanges++;
		}
		else
		{
			keyComparisons++;
			P->balanceFactor = +1;
			bfChanges++;
			P = P->left;
			aToYBfChanges++;
		}
	}

	// Now we check the BF at A and see if we just pushed the tree INTO
	// BALANCE, into an "unacceptable IMBALANCE", or if it is still 
	// "BALANCED ENOUGH".

	if (A->balanceFactor == 0) // Tree was completely balanced before the insert.
	{						   // The insert pushed it to slight (acceptable) imbalance
		A->balanceFactor = d;  // Set the BF to +/- 1 (displacement tells direction)
		bfChanges++;
		noRotNeeded++;
		return;				   // This is close enough to live with, so exit now
	}

	if (A->balanceFactor == -d) // If the tree had a slight imbalance the OTHER way,
	{							// then did the insertion throw the tree INTO complete
		A->balanceFactor = 0;	// balance? if so, set the BF to zero and we're done
		bfChanges++;
		noRotNeeded++;
		return;
	}

	// If we took neither of the 2 returns just above, then the tree WAS
	// acceptably imbalanced, and is now UNACCEPTABLY IMBALANCED. Which 
	// rotation type do we need to apply?

	if (d == +1) // This is a left imbalance (left subtree too tall)
	{			 // Is it LL or LR?
		if (B->balanceFactor == +1) // LL ROTATION
		{
			llRot++;

			A->left = B->right; // A's takes B's former right subtree as its new left subtree
			B->right = A;		// A gets moved into B's right subtree
			ptrChanges += 2;
			A->balanceFactor = B->balanceFactor = 0; // Adjust their balance factors to be 0 since we fixed them
			bfChanges += 2;
		}
		else // LR Rotation: 3 cases
		{
			lrRot++;
			C = B->right; // C is B's right child
			CL = C->left; // CL and CR are C's left and right children
			CR = C->right;

			C->left = B; // C gets B as its new left subtree
			C->right = A;// and A as its new right subtree
			B->right = CL;// B's right subtree becomes C's old left subtree
			A->left = CR;// and A's left subtree becomes C's old right subtree
			ptrChanges += 4;

			switch (C->balanceFactor) // Depending on C's balance factor, we need to adjust the others
			{
			case 0:
				A->balanceFactor = B->balanceFactor = 0; // If C was in balance before, A and B are now as well
				bfChanges += 2;
				break;
			case -1:					// If C had an extra layer to its right before
				B->balanceFactor = +1;  // then B gets a balance factor of +1 
				A->balanceFactor = 0;   // and A goes to 0
				C->balanceFactor = 0;
				bfChanges += 3;
				break;
			case 1:						// If C had an extra layer to its left before 
				B->balanceFactor = 0;	// Then B goes to 0
				A->balanceFactor = -1;  // And A gets a balance factor of -1
				C->balanceFactor = 0;
				bfChanges += 3;
				break;
			}

			B = C;						// B is the root of the now-reblanced subtree (recycle)
		} // end of else (LR Rotation)
	} // end of if (d = +1)
	else // d = -1. This is a right imbalance
	{
		// (RR or RL)
		// SYMMETRIC TO LEFT BALANCE
		if (B->balanceFactor == -1) // RR Rotation
		{
			rrRot++;

			A->right = B->left; // A's takes B's former left subtree as its new right subtree
			B->left = A;		// A gets moved into B's right subtree
			ptrChanges += 2;
			A->balanceFactor = B->balanceFactor = 0; // Adjust their balance factors to be 0 since we fixed them
			bfChanges += 2;
		}
		else // RL Rotation: 3 cases
		{
			rlRot++;
			C = B->left;  // C is B's left child
			CL = C->left; // CL and CR are C's left and right children
			CR = C->right;

			C->right = B; // C gets B as its new right subtree
			C->left = A;  // and A as its new left subtree
			B->left = CR; // B's left subtree becomes C's old right subtree
			A->right = CL;// and A's right subtree becomes C's old left subtree
			ptrChanges += 4;

			switch (C->balanceFactor) // Depending on C's balance factor, we need to adjust the others
			{
			case 0:
				A->balanceFactor = B->balanceFactor = 0; // If C was in balance before, A and B are now as well
				bfChanges += 2;
				break;
			case -1:					// If C had an extra layer to its right before
				A->balanceFactor = +1;  // Then A gets a balance factor of +1
				B->balanceFactor = 0;   // and B gets a balance factor of 0
				C->balanceFactor = 0;
				bfChanges += 3;
				break;
			case 1:						// If C had an extra layer to its left before
				A->balanceFactor = 0;   // Then A goes to 0
				B->balanceFactor = -1;  // and B goes to -1
				C->balanceFactor = 0;
				bfChanges += 3;
				break;
			}

			B = C; // B is the root of the now-rebalanced subtree (recycle)
		} // end of else (RL Rotation)
	}

	// Finish up:
	//
	// Regardless, the subtree rooted at B has been rebalanced, and needs to 
	// be the new child of F. The original subtree of F had root A.

	// did we rebalance the whole tree's root?
	if (F == nullptr)
	{
		root = B; // B is the tree's new root - done
		ptrChanges++;
		return;
	}

	// The root of what we rebalanced WAS A; now it's B. If the subtree we
	// rebalanced was LEFT of F, then B needs to be left of F;
	// if A was RIGHT of F, then B now needs to be right of F.
	if (A == F->left)
	{
		F->left = B;
		ptrChanges++;
		return;
	}
	if (A == F->right)
	{
		F->right = B;
		ptrChanges++;
		return;
	}
	cout << "We should never get here" << endl;
}

void AVL::list()
{
	// This method lists out our nodes in the tree through a recursive in-order traversal
	if (root == nullptr)
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

void AVL::traverse(int& index, node* n)
{
	// This method does a recursive in-order traversal of the nodes in the tree to print them out with an index and their counts
	// We also pull in an index parameter, passed as a reference, so that no matter which recursive call we are in, the correct index will be printed with proper incrementing
	// This indexing also could have been implemented as a class variable in the BST, but I believe this passing an int by reference makes it cleaner since we don't have a class variable that is barely used
	if (n == nullptr) return; // If we have a null node, we don't have any work to do, simply return out
	if (n->left != nullptr)
	{
		// If our left child is not null, we want to recursively traverse that child
		traverse(index, n->left);
		cout << ", "; // We know that there will be something that comes after this, because our current node isn't null, so we want to add a comma after our left child is printed
	}
	cout << "(" << ++index << ") " << n->word << " " << n->count; // Print out the index (pre-incremented), followed by our word and its count
	if (n->right != nullptr)
	{
		// If our right child is not null, we want to recursively traverse that child
		cout << ", "; // We print the comma here, and not immediately after printing the current child, because we want to be sure that there is something to the right before we do so
		traverse(index, n->right);
	}
}

void AVL::displayStatistics()
{
	// This method is used to print out various statistics about
	// the work our AVL tree did
	cout << "---------------------------" << endl;
	cout << "AVL STATISTICS" << endl;
	cout << "Balance Factor Changes: " << bfChanges << endl;
	cout << "Pointer Changes: " << ptrChanges << endl;
	cout << "Key Comparisons: " << keyComparisons << endl;
	cout << "Times no rotations were needed: " << noRotNeeded << endl;
	cout << "Times we completed a LL Rotation: " << llRot << endl;
	cout << "Times we completed a LR Rotation: " << lrRot << endl;
	cout << "Times we completed a RL Rotation: " << rlRot << endl;
	cout << "Times we completed a RR Rotation: " << rrRot << endl;
	cout << "Tree Height: " << getHeight() << endl;
	cout << "A to Y Passes: " << aToYPasses << endl;
	cout << "A to Y Balance Factor Changes: " << aToYBfChanges << endl;

	unsigned long long numWords, numUniqueWords; // Create variables to store the number of words we have
	calculateWords(numWords, numUniqueWords);	 // Calculate the words using the variables we just made
	cout << "Number of words: " << numWords << endl;
	cout << "Number of unique words: " << numUniqueWords << endl;
}

unsigned long long AVL::getHeight()
{
	// This is a public method that calls our private
	// calculateHeight method, on the root, and then
	// returns the height of the tree
	calculateHeight(root, 0);
	return treeHeight;
}

void AVL::calculateHeight(node* currNode, unsigned long long pathHeight)
{
	// This is a recursive helper method used to calculate the height
	// of the tree underneath a given node, with a given pathHeight
	if (currNode->left != nullptr) calculateHeight(currNode->left, pathHeight + 1);	  // If we can go left, recursively go left and add one to the height
	if (currNode->right != nullptr) calculateHeight(currNode->right, pathHeight + 1); // If we can go right, recursively go right and add one to the height
	if (pathHeight > treeHeight) treeHeight = pathHeight; // If the current path height is greater than our tree height class variable, then set the tree height to it
}

void AVL::calculateWords(unsigned long long& numWords, unsigned long long& numUniqueWords)
{
	// This is a helper method used to calculate the total number of words & unique words in the tree
	numWords = 0;		// Make sure our variables are initialized to 0
	numUniqueWords = 0;

	// And calculate them using our recursive method on the root
	calculateWords(root, numWords, numUniqueWords);
}

void AVL::calculateWords(node* start, unsigned long long& numWords, unsigned long long& numUniqueWords)
{
	// This is a recursive helper method used to calculate the
	// total number of words & unique words in the tree
	if (start->left != nullptr) calculateWords(start->left, numWords, numUniqueWords);  // If we can go left, do so
	if (start->right != nullptr) calculateWords(start->right, numWords, numUniqueWords);// If we can go right, do so
	
	numWords += start->count; // Add the count in the current node to the total number of words
	numUniqueWords++;		  // Increment the number of unique words by 1 since we are at a new node
}