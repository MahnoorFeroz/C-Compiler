#include <iostream> 
#include <string.h>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<string> Vs{ "!=" , "(" , "," , ")" , "*" , "/" , "+" , "-" , ":=" , ";" , "<" , ">" , "=" , "==" , "[" ,"]" , "float" , "int" , "Identifiers" , "Number" , "while" ,"EOF"};
    std::sort(begin(Vs),end(Vs));
    for(int i=0;i<Vs.size();i++)
        cout<< Vs[i]<< endl;
    return 0;
}