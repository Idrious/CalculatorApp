#ifndef _FUNCTION_DECLARATIONS_H_
#define _FUNCTION_DECLARATIONS_H_

#include "CalculatorGUI.h"
#include "raylib.h"

#include <iostream>
#include <bits/stdc++.h>
#include <vector>
#include <map>

#define _DEBUG_ false // debug mode for extra information pringing, set to TRUE if desired
static const int BUFFER_MAX_LENGTH = 128;  // predefined input buffer length

enum TOKEN_TYPE{OPERAND_TYPE,OPERATOR_TYPE}; // enum for specifying token type, a token can be a numerical operand (float) or a valid operator (char)
enum OPERATOR { PLUS,MINUS,DIVIDE,MULTIPLY, EQUALS }; // enum for clarity and operatorMap access
enum MODIFY_CONTROL{ DECIMAL,NEGPOS }; // enum for clarity and controlMap access

/*
----------------------------------------------------------------
-------------- General Functions Implementation ----------------
----------------------------------------------------------------
*/

// struct for valid input information
struct VALID_INPUT_INFO
{
    const std::vector<int>  VALID_NUM = {'0','1','2','3','4','5','6','7','8','9'};    // valid numerical inputs
    const std::vector<int>  VALID_OPERATOR = {'=','+','-','/','*'};   // valid operators that can be used for calculations
    const std::vector<int>  VALID_MODIFY = {'.','~'};                 // modify controls are special characters that modify the calculation, such as decimal input (x.xx) and swapping numerical state (-/+)
};
static VALID_INPUT_INFO validInputs; // struct member to hold input information

// general function, given an int and an array, check if a is in the supplied array, return false if not
static bool isIn(int a,std::vector<int> v)
{
    for(int i = 0;i < v.size(); i++)
    { if(a == v[i]) { return true; } }
    return false;
}

// if given a valid c is in VALID_NUM array, return true, otherwise false
static bool isValidNum(int c)
{
    if(c && isIn(c, validInputs.VALID_NUM)) 
    { return true; }
    else 
    { return false; }
}

// if givec a valid c is in VALID_MODIFY array, return true, otherwise false
static bool ifModifiesInput(int c)
{
    if(c && isIn(c, validInputs.VALID_MODIFY)) 
    { return true;}
    else 
    { return false;}
}

// if a given valid c is in the VALID_OPERATOR array, return true, otherwise false
static bool isValidOperator(int c)
{
    if(c && isIn(c, validInputs.VALID_OPERATOR)) 
    { return true; }
    else 
    { return false; }
}

// operator precedence function, (*,/) higher than (+,-) for calculations
static int precedence(char op)
{
    if(op == '*' || op == '/') { return 2; }
    if(op == '+' || op == '-') { return 1; }
    return 0;
}

// given two character operators, return the logical result from invoking precedence() function on each respective operator
static bool equalPrecedenceOrHigher(char op1, char op2)
{ return (precedence(op1)>=precedence(op2)); }

// Token class holds information about a parsed token, specifically what kind of token it is, a numerical operand, or a character operator
// contains data dependant on type, operands have parsed float values, while operators have a single character _op
class Token
{
    public:
        TOKEN_TYPE type; // the type of this token

        // dependant on token type, give this class a float value, or a character operator
        union tokenData {
            char _op;
            float value;
        } data;

        Token(TOKEN_TYPE _type, float _data) // constructor for a numerical operand token, supplied with a parsed float
        {
            type = _type;
            data.value = _data;
        }

        Token(TOKEN_TYPE _type, char _operator) // constructor for a character operator token, supplied with a single character operator
        {
            type = _type;
            data._op = _operator;
        }

        void PrintInfo() // basic printing information, prints spaces in between data values for clarity
        {
            if(type == OPERAND_TYPE)
            { std::cout<< " " << data.value << " "; }
            else
            { std::cout<< " " << data._op << " "; }
        }
    private:
};

// struct for holding expression information, like input buffer, input size, and a infix notation style vector of tokens
struct CalculatorInfo
{
    std::vector<Token*> tokens = {}; // vector of tokens populated by parsed information during execution, either operators, or operands
    char inputBuffer[BUFFER_MAX_LENGTH]; // input buffer gets populated by valid tokens
    size_t inputSize = 0; // input buffer size counter, incremented when valid inputs added to buffer
    char resultsBuffer[BUFFER_MAX_LENGTH]; // results buffer is populated when an expression is evaluated, used by the GUI for display purposes
};

// Calculator class is the main feature of the program. Contains methods for recieving input, processing it, parsing it into a token vector, and computing / evaluating expressions given valid token vectors
class Calculator
{
    public:
        Calculator();
        ~Calculator(){}
        CalculatorGUI* GUI; // front end GUI for drawing buttons, has wrapper function for processing input from GUI front end

        std::vector<Token*> GetTokens() // return the expressionInfo token vector
        { return expressionInfo.tokens; }

        char* GetInputBuffer() // return the expressionInfo input buffer
        { return expressionInfo.inputBuffer; }

        size_t GetInputBufferLength() // return the expressionInfo input buffer size counter
        { return expressionInfo.inputSize; }

        char* GetResultsBuffer() // return the expressionInfo results buffer
        { return expressionInfo.resultsBuffer; }

        void ProcessInput(int inputC); // higher level process input function, checks whether supplied input is to be added to the buffer or if it changes the control flow
        
        void ClearInputBuffer(); // Clear the input buffer and reset the size counter
        void ClearExpressionTokens(); // clear the tokens vector
        void ClearResultsBuffer(); // clear the reults buffer
       
        CalculatorInfo GetCalcInfo() // return the entire expressionInfo struct (for testing)
        { return expressionInfo; }

        float GetResult() // return the calculated result
        { return calculatorResult; }

        friend class CalculatorPrivateTEST; // friend class for unit testing postfix conversion and evaluation functions
    private:
        CalculatorInfo expressionInfo; // struct for holding buffer information such as inputbuffer, reslultsbuffer, and parsed tokens
        float calculatorResult; // calculated results

        void AddToInput(int c); // Add to the input buffer
        void AddToBufferIfValid(int c); // adds to the input buffer if a valid numerical input

        void DecimalInput(int c); // Checks to see if a decimal point exists in the input buffer. If it does, input is ignored, if no pre existing decimal point is found, it is added to the buffer.
        void NegativeInput(int c); // Checks to see if the input string has a prepended '-' (already negative), if so, removes it, if not, prepends one to the inputBuffer
        void ModifyInput(int c); // given a valid c, determine what is modifying input, be it a decimal or negative

        bool CheckForInputEndingControl(int inputC); // Check for what type of operator was supplied, if '=', evaluate the expression
        float ParseAndClear(); // get the number from the inputBuffer, clear the buffer, and return the parsed number
        void EvaluateExpression(); // Evaluate the contents of the generated tokens vector
   
        // Based off modified Shunting Yard algorithm by (Djikstra), infix -> postfix and evaluation functions
        std::vector<Token*> convertToPostfix(std::vector<Token*> infix); // Given a vector of tokens in infix notation, convert and return a token vector in postfix notation.
        float EvaluatePostFix(std::vector<Token*> postfix); // Given a vector of tokens, evaluate and reduce their operations to a final result and return it.
};

#endif