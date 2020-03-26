// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BST.h"
#include "Skiplist.h"
#include "RBT.h"
#include "AVL.h"
#include <fstream>

void run();

int main()
{
    run();
}


void run()
{
    char c;
    RBT* RBT_T = new RBT();
    AVL* AVL_T = new AVL();
    BST* BST_T = new BST();
    Skiplist* SL = new Skiplist();

    char chari[50];
    memset(chari, 0, 50);
    int iPtr;
    ifstream inFile;

    for (int pass = 0; pass < 6; pass++)
    {
        bool isDelimiter = false, wasDelimiter = false;
        
        inFile.open("C:\\Users\\Quinn\\Desktop\\Huffman Testing\\Shakespeare.txt", ios::binary);
        if (inFile.fail())
        {
            cout << "Unable to open input file \n\n Program Exiting \n\n Press ENTER to exit";
            cin.get(c);
            exit(1);
        }
        iPtr = 0;
        inFile.get(c);
        while (!inFile.eof())
        {
            isDelimiter = (c == ' ' || c == 10 || c == 13 || c == ',' || c == '.' || c == '\'' ||
                c == ':' || c == ';' || c == '"' || c == '?' || c == '!' || c == '-' ||
                c == '(' || c == ')' || c == '[' || c == ']' || c == '_' || c == '*' || c == 9);
            if (isDelimiter && !wasDelimiter)
            {
                wasDelimiter = true;
                if (pass == 2) RBT_T->insert(chari);
                else if (pass == 3) AVL_T->insert(chari);
                else if (pass == 4) BST_T->insert(chari);
                else if (pass == 5) SL->insert(chari);

                memset(chari, 0, 50);
                iPtr = 0;
            }
            else if (!isDelimiter) chari[iPtr++] = c;

            wasDelimiter = isDelimiter;
            inFile.get(c);
        }
        inFile.close();

        if (pass == 2)
        {
            if (strlen(chari)) RBT_T->insert(chari);
            RBT_T->displayStatistics();
        }
        if (pass == 3)
        {
            if (strlen(chari)) AVL_T->insert(chari);
            AVL_T->displayStatistics();
        }
        if (pass == 4)
        {
            if (strlen(chari)) BST_T->insert(chari);
            BST_T->displayStatistics();
        }
        if (pass == 5)
        {
            if (strlen(chari)) SL->insert(chari);
            SL->displayStatistics();
        }
    }
    delete RBT_T;
    delete AVL_T;
    delete BST_T;
    delete SL;
}