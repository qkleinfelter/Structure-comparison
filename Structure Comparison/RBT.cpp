#include "RBT.h"
#include <iostream>
#include <string.h>

RBT::RBT()
{
	startTime = clock();
	nil = new node;
	strcpy(nil->word, "");
	nil->color = BLACK;
	root = nil;
}

RBT::~RBT()
{
	if (root != nil)
	{
		deleteNode(root);
	}
	delete nil;
	nil = nullptr;
	root = nullptr;
}

void RBT::deleteNode(node* n)
{
	if (n->left != nil)
	{
		deleteNode(n->left);
	}
	if (n->right != nil)
	{
		deleteNode(n->right);
	}
	delete n;
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
		int compareVal = strcmp(word, x->word);
		keyComparisons++;
		y = x; // Save our current location in y
		if (compareVal == 0)
		{
			// If the word in x matches our word we increment the count, print and return
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

	if (y == nil)
	{
		// y is only NULL if the root was null so our new node must be the first in the tree, therefore make it the root
		root = newNode;
		ptrChanges++;
	}
	else if (strcmp(newNode->word, y->word) < 0)
	{
		keyComparisons++;
		// If newNode's word is less than the parents word, make it the left child
		y->left = newNode;
		ptrChanges++;
	}
	else
	{
		keyComparisons++;
		// Otherwise, we know it must go to the right of the parent
		y->right = newNode;
		ptrChanges++;
	}
	newNode->left = newNode->right = nil;
	ptrChanges += 2;
	newNode->color = RED;
	insertFixup(newNode);
}

void RBT::leftRotation(node* x)
{
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
	{								// put x on y's left, which...
		x->parent->right = y;       
		ptrChanges++;
	}
	y->left = x;                    // ...makes x's parent be y
	x->parent = y;
	ptrChanges += 2;
}

void RBT::rightRotation(node* x)
{
	node* y = x->left;
	x->left = y->right;
	ptrChanges++;
	if (y->right != nil)
	{
		y->right->parent = x;
		ptrChanges++;
	}
	y->parent = x->parent;
	ptrChanges++;
	if (x->parent == nil)
	{
		root = y;
		ptrChanges++;
	}
	else if (x == x->parent->right)
	{
		x->parent->right = y;
		ptrChanges++;
	}
	else
	{
		x->parent->left = y;
		ptrChanges++;
	}
	y->right = x;
	x->parent = y;
	ptrChanges += 2;
}

void RBT::insertFixup(node* z)
{
	while (z->parent->color == RED)
	{
		if (z->parent == z->parent->parent->left)
		{
			node* y = z->parent->parent->right;
			if (y->color == RED)
			{
				case1Fix++;
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				recolorings += 3;
				z = z->parent->parent;
			}
			else 
			{
				if (z == z->parent->right) 
				{
					case2Fix++;
					z = z->parent;
					leftRotation(z);
				}
				case3Fix++;
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				recolorings += 2;
				rightRotation(z->parent->parent);
			}
		}
		else
		{
			node* y = z->parent->parent->left;
			if (y->color == RED)
			{
				case1Fix++;
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				recolorings += 3;
				z = z->parent->parent;
			}
			else 
			{
				if (z == z->parent->left)
				{
					case2Fix++;
					z = z->parent;
					rightRotation(z);
				}
				case3Fix++;
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				recolorings += 2;
				leftRotation(z->parent->parent);
			}
		}
	}
	root->color = BLACK;
	recolorings++; // Do we need to check if the root was already black before incrementing here? -- ask larry
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

void RBT::print2D()
{
	print2DUtil(root, 0);
}

void RBT::print2DUtil(node* start, int space)
{
	if (start == nil) return;
	space += COUNT;
	print2DUtil(start->right, space);

	cout << endl;
	for (int i = COUNT; i < space; i++)
		cout << " ";
	cout << start->word << endl;

	print2DUtil(start->left, space);
}

void RBT::displayStatistics()
{
	clock_t endTime = clock();
	double secondsElapsed = (endTime - startTime) / 1000;
	cout << "---------------------------" << endl;
	cout << "RBT STATISTICS" << endl;
	cout << "Recolorings: " << recolorings << endl;
	cout << "Pointer Changes: " << ptrChanges << endl;
	cout << "Key Comparisons: " << keyComparisons << endl;
	cout << "Times no fixups were needed: " << noFixesNeeded << endl; // Need to actually adjust this
	cout << "Times we completed a Case 1 Fixup: " << case1Fix << endl;
	cout << "Times we completed a Case 2 Fixup: " << case2Fix << endl;
	cout << "Times we completed a Case 3 Fixup: " << case3Fix << endl;
	cout << "Tree Height: " << getHeight() << endl;

	unsigned long long numWords, numUniqueWords;
	calculateWords(numWords, numUniqueWords);
	cout << "Number of words: " << numWords << endl;
	cout << "Number of unique words: " << numUniqueWords << endl;

	cout << "Elapsed Time: " << secondsElapsed << " seconds." << endl;
	cout << "END RBT STATISTICS" << endl;
	cout << "---------------------------" << endl;
}

unsigned long long RBT::getHeight()
{
	calculateHeight(root, 0);
	return treeHeight;
}

void RBT::calculateHeight(node* currNode, unsigned long long pathHeight)
{
	if (currNode->left != nil) calculateHeight(currNode->left, pathHeight + 1);
	if (currNode->right != nil) calculateHeight(currNode->right, pathHeight + 1);
	if (pathHeight > treeHeight) treeHeight = pathHeight;
}

void RBT::calculateWords(unsigned long long& numWords, unsigned long long& numUniqueWords)
{
	numWords = 0;
	numUniqueWords = 0;

	calculateWords(root, numWords, numUniqueWords);
}

void RBT::calculateWords(node* start, unsigned long long& numWords, unsigned long long& numUniqueWords)
{
	if (start->left != nil) calculateWords(start->left, numWords, numUniqueWords);
	if (start->right != nil) calculateWords(start->right, numWords, numUniqueWords);

	numWords += start->count;
	numUniqueWords++;
}