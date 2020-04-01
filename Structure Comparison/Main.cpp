/*
    Author: Quinn Kleinfelter
	EECS 2510-001 Non Linear Data Structures Spring 2020
	Dr. Thomas
	Last Edited: 4/1/2020
	Main File used to run tests on our 4 data structures,
    AVL, BST, RBT, and Skiplist.
*/

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
    // Method to run everything so that our main method
    // isn't clogged up by all of this stuff
    // Parses through the file 6 times, and prints relevant statistics

    char c;
    RBT* RBT_T = new RBT();         // instantiate each of the trees
    AVL* AVL_T = new AVL();         //
    BST* BST_T = new BST();         //
    Skiplist* SL = new Skiplist();  // and the skip list

    char chari[50]; // assumes no word is longer than 49 characters
    memset(chari, 0, 50); // zero the word buffer
    int iPtr;
    ifstream inFile;

    clock_t dryRunElapsed;

    for (int pass = 0; pass < 6; pass++)
    {
        clock_t startTime = clock();

        bool isDelimiter = false, wasDelimiter = false;
        
        inFile.open("C:\\Users\\Quinn\\Desktop\\Structure Comparison Testing\\ALL.txt", ios::binary);
        if (inFile.fail())
        {
            cout << "Unable to open input file \n\n Program Exiting \n\n Press ENTER to exit";
            cin.get(c);
            exit(1);
        }
        iPtr = 0;
        inFile.get(c); // priming read
        while (!inFile.eof())
        {
            isDelimiter = (c == ' ' || c == 10 || c == 13 || c == ',' || c == '.' || c == '\'' ||
                c == ':' || c == ';' || c == '"' || c == '?' || c == '!' || c == '-' ||
                c == '(' || c == ')' || c == '[' || c == ']' || c == '_' || c == '*' || c == 9);
            if (isDelimiter && !wasDelimiter) // if THIS character IS a delimiter and the last
                                              // one WASN't, it's the end of a word
            {
                wasDelimiter = true;
                if (pass == 2) RBT_T->insert(chari);        // insert this word in the RBT
                else if (pass == 3) AVL_T->insert(chari);   // insert it in the AVL Tree
                else if (pass == 4) BST_T->insert(chari);   // insert it in the BST
                else if (pass == 5) SL->insert(chari);      // insert it in the Skiplist

                memset(chari, 0, 50); // zero the word buffer
                iPtr = 0;
            }
            else if (!isDelimiter) chari[iPtr++] = c; // if this isn't a delimiter, keep going

            wasDelimiter = isDelimiter; // for the NEXT iteration
            inFile.get(c);
        }
        inFile.close();
        // If the file doesn't end with a delimiter, it will leave the last word unprocessed.
        // Insert once more, and display the statistics for this structure...
        //
        clock_t endTime = clock();
        clock_t elapsedTime = endTime - startTime;

        if (pass == 2) // RBT
        {
            if (strlen(chari)) RBT_T->insert(chari);
            RBT_T->displayStatistics();
        }
        if (pass == 3) // AVL
        {
            if (strlen(chari)) AVL_T->insert(chari);
            AVL_T->displayStatistics();
        }
        if (pass == 4) // BST
        {
            if (strlen(chari)) BST_T->insert(chari);
            BST_T->displayStatistics();
        }
        if (pass == 5) // Skiplist
        {
            if (strlen(chari)) SL->insert(chari);
            SL->displayStatistics();
        }

        if (pass == 1)
        {
            dryRunElapsed = elapsedTime;
        }
        else if (pass > 1)
        {
            cout << "Elapsed Time: " << (elapsedTime - dryRunElapsed) / 1000.0 << " seconds" << endl;
            cout << "---------------------------" << endl;
        }
    }
    // Delete all of our trees now that we are done
    delete RBT_T; 
    delete AVL_T;
    delete BST_T;
    delete SL;
}