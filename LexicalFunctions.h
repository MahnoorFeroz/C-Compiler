#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <vector>
#include <string.h>
#include <algorithm>
using namespace std;

#define HASHSIZE 1000
#define STATES 52
#define TOKENS 25

int TotalKeywords;
int **TransitionTable = nullptr;
char **Keywords, **HashTable, **Datatypes;
vector<char> Transition;
int acceptanceTable[STATES];

int hashFunction(const char *character)
{
    int sum = 0;
    for (int i = 0; i < strlen(character); i++)
        sum = sum + (character[i] * (i + 2));
    return sum % HASHSIZE;
}
void LoadTransitions()
{
    ifstream read("Transitions.txt");
    if (!read.is_open())
    {
        cout << "Transitions File: Opening Error\n";
        return;
    }
    char ch;
    while (!read.eof())
    {
        read >> ch;
        Transition.push_back(ch);
    }
    Transition.pop_back();
    std::sort(Transition.begin(), Transition.end());
}

void LoadTransitionTable()
{
    ifstream read("TransitionTable.txt");
    if (!read.is_open())
    {
        cout << "Transition File: Opening Error\n";
        return;
    }
    //Initializing Transition Table
    TransitionTable = new int *[STATES];

    //Start Reading File
    for (int i = 0; i < STATES; i++)
    {
        TransitionTable[i] = new int[TOKENS];
        for (int j = 0; j < TOKENS; j++)
            read >> TransitionTable[i][j];
    }
}
void DefineMapping()
{
    HashTable = new char *[HASHSIZE];
    for(int i=0;i<HASHSIZE;i++)
        HashTable[i]=nullptr;
    for (int i = 0; i < TotalKeywords; i++)
    {
        HashTable[hashFunction(Keywords[i])] = new char[strlen(Keywords[i])];
        strcpy(HashTable[hashFunction(Keywords[i])], Keywords[i]);
    }
}
void LoadKeywords()
{
    ifstream read("Keyword.txt");
    if (!read.is_open())
    {
        cout << " Keywords File: Opening Error\n";
        return;
    }
    read >> TotalKeywords;
    Keywords = new char *[TotalKeywords];
    char extra[20];
    for (int i = 0; i < TotalKeywords; i++)
    {
        read >> extra;
        extra[strlen(extra) + 1] = '\0';
        Keywords[i] = new char[strlen(extra) + 1];
        strcpy(Keywords[i], extra);
    }
    read.close();
}
void LoadDataTypes()
{
    ifstream read("Datatypes.txt");
    if (!read.is_open())
    {
        cout << "Datatypes File: Opening Error\n";
        return;
    }
    Datatypes = new char *[5];
    char extra[20];
    for (int i = 0; i < 5; i++)
    {
        read >> extra;
        extra[strlen(extra) + 1] = '\0';
        Datatypes[i] = new char[strlen(extra) + 1];
        strcpy(Datatypes[i], extra);
    }
    read.close();
}
void LoadAcceptanceTable()
{
    ifstream read("AcceptanceTable.txt");
    if (!read.is_open())
    {
        cout << "AcceptanceTable File: Opening Error\n";
        return;
    }
    for (int i = 0; i < STATES; i++)
        read >> acceptanceTable[i];
}
char *ReadFile(int &size)
{
    ifstream file("SourceCode.txt", ios::in | ios::binary | ios::ate);
    if (!file.is_open())
    {
        cout << "Token File: Opening Error\n";
        return nullptr;
    }
    size = file.tellg();
    char *Input = new char[size];
    file.seekg(0, ios::beg);
    file.read(Input, size);
    file.close();
    return Input;
}
void LoadLexialData()
{
    LoadTransitions();
    LoadTransitionTable();
    LoadAcceptanceTable();
    LoadKeywords();
    DefineMapping();
    LoadDataTypes();
}
