#include "Skiplist.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

Skiplist::Skiplist()
{
	startTime = clock();
	head = createSentinelNode();
	tail = createSentinelNode();

	head->right = tail;
	tail->left = head;

	height = 1;
	numItems = 0;

	srand(time(NULL));
}

Skiplist::~Skiplist()
{
	node* p = head;
	while (p != nullptr)
	{
		node* q = p->right;
		while (q != nullptr)
		{
			node* del = q;
			q = q->right;
			delete del;
		}
		node* del = p;
		p = p->down;
		delete del;
	}
}

Skiplist::node* Skiplist::createSentinelNode()
{
	node* newNode = new node;
	newNode->isSentinel = true;
	return newNode;
}

Skiplist::node* Skiplist::find(const char word[50], bool& found)
{
	node* p = head;
	while (true)
	{
		while (!p->right->isSentinel)
		{
			int compare = strcmp(p->right->word, word);
			keyComparisons++;
			if (compare < 0)
			{
				p = p->right;
			}
			else if (compare == 0)
			{
				p = p->right;
				while (p->down != nullptr)
				{
					p = p->down;
				}
				found = true;
				return p;
			}
			else
			{
				break;
			}
		}

		if (p->down == nullptr)
		{
			found = false;
			return p;
		}
		else
		{
			p = p->down;
		}
	}
}

void Skiplist::insert(const char word[50])
{
	bool found = false;
	node* p = find(word, found);

	if (found)
	{
		p->count++;
		return;
	}
	node* newNode = new node;
	// defaults
	strcpy(newNode->word, word);

	newNode->left = p;
	newNode->right = p->right;
	ptrChanges += 2;
	newNode->isSentinel = false;

	p->right->left = newNode;
	p->right = newNode;
	ptrChanges += 2;

	numItems++;

	int currentHeight = 1;

	while (rand() & 1)
	{
		currentHeight++;
		coinTosses++;

		if (currentHeight > height)
		{
			node* negInf = createSentinelNode();
			node* posInf = createSentinelNode();

			negInf->down = head;
			head->up = negInf;
			ptrChanges += 2;

			posInf->down = tail;
			tail->up = posInf;
			ptrChanges += 2;

			negInf->right = posInf;
			posInf->left = negInf;
			ptrChanges += 2;

			head = negInf;
			tail = posInf;

			height++;
		}

		node* stackNode = new node; // Node that gets stacked on top of the current node
		strcpy(stackNode->word, word);
		stackNode->down = newNode;
		newNode->up = stackNode;
		ptrChanges += 2;

		node* leftNode = p; 
		while (leftNode->up == nullptr)
		{
			leftNode = leftNode->left;
		}

		node* rightNode = p;
		while (rightNode->up == nullptr)
		{
			rightNode = rightNode->right;
		}

		stackNode->left = leftNode->up;
		stackNode->right = rightNode->up;
		ptrChanges += 2;

		newNode = stackNode;
	}
	coinTosses++; // We need to increment coin tosses one more time outside the loop, because it won't get incremented inside if its not an odd number
}

void Skiplist::list()
{
	node* start = head;
	while (start->down != nullptr)
	{
		start = start->down;
	}
	
	node* end = tail;
	while (end->down != nullptr)
	{
		end = end->down;
	}

	start = start->right;
	int index = 0;
	cout << "Set contains: ";

	while (start != end)
	{
		cout << "(" << ++index << ") " << start->word << " " << start->count;

		if (index != numItems)
		{
			cout << ", ";
		}
		
		start = start->right;
 	}
}

void Skiplist::stackedList()
{
	cout << "num items: " << numItems << ", height: " << height << endl;

	node* p = head;
	while (p->down != nullptr)
	{
		p = p->down;
	}
	p = p->right;

	while (!p->isSentinel)
	{
		node* q = p;
		do
		{
			cout << q->word << " ";
			q = q->up;
		} while (q != nullptr);
		cout << endl;
		p = p->right;
	}
}

void Skiplist::displayStatistics()
{
	cout << "---------------------------" << endl;
	cout << "SKIPLIST STATISTICS" << endl;
	clock_t endTime = clock();
	cout << "Pointer Changes: " << ptrChanges << endl;
	cout << "Key Comparisons: " << keyComparisons << endl;
	cout << "Coin Tosses: " << coinTosses << endl;
	double secondsElapsed = difftime(endTime, startTime) / 1000;
	cout << "Elapsed Time: " << secondsElapsed << " seconds." << endl;
	cout << "END SKIPLIST STATISTICS" << endl;
	cout << "---------------------------" << endl;
}