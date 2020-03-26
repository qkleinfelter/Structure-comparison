#include "AVL.h"
#include <iostream>

AVL::AVL()
{
	root = nullptr;
}

AVL::~AVL()
{

}

void AVL::insert(const char word[50]) // lecture 12 slides 51+
{
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
		if (compare == 0) return;
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
		Q->left = Y;
	}
	else
	{
		Q->right = Y;
	}

	if (strcmp(word, A->word) > 0)
	{
		B = P = A->right;
		d = -1;
	}
	else
	{
		B = P = A->left;
		d = +1;
	}

	while (P != Y)
	{
		if (strcmp(word, P->word) > 0)
		{
			P->balanceFactor = -1;
			P = P->right;
		}
		else
		{
			P->balanceFactor = +1;
			P = P->left;
		}
	}

	if (A->balanceFactor == 0)
	{
		A->balanceFactor = d;
		return;
	}

	if (A->balanceFactor == -d)
	{
		A->balanceFactor = 0;
		return;
	}

	if (d == +1)
	{
		if (B->balanceFactor == +1) // LL ROTATION
		{
			B->right = A;
			A->left = nullptr;
			A->balanceFactor = B->balanceFactor = 0;
		}
		else // LR Rotation: 3 cases
		{
			// TODO: SLIDE 57
			// 4 LOC HERE BUT SOME MORE TO WRITE
			C = B->right;
			CL = C->left;
			CR = C->right;

			cout << "a's word " << A->word << endl;
			cout << "b's word " << B->word << endl;
			cout << "c's word " << C->word << endl;

			C->left = B;
			C->right = A;
			B->right = CL;
			A->left = CR;

			switch (C->balanceFactor)
			{
				// MORE TODO 
				// THERE ARE 3 SUBCASES HERE
			case 0:
				A->balanceFactor = B->balanceFactor = 0;
				break;
			case -1:
				B->balanceFactor = +1;
				A->balanceFactor = C->balanceFactor = 0;
				break;
			case 1:
				B->balanceFactor = C->balanceFactor = 0;
				A->balanceFactor = -1;
				break;
			}

			C->balanceFactor = 0;
			B = C;
		} // end of else (LR Rotation)
	} // end of if (d = +1)
	else // d = -1. This is a right imbalance
	{
		// (RR or RL)
		// SYMMETRIC TO LEFT BALANCE
		if (B->balanceFactor == -1) // RR Rotation
		{
			B->left = A;
			A->right = nullptr;
			A->balanceFactor = B->balanceFactor = 0;
		}
		else // RL Rotation: 3 cases
		{
			// TODO: SLIDE 58
			C = B->left;
			CL = C->left;
			CR = C->right;

			cout << "a's word " << A->word << " and bf " << A->balanceFactor << endl;
			cout << "b's word " << B->word << " and bf " << B->balanceFactor << endl;
			cout << "c's word " << C->word << " and bf " << C->balanceFactor << endl;

			C->right = B;
			C->left = A;
			B->left = CL;
			A->right = CR;

			switch (C->balanceFactor)
			{
				// MORE TODO 
				// THERE ARE 3 SUBCASES HERE
			case 0:
				A->balanceFactor = B->balanceFactor = 0;
				break;
			case -1:
				A->balanceFactor = +1;
				B->balanceFactor = C->balanceFactor = 0;
				break;
			case 1:
				A->balanceFactor = C->balanceFactor = 0;
				B->balanceFactor = -1;
				break;
			}

			C->balanceFactor = 0;
			B = C;
		} // end of else (RL Rotation)
	}

	// Finish up:
	if (F == nullptr)
	{
		root = B;
		return;
	}

	if (A == F->left)
	{
		F->left = B;
		return;
	}
	if (A == F->right)
	{
		F->right = B;
		return;
	}
	cout << "If we get here, we fucked up" << endl; // adjust this line before turning in
}

void AVL::list()
{
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
	cout << start->word << endl;

	print2DUtil(start->left, space);
}
