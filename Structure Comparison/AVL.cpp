#include "AVL.h"
#include <iostream>

AVL::AVL()
{
	startTime = clock();
	root = nullptr;
}

AVL::~AVL()
{
	if (root == nullptr)
	{
		return;
	}
	deleteNode(root);
	root = nullptr;
}

void AVL::deleteNode(node* n)
{
	if (n->left != nullptr)
	{
		deleteNode(n->left);
	}
	if (n->right != nullptr)
	{
		deleteNode(n->right);
	}
	delete n;
}

void AVL::insert(const char word[50]) // lecture 12 slides 51+
{
	//cout << "Inserting " << word << " into the AVL Tree" << endl;
	node* Y;
	node* A, *B, *F;
	node* P, *Q;
	node* C, * CL, * CR;
	int d;

	if (root == nullptr)
	{
		Y = new node;
		strcpy(Y->word, word);
		Y->left = Y->right = nullptr;
		Y->balanceFactor = 0;
		root = Y;
		return;
	}

	F = Q = nullptr;
	A = P = root;

	while (P != nullptr)
	{
		int compare = strcmp(word, P->word);
		keyComparisons++;
		if (compare == 0)
		{
			P->count++;
			return;
		}
		if (P->balanceFactor != 0)
		{
			A = P;
			F = Q;
		}
		Q = P;
		P = (compare < 0) ? P->left : P->right;
	}

	Y = new node;
	strcpy(Y->word, word);
	Y->left = Y->right = nullptr;
	Y->balanceFactor = 0;

	if (strcmp(word, Q->word) < 0)
	{
		keyComparisons++;
		Q->left = Y;
		ptrChanges++;
	}
	else
	{
		keyComparisons++;
		Q->right = Y;
		ptrChanges++;
	}

	if (strcmp(word, A->word) > 0)
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

	while (P != Y)
	{
		if (strcmp(word, P->word) > 0)
		{
			keyComparisons++;
			P->balanceFactor = -1;
			bfChanges++;
			P = P->right;
		}
		else
		{
			keyComparisons++;
			P->balanceFactor = +1;
			bfChanges++;
			P = P->left;
		}
	}

	if (A->balanceFactor == 0)
	{
		A->balanceFactor = d;
		bfChanges++;
		return;
	}

	if (A->balanceFactor == -d)
	{
		A->balanceFactor = 0;
		bfChanges++;
		return;
	}

	if (d == +1)
	{
		if (B->balanceFactor == +1) // LL ROTATION
		{
			llRot++;
			//cout << "LL Rot" << endl;
			//cout << "a's word " << A->word << " and bf " << A->balanceFactor << endl;
			//cout << "b's word " << B->word << " and bf " << B->balanceFactor << endl;

			A->left = B->right;
			B->right = A;
			ptrChanges += 2;
			A->balanceFactor = B->balanceFactor = 0;
			bfChanges += 2;
		}
		else // LR Rotation: 3 cases
		{
			lrRot++;
			C = B->right;
			CL = C->left;
			CR = C->right;

			//cout << "LR Rot" << endl;
			//cout << "a's word " << A->word << " and bf " << A->balanceFactor << endl;
			//cout << "b's word " << B->word << " and bf " << B->balanceFactor << endl;
			//cout << "c's word " << C->word << " and bf " << C->balanceFactor << endl;

			C->left = B;
			C->right = A;
			B->right = CL;
			A->left = CR;
			ptrChanges += 4;

			switch (C->balanceFactor)
			{
			case 0:
				A->balanceFactor = B->balanceFactor = 0;
				break;
			case -1:
				B->balanceFactor = +1;
				A->balanceFactor = 0;
				break;
			case 1:
				B->balanceFactor = 0;
				A->balanceFactor = -1;
				break;
			}

			C->balanceFactor = 0;
			bfChanges += 3;
			B = C; // Is this a pointer change?
		} // end of else (LR Rotation)
	} // end of if (d = +1)
	else // d = -1. This is a right imbalance
	{
		// (RR or RL)
		// SYMMETRIC TO LEFT BALANCE
		if (B->balanceFactor == -1) // RR Rotation
		{
			rrRot++;
			//cout << "RR Rot" << endl;
			//cout << "a's word " << A->word << " and bf " << A->balanceFactor << endl;
			//cout << "b's word " << B->word << " and bf " << B->balanceFactor << endl;

			A->right = B->left;
			B->left = A;
			ptrChanges += 2;
			A->balanceFactor = B->balanceFactor = 0;
			bfChanges += 2;
		}
		else // RL Rotation: 3 cases
		{
			rlRot++;
			C = B->left;
			CL = C->left;
			CR = C->right;

			//cout << "RL Rot" << endl;
			//cout << "a's word " << A->word << " and bf " << A->balanceFactor << endl;
			//cout << "b's word " << B->word << " and bf " << B->balanceFactor << endl;
			//cout << "c's word " << C->word << " and bf " << C->balanceFactor << endl;

			C->right = B;
			C->left = A;
			B->left = CR;
			A->right = CL;
			ptrChanges += 4;

			switch (C->balanceFactor)
			{
			case 0:
				A->balanceFactor = B->balanceFactor = 0;
				break;
			case -1:
				A->balanceFactor = +1;
				B->balanceFactor = 0;
				break;
			case 1:
				A->balanceFactor = 0;
				B->balanceFactor = -1;
				break;
			}

			C->balanceFactor = 0;
			bfChanges += 3;
			B = C; // Is this a pointer change?
		} // end of else (RL Rotation)
	}

	// Finish up:
	if (F == nullptr)
	{
		root = B;
		ptrChanges++;
		return;
	}

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

void AVL::print2D()
{
	print2DUtil(root, 0);
}

void AVL::print2DUtil(node* start, int space)
{
	if (start == nullptr) return;
	space += COUNT;
	print2DUtil(start->right, space);

	cout << endl;
	for (int i = COUNT; i < space; i++)
		cout << " ";
	cout << start->word << "(" << start->balanceFactor << ")" << endl;

	print2DUtil(start->left, space);
}

void AVL::displayStatistics()
{
	cout << "---------------------------" << endl;
	cout << "AVL STATISTICS" << endl;
	clock_t endTime = clock();
	cout << "Balance Factor Changes: " << bfChanges << endl;
	cout << "Pointer Changes: " << ptrChanges << endl;
	cout << "Key Comparisons: " << keyComparisons << endl;
	cout << "Times no rotations were needed: " << noRotNeeded << endl; // need to actually increment this
	cout << "Times we completed a LL Rotation: " << llRot << endl;
	cout << "Times we completed a LR Rotation: " << lrRot << endl;
	cout << "Times we completed a RL Rotation: " << rlRot << endl;
	cout << "Times we completed a RR Rotation: " << rrRot << endl;
	double secondsElapsed = difftime(endTime, startTime) / 1000;
	cout << "Elapsed Time: " << secondsElapsed << " seconds." << endl;
	cout << "END AVL STATISTICS" << endl;
	cout << "---------------------------" << endl;
}