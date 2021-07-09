#include "LexicalFunctions.h"
#include "ParserFunctions.h"

std::stack<int> StackWithRules;
ofstream ParserErrorptr;
int LineNo=1;

struct Token
{
    string Lexeme;
    string datatype;
};
template <typename T>
int BinarySearchHelper(vector<T> Vs, int starting, int ending, T num)
{
    if (starting == ending || starting > ending)
        return -1;
    int middle = (starting + ending) / 2;
    if (Vs[middle] == num)
        return middle;
    if (middle == starting || middle == ending)
    {
        if (num == Vs[starting])
            return starting;
        if (num == Vs[ending])
            return ending;
        return -1;
    }
    if (num < Vs[middle])
        return BinarySearchHelper(Vs, starting, middle, num);
    return BinarySearchHelper(Vs, middle, ending, num);
}

template <typename T>
int BinarySearch(vector<T> Vs, T num)
{
    if (Vs[0] > num || Vs[Vs.size() - 1] < num)
        return -1;
    return BinarySearchHelper(Vs, 0, Vs.size(), num);
}

//-----------------------------------------Lexical Functions------------------------------------------------------
void MakeTokens(string token_, int state, Token &token_datatype)
{
    char *tokenType = Datatypes[acceptanceTable[state] - 1];
    ofstream fout("Token.txt", ios::out | ios::app);
    if (!fout.is_open())
    {
        cout << "Token File: Opening Error\n";
        return;
    }
    fout << "< " << tokenType << " , " << token_ << " >\n";
    token_datatype.Lexeme = token_, token_datatype.datatype = tokenType;
    fout.close();
}
int GetTransitionIndex(char character)
{
    return BinarySearch(Transition, character);
}

bool isDecrementalPointer(int state, int col)
{
    if (TransitionTable[state][col] == -1)
        return true;
    return false;
}
bool isFinalState(int state)
{
    if (acceptanceTable[state] >= 1)
        return true;
    return false;
}
bool isErrorState(int state)
{
    if (acceptanceTable[state] == -1)
        return true;
    return false;
}
bool isOtherOperator(char op)
{
    if (BinarySearch(Transition, op) == -1)
        return true;
    return false;
}
bool isCharacter(char ch)
{
    if (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z')
        return true;
    return false;
}
bool isNumber(char ch)
{
    if ((((int)ch) - 48) >= 0 && (((int)ch) - 48) <= 9)
        return true;
    return false;
}
bool isIncreamentalOperator(int state, int Trans)
{
    if (TransitionTable[state][Trans] == -2)
        return true;
    return false;
}
bool isKeyword(string key, int &index)
{
    char *keytemp = HashTable[hashFunction(key.c_str())];
    if (keytemp != nullptr)
    {
        if (strcmp(keytemp, key.c_str()) == 0)
            return true;
    }
    return false;
}
void TokenToBeMake(int &Curr_State, int col, string &word, int &index, Token &token_)
{
    if (isDecrementalPointer(Curr_State - 1, col))
    {
        char temp = word[word.size() - 1];
        word.pop_back();
        MakeTokens(word, Curr_State - 1, token_);
        if (temp != '\n' && temp != '\t' && temp != ' ')
            index--;
    }
    else
        MakeTokens(word, Curr_State - 1, token_);
    word.clear();
    Curr_State = 1;
}
void ErrorToBeMake(ofstream &Errorfile, int &Curr_State, int col, string &word, int &index)
{
    if (word != "999" && isDecrementalPointer(Curr_State - 1, col))
    {
        char temp = word[word.size() - 1];
        word.pop_back();
        Errorfile << "Error in Line no: " << LineNo << " (" << word << ")\n";
        if (temp != '\n' && temp != '\t' && temp != ' ')
            index--;
    }
    else
        Errorfile << "Error in Line no: " << LineNo << " (" << word << ")\n";
    Curr_State = 1;
}
bool FirstStateError(ofstream &Errorfile, int &Curr_State, int &index, string &word, char Next_State)
{
    if (Curr_State == -51)
    {
        if (Next_State == ' ' && Next_State == '\n' && Next_State == '\t')
            index++;
        else
            Errorfile << "Error in Line no: " << LineNo << " (" << word << ")\n";
        word.clear();
        Curr_State = 1;
        return true;
    }
    return false;
}
bool isIgnoreState(int state, char Trans, int &index)
{
    if (state == 1)
    {
        if (Trans == '\n')
            LineNo++;
        if (Trans == '\n' || Trans == '\t' || Trans == ' ' || Trans == '\r')
            return true;
    }
    return false;
}
char SetTransition(char Curr_Trans, string word)
{
    if (isCharacter(Curr_Trans))
        return 'L';
    else if (isNumber(Curr_Trans))
        return 'D';
    else if (isOtherOperator(Curr_Trans))
        return 'o';
    return Curr_Trans;
}
void PrintHashTable()
{
    for (int i = 0; i < TotalKeywords; i++)
        cout << hashFunction(Keywords[i]) << " ==> " << HashTable[hashFunction(Keywords[i])] << endl;
}

//--------------------------------------Parser Functions---------------------------------------------------------

void PrintStack(stack<int> StackWithRules, Token token_)
{
    vector<int> Vs;
    while (!StackWithRules.empty())
    {
        Vs.push_back(StackWithRules.top());
        StackWithRules.pop();
    }
    for (int i = Vs.size() - 1; i > -1; i--)
        cout << Vs[i] << " ";
    cout << "\t===> " << token_.Lexeme << "\t" << token_.datatype << endl;
}
int GetEndOfRuleIndex(int rule)
{
    int i = 0;
    while (SubGrammer[rule][i] != EndOfRule) //Moving pointer at the end of rule
        i++;
    return i - 1;
}
bool isSet(string datatype)
{
    if (datatype == "Number" || datatype == "Identifiers" || datatype == "EOF")
        return true;
    return false;
}
ofstream OpenParserErrorFile()
{
    ofstream out("ParserError.txt", ios::out | ios::app);
    if (!out.is_open())
        cout << "Error File: Opening Error\n";
    return out;
}
int SearchTokenInTerminal(Token token_)
{
    int num = BinarySearch(Terminals, token_.Lexeme);
    if (num >= 0)
        return num;
    return BinarySearch(Terminals, token_.datatype);
}

void isTerminalError(int Curr_State, int rule, Token &token_, int index)
{
    ErrorToBeMake(ParserErrorptr, Curr_State, rule, token_.Lexeme, index);
    StackWithRules.pop();
    if (StackWithRules.top() == EndOfRule)
        StackWithRules.push(-1);
    token_.datatype.clear(), token_.Lexeme.clear();
}
void PanicMode(int Curr_State, int rule, Token &token_, int index, int terminal)
{
    int newrule = ParserTable[rule - 1][terminal];
    if (newrule == POP)
    {
        ErrorToBeMake(ParserErrorptr, Curr_State, newrule, token_.Lexeme, index);
        StackWithRules.pop();
    }
    else if (newrule == SKIP)
    {
        ErrorToBeMake(ParserErrorptr, Curr_State, newrule, token_.Lexeme, index);
        token_.datatype.clear(), token_.Lexeme.clear();
    }
    else if (newrule > 0) //Any Non terminal
    {
        StackWithRules.pop();
        for (int i = GetEndOfRuleIndex(newrule); i != -1; i--) //push rule from right to left
            StackWithRules.push(SubGrammer[newrule][i]);
    }
}
bool isValidDatatype(Token token_,int rule,int Dnum)
{
    if ((isSet(token_.datatype) && (Dnum != -1)) && (Terminals[Dnum] == Terminals[rule - 1]))
        return true;
    return false;
}
bool isValidLexeme(Token token_,int rule,int num)
{
    if (((num != -1) && (Terminals[num] == Terminals[rule - 1])))
        return true;
    return false;
}

void TerminalPoint(Token &token_, int Curr_State, int rule, int index)
{
    int num = BinarySearch(Terminals, token_.Lexeme);
    int Dnum = BinarySearch(Terminals, token_.datatype);
    if (isValidDatatype(token_,rule,Dnum) || isValidLexeme(token_,rule,num))
    {
        token_.datatype.clear(), token_.Lexeme.clear();
        StackWithRules.pop();
    }
    else
    {
        if(num>-1)
            PanicMode(Curr_State,rule,token_,index,num);
        else
            PanicMode(Curr_State,rule,token_,index,Dnum);
            /*This type of error occur when both(Top of Stack and Token.Lexeme)
            does not match*/
    }
}

//------------------------------------------Compiler------------------------------------------------------------

Token GetToken(int &index, int &Curr_State, int &IncreaseLine, char *Code)
{
    int Filesize = strlen(Code);
    string word;
    Token token_;
    ofstream LexicalErrorptr("LexicalError.txt", ios::out | ios::app);
    if (!LexicalErrorptr.is_open())
    {
        cout << "Error File: Opening Error\n";
        return token_;
    }
    bool TokenCreated = false;
    while (!TokenCreated)
    {
        char Curr_Trans;
        if (index == Filesize)
        {
            token_.Lexeme = word, token_.datatype = "EOF",index++;
            return token_;
        }
        if (index < Filesize)
            Curr_Trans = Code[index++];
        else
            Curr_Trans = ' ', index++;
        if (!isIgnoreState(Curr_State, Curr_Trans, index))
        {
            word.push_back(Curr_Trans);
            if (Curr_Trans == '\n')
                IncreaseLine = 1;
            if (!isCharacter(Code[index]) && isKeyword(word, index))
                Curr_State = 1, Curr_Trans = 'K';
            else
                Curr_Trans = SetTransition(Curr_Trans, word);
            int col = GetTransitionIndex(Curr_Trans);
            Curr_State = TransitionTable[Curr_State - 1][col];
            if (!FirstStateError(LexicalErrorptr, Curr_State, index, word, Code[index]))
            {
                if (isFinalState(Curr_State - 1))
                {
                    TokenToBeMake(Curr_State, col, word, index, token_);
                    TokenCreated = true;
                }
                if (isErrorState(Curr_State - 1))
                {
                    ErrorToBeMake(LexicalErrorptr, Curr_State, col, word, index);
                    word.clear();
                }
                if (IncreaseLine)
                    LineNo++, IncreaseLine = 0;
            }
        }
    }
    return token_;
}

void Compiler()
{
    // declareting and Intializing Parser Variables
    int Filesize = 0, index = 0, Curr_State = 1, IncreaseLine = 0, rule;
    char *Code = ReadFile(Filesize);
    Token token_;
    ParserErrorptr = OpenParserErrorFile();

    // Creating Stack
    StackWithRules.push(EndOfRule);
    StackWithRules.push(-1);

    //Parser Working
    while (!StackWithRules.empty()) //Lexical Send token to parser
    {
        rule = StackWithRules.top();
        if (token_.Lexeme.empty() && token_.datatype.empty())
            token_ = GetToken(index, Curr_State, IncreaseLine, Code); //Get token from lexical
        PrintStack(StackWithRules, token_);
        if (token_.datatype == "EOF" && StackWithRules.top() == 999)
            break;
        if (isTerminal(rule)) 
        {
            int terminal = SearchTokenInTerminal(token_);
            if (terminal < 0)   
                isTerminalError(Curr_State, rule, token_, index);       
            else               
                PanicMode(Curr_State, rule*(-1), token_, index, terminal);
            /*In this check we check whether the terminal is a 
            valid terminal or not for this we search in terminals.txt 
                1. if it does not exists then if statement execute
                    and make token error. 
                2. if it exists then else statement executes
                    and Parser table decide what we should do. */
        }
        else if (rule == isNULL) // if rule approaches to null
            StackWithRules.pop();
        else                     
            TerminalPoint(token_, Curr_State, rule, index);
            // state where we compare top of stack and the token 
    }
    ParserErrorptr.close();
}

int main()
{
    LoadLexialData();
    LoadParserData();
    Compiler();
    return 0;
}
