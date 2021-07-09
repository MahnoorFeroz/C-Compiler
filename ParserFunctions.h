#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
using namespace std;

#define TotalRules 46
#define TotalTerminals 22
#define TotalNonTerminals 24
#define isNULL 111
#define POP 444
#define SKIP 333
#define EndOfRule 999


int **SubGrammer;
int **ParserTable;
vector<string> Terminals;
vector<string> NonTerminals;

void LoadSubGrammer()
{
    ifstream read("Subgrammer.txt");
    if (!read.is_open())
    {
        cout << "SubGrammer File: Opening Error\n";
        return;
    }
    //Initializing SubGrammer
    SubGrammer = new int *[TotalRules];

    //Start Reading File
    for (int ruleNo = 1; ruleNo < TotalRules; ruleNo++)
    {
        int Transition = 0;
        SubGrammer[ruleNo] = new int[10];
        while (SubGrammer[ruleNo][Transition - 1] != EndOfRule)
            read >> SubGrammer[ruleNo][Transition++];
    }
}

void PrintSubGrammer()
{
    for (int ruleNo = 0; ruleNo < TotalRules; ruleNo++)
    {
        int Transition = 0;
        while (SubGrammer[ruleNo][Transition] != EndOfRule)
            cout << SubGrammer[ruleNo][Transition++] << " ";
        cout << endl;
    }
}

void LoadParserTable()
{
    ifstream read("ParserTable.txt");
    if (!read.is_open())
    {
        cout << "ParserTable File: Opening Error\n";
        return;
    }
    //Initializing Parser Table
    ParserTable = new int *[TotalNonTerminals];

    //Start Reading File
    for (int row = 0; row < TotalNonTerminals; row++)
    {
        ParserTable[row] = new int[TotalTerminals];
        for (int col = 0; col < TotalTerminals; col++)
            read >> ParserTable[row][col];
    }
}
void PrintParserTable()
{
    for (int row = 0; row <= TotalNonTerminals; row++)
    {
        cout << row<< ".\t";
        for (int col = 0; col <= TotalTerminals; col++)
            cout << ParserTable[row][col] << " ";
        cout << endl;
    }
}
void LoadTerminals()
{
    ifstream read("Terminals.txt");
    if (!read.is_open())
    {
        cout << "Terminals File: Opening Error\n";
        return;
    }
    string temp;
    for (int ter = 1; ter <= TotalTerminals; ter++)
    {
        read >> temp;
        Terminals.push_back(temp);
        temp.clear();
    }
}
bool isTerminal(int index)
{
    if(index<0)
        return true;
    return false;
}


void PrintTerminal()
{
    for(int i=1;i<=TotalTerminals;i++)
    {
        cout<< Terminals[i]<<endl;
    }
}
void LoadNonTerminals()
{
    ifstream read("NonTerminals.txt");
    if (!read.is_open())
    {
        cout << "NonTerminals File: Opening Error\n";
        return;
    }

    string temp;
    for (int ter = 1; ter <= TotalNonTerminals; ter++)
    {
        read >> temp;
        NonTerminals.push_back(temp);
        temp.clear();
    }
}
void PrintNonTerminal()
{
    for (int ter = 1; ter <= TotalNonTerminals; ter++)
        cout << NonTerminals[ter] << endl;
}

void LoadParserData()
{
    LoadSubGrammer();
    LoadTerminals();
    LoadNonTerminals();
    LoadParserTable();
}