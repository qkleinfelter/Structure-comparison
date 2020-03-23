#include "Skiplist.h"
#include <stdlib.h>
#include <time.h>

Skiplist::Skiplist()
{
	head = new node;
	tail = new node;
	strcpy(head->word, node::NEG_INF);
	strcpy(tail->word, node::POS_INF);

	head->right = tail;
	tail->left = head;

	height = 1;
	numItems = 0;

	srand(time(NULL));
}

Skiplist::~Skiplist()
{

}

Skiplist::node* Skiplist::find(const char word[50])
{
	node* p = head;
	while (true)
	{
		while (strcmp(p->right->word, node::POS_INF) != 0 && strcmp(p->right->word, word) <= 0)
			p = p->right;
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
	}
	else
	{
		node* newNode = new node;
		// defaults
		strcpy(newNode->word, word);
		p->right->left = newNode;
		p->right = newNode;

		int currentHeight = 1;

		while (rand() & 1) // seed this somehow eventually
		{
			currentHeight++;
			if (currentHeight > height)
			{
				
			}
		}
	}
}