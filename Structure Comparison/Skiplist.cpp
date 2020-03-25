#include "Skiplist.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

Skiplist::Skiplist()
{
	head = createNegInfNode();
	tail = createPosInfNode();

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

Skiplist::node* Skiplist::createNegInfNode()
{
	node* newNode = new node;
	newNode->isSentinel = true;
	return newNode;
}

Skiplist::node* Skiplist::createPosInfNode()
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
			if (compare < 0)
			{
				p = p->right;
			}
			else if (compare == 0)
			{
				p = p->right;
				while (p->down != nullptr) p = p->down;
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

	p->right->left = newNode;
	p->right = newNode;

	numItems++;

	int currentHeight = 1;

	while (rand() & 1)
	{
		currentHeight++;

		if (currentHeight > height)
		{
			node* negInf = createNegInfNode();
			node* posInf = createPosInfNode();

			negInf->down = head;
			head->up = negInf;

			posInf->down = tail;
			tail->up = posInf;

			negInf->right = posInf;
			posInf->left = negInf;

			head = negInf;
			tail = posInf;

			height++;
		}

		node* stackNode = new node; // Node that gets stacked on top of the current node
		strcpy(stackNode->word, word);
		stackNode->down = newNode;
		newNode->up = stackNode;

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

		newNode = stackNode;
	}
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