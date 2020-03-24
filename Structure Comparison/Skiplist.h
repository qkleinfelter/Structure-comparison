#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <string.h>
using namespace std;
class Skiplist
{
public:
	Skiplist();
	~Skiplist();
	void insert(const char word[50]);
	void list();
	void stackedList();
private:
	struct node
	{
		char word[50];
		int count = 1;
		node* left = nullptr;
		node* right = nullptr;
		node* up = nullptr;
		node* down = nullptr;

		const static char NEG_INF[50];
		const static char POS_INF[50];
	};

	node* head = nullptr;
	node* tail = nullptr;
	int height;
	int numItems;
	node* find(const char word[50]);

	void deleteNode(node* n);

	node* createNegInfNode();
	node* createPosInfNode();
};