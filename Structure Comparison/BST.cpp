#include "BST.h"
#include <iostream>

BST::BST()
{
	// Constructor, nothing to do here except make sure we don't have a root already.
	// This is redundant since it defaults to NULL, but we do it anyway for clarity
	root = nullptr; // New Trees don't have a root until we insert an item.
}

BST::~BST()
{
	// Deletes the node at the root of the tree, and all nodes in its subtrees
	deleteNode(root);
	root = nullptr; // After we have deleted all of our nodes make the root of the tree null
}

void BST::deleteNode(node* n)
{
	// This method uses a post-order traversal of all of the subtrees of node n to delete each node.
	// We must delete the nodes in the left and right subtrees first to ensure that we still have access to them
	// If we had deleted n at the beginning (pre-order) or in the middle (in-order), we would not have access to the left and/or right subtrees to delete anymore
	if (n->left != nullptr) deleteNode(n->left); // If the left subtree isn't null, recursively delete it
	if (n->right != nullptr) deleteNode(n->right); // If the right subtree isn't null, recursively delete it
	delete n; // After we've deleted the subtrees, delete n
}

void BST::insert(const char word[50])
{
	// To add a word to the tree, we first traverse the tree, looking for the word
	// If we find it, we increment the counter, print and return; otherwise we store the position where it belongs,
	// create a new node, and append it to the bottom of the tree in the appropriate position
	node* x = root; // Start at the root
	node* y = nullptr; // Y will lag one step behind x, to ensure we keep track of where we fall off the tree
	while (x != nullptr)
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
	newNode->left = NULL; // We only add new nodes as leaves so set the left and right pointers to null
	newNode->right = NULL;

	if (y == NULL)
	{
		// y is only NULL if the root was null so our new node must be the first in the tree, therefore make it the root
		root = newNode;
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

void BST::list()
{
	// This method lists out our nodes in the tree through a recursive in-order traversal
	if (root == NULL)
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

void BST::traverse(int& index, node* n)
{
	// This method does a recursive in-order traversal of the nodes in the tree to print them out with an index and their counts
	// We also pull in an index parameter, passed as a reference, so that no matter which recursive call we are in, the correct index will be printed with proper incrementing
	// This indexing also could have been implemented as a class variable in the BST, but I believe this passing an int by reference makes it cleaner since we don't have a class variable that is barely used
	if (n == NULL) return; // If we have a null node, we don't have any work to do, simply return out
	if (n->left != NULL)
	{
		// If our left child is not null, we want to recursively traverse that child
		traverse(index, n->left);
		cout << ", "; // We know that there will be something that comes after this, because our current node isn't null, so we want to add a comma after our left child is printed
	}
	cout << "(" << ++index << ") " << n->word << " " << n->count; // Print out the index (pre-incremented), followed by our word and its count
	if (n->right != NULL)
	{
		// If our right child is not null, we want to recursively traverse that child
		cout << ", "; // We print the comma here, and not immediately after printing the current child, because we want to be sure that there is something to the right before we do so
		traverse(index, n->right);
	}
}