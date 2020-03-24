#include "Skiplist.h"
#include <stdlib.h>
#include <time.h>

const char Skiplist::node::NEG_INF[50] = { '.' };
const char Skiplist::node::POS_INF[50] = { ',' };

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

}

Skiplist::node* Skiplist::createNegInfNode()
{
	node* newNode = new node;
	strcpy(newNode->word, node::NEG_INF);
	return newNode;
}

Skiplist::node* Skiplist::createPosInfNode()
{
	node* newNode = new node;
	strcpy(newNode->word, node::POS_INF);
	return newNode;
}

Skiplist::node* Skiplist::find(const char word[50])
{
	node* p = head;
	while (true)
	{
		while (strcmp(p->right->word, node::POS_INF) != 0 && strcmp(p->right->word, word) <= 0)
		{
			p = p->right;
		}

		if (p->down == nullptr) return p;

		p = p->down;
	}
}

void Skiplist::insert(const char word[50])
{
	node* p = find(word);

	if (strcmp(p->word, word) == 0)
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

	while (rand() & 1) // seed this somehow eventually
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
	}
}
