#include "RBT.h"
#include <iostream>
#include <string.h>

RBT::RBT()
{
	nil = new node;
	strcpy(nil->word, "");
	nil->color = BLACK;
	root = nil;
}

RBT::~RBT()
{

}

void RBT::insert(const char word[50])
{
	// To add a word to the tree, we first traverse the tree, looking for the word
	// If we find it, we increment the counter, print and return; otherwise we store the position where it belongs,
	// create a new node, and append it to the bottom of the tree in the appropriate position
	node* x = root; // Start at the root
	node* y = nil; // Y will lag one step behind x, to ensure we keep track of where we fall off the tree
	while (x != nil)
	{
		int compareVal = strcmp(x->word, word);
		y = x; // Save our current location in y
		if (compareVal == 0)
		{
			// If the word in x matches our word we increment the count, print and return
			x->count++;
			return;
		}
		// The word didn't match our word, so we check to see if the word we want to insert should go to the left or right of the current word,
		// and make x the pointer from our current words appropriate direction
		x = compareVal > 0 ? x->left : x->right;
	}
	// We made it here so we must not have found the word in the list
	// Therefore, create a new node to store the word
	node* newNode = new node;
	newNode->count = 1; // Redundantly set the count in the new node to 1
	strcpy(newNode->word, word); // Set the word in the new node to be the word we are adding to the list
	newNode->left = nil; // We only add new nodes as leaves so set the left and right pointers to null
	newNode->right = nil;
	newNode->parent = y;

	if (y == nil)
	{
		// y is only NULL if the root was null so our new node must be the first in the tree, therefore make it the root
		root = newNode;
		root->parent = nil;
	}
	else if (strcmp(newNode->word, y->word) < 0)
	{
		// If newNode's word is less than the parents word, make it the left child
		y->left = newNode;
	}
	else
	{
		// Otherwise, we know it must go to the right of the parent
		y->right = newNode;
	}
}

void RBT::leftRotation(node* x)
{
	node* y = x->right;
	x->right = y->left;
	if (y->left != nil)
		y->left->parent = x;
	y->parent = x->parent;
	if (x->parent == nil)
		root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;
}

void RBT::rightRotation(node* x)
{
	node* y = x->left;
	x->left = y->left;
	if (y->right != nil)
		y->right->parent = x;
	y->parent = x->parent;
	if (x->parent == nil)
		root = y;
	else if (x == x->parent->right)
		x->parent->right = y;
	else
		x->parent->left = y;
	y->right = x;
	x->parent = y;
}
