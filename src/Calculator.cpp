#include "main.h"

/*
----------------------------------------------------------------
----------------- Calculator Implementation --------------------
----------------------------------------------------------------
*/

Calculator::Calculator()
{
    // GUI = new CalculatorGUI(this); // removed for Unit Testing, CalculatorGUI dependencies too much of a headache due to rayGUI imeplementation, GUI instance is created in main directly after this constructor.
    ClearInputBuffer();
    ClearExpressionTokens();
    ClearResultsBuffer();
}

// adds to the input buffer if a valid numerical input
void Calculator::AddToBufferIfValid(int c)
{
    if(isValidNum(c)) // recieved a valid numerical input, add to the input buffer
    { AddToInput(c); }
    else if(ifModifiesInput(c)) // special valid token that modifies control
    { ModifyInput(c); }
    else if(c) // still recieved an input, but it wasnt a valid number
    { /*std::cout<<"input was not a valid number. Did not add to buffer.\n";*/ }
}

// higher level process input function, checks whether supplied input is to be added to the buffer or if it changes the control flow
void Calculator::ProcessInput(int inputC)
{
    // if(inputC) { std::cout<<"input: " << inputC << "(char)"<<(char)inputC <<std::endl; }; // for testing inputs
    AddToBufferIfValid(inputC); // user entered a valid number or a special character that modfies input (such as decimal or negative)
    CheckForInputEndingControl(inputC); // user entered an operator, could be an '=' which means we need to evaluate expression
}

// Add to the input buffer
void Calculator::AddToInput(int c)
{
    if(expressionInfo.inputSize+1>sizeof(expressionInfo.inputBuffer)) // buffer full detection
    { std::cout<<"Input Buffer exceeded. Won't add.\n"; }
    else // add to the buffer, increment size
    { expressionInfo.inputBuffer[expressionInfo.inputSize++] = c; }
}

// Checks to see if a decimal point exists in the input buffer. If it does, input is ignored, if no pre existing decimal point is found, it is added to the buffer.
void Calculator::DecimalInput(int c)
{
    if(_DEBUG_) { std::cout << "decimal detected.\n"; }
    char* decimal = (char*) memchr (expressionInfo.inputBuffer, '.', strlen(expressionInfo.inputBuffer));  // check if decimal point already exists in number
    if(decimal!=NULL)  // if so, not valid, found a bug HERE. 
    { if(_DEBUG_) { std::cout<<"Attempted to place more than one decimal point into number. Ignored.\n"; } }
    else // if not, insert into buffer
    { AddToInput(c); }
}

// Checks to see if the input string has a prepended '-' (already negative), if so, removes it, if not, prepends one to the inputBuffer
void Calculator::NegativeInput(int c)
{
    if(_DEBUG_) { std::cout << "negative detected.\n"; }
    
    char replaceBuffer[BUFFER_MAX_LENGTH+8];
    if(expressionInfo.inputBuffer[0] == '-') // is a negative already, remove from buffer
    { memcpy(replaceBuffer, expressionInfo.inputBuffer+1, sizeof(expressionInfo.inputBuffer)-1); }
    else // is not a negative, prepend a negative symbol
    {
        replaceBuffer[0] = '-'; // set first character of temp array to negative symbol
        memcpy(replaceBuffer+1, expressionInfo.inputBuffer, sizeof(expressionInfo.inputBuffer)); // copy the remains of the inputBuffer
    }

    memset(expressionInfo.inputBuffer, sizeof(expressionInfo.inputBuffer), 0); // clear input buffer
    // or ClearInputBuffer();
    memcpy(expressionInfo.inputBuffer, replaceBuffer, sizeof(replaceBuffer)); // copy temp buffer into inputBuffer
    expressionInfo.inputSize = strlen(replaceBuffer); // redetermine size of inputBuffer
    memset(replaceBuffer, sizeof(replaceBuffer), 0); // clear temp buffer
}

// given a valid c, determine what is modifying input, be it a decimal or negative
void Calculator::ModifyInput(int c)
{
    switch (c)
    {
        case '.':
            DecimalInput(c);
            break;
        case '~':
            NegativeInput(c);
            break;
        default:
            std::cout<<"Uh oh. Detected unknown control that is supposed to modify the input.\n";
            break;
    }
}

// Check for what type of operator was supplied, if '=', evaluate the expression
bool Calculator::CheckForInputEndingControl(int inputC)
{
    if(isValidOperator(inputC)) // operators end the current input buffer, meaning it's time to add some tokens with parsed numerical values and what operator was supplied
    {
        if(expressionInfo.inputSize == 0) // non equals operator was pressed but no input was given (Ignore.)
        { return false; }

        float number = ParseAndClear(); // get the numerical value from the input buffer (including decimal / negative shenanigans) and clear the input buffer
        expressionInfo.tokens.push_back(new Token(OPERAND_TYPE, number));

        if(inputC == '=') // Evaluate expression
        { 
            EvaluateExpression(); 
            return true; 
        }
        expressionInfo.tokens.push_back(new Token(OPERATOR_TYPE, (char)inputC));
        return true;
    }   
    return false;
}

// Clear the input buffer and reset the size counter
void Calculator::ClearInputBuffer()
{
    std::memset(expressionInfo.inputBuffer, 0, BUFFER_MAX_LENGTH); // clear memory
    expressionInfo.inputSize=0; // reset input size
    if(_DEBUG_) { std::cout<<"Input Buffer cleared.\n"; }
}

// clear the tokens vector
void Calculator::ClearExpressionTokens()
{
    if(_DEBUG_) { std::cout <<"\nclearing tokens:\n"; }
    expressionInfo.tokens.clear();
}

// clear the reults buffer
void Calculator::ClearResultsBuffer()
{
    if(_DEBUG_) { std::cout <<"\nclearing results buffer:\n"; }
    std::memset(expressionInfo.resultsBuffer, 0, BUFFER_MAX_LENGTH); // clear memory
}

// get the number from the inputBuffer, clear the buffer, and return the parsed number
float Calculator::ParseAndClear()
{
    float number = atof(expressionInfo.inputBuffer);
    if(_DEBUG_) { std::cout << "parsed: " << number << std::endl; }
    ClearInputBuffer();
    return number;
}

// Evaluate the contents of the generated tokens vector
void Calculator::EvaluateExpression()
{
    if(_DEBUG_)
    {
        std::cout <<" To evaluate expression composed of infix tokens:\n";
        std::cout<< "# Tokens: " << expressionInfo.tokens.size() << std::endl;
        for(Token* currentToken : expressionInfo.tokens) 
        { currentToken->PrintInfo(); }
    }
    
    std::vector<Token*> postfix = convertToPostfix(expressionInfo.tokens); // create token vector in post fix notation for calculation
    calculatorResult = EvaluatePostFix(postfix); // perform calculation on post fix vector, store result
    snprintf(expressionInfo.resultsBuffer, sizeof(expressionInfo.resultsBuffer), "result: %g", calculatorResult); // copy into results display buffer
    ClearExpressionTokens();
}

// Given a vector of tokens in infix notation, convert and return a token vector in postfix notation.
// Based off modified Shunting Yard algorithm by (Djikstra) 
std::vector<Token*> Calculator::convertToPostfix(std::vector<Token*> infix)
{
    std::stack<Token*> operatorStack; // stack of tokens for operators to be pushed and popped from during algorithm
    std::vector<Token*> postfix; // token vector to be populated by algorithm

    infix.push_back(new Token(OPERATOR_TYPE, ')'));

    for(Token* t : infix) // iterate through infix token vector (from left to right)
    {
        if(t->data._op == '(') // detected '(' marker, push onto operator stack
        { operatorStack.push(t); }
        else if(t->type == OPERAND_TYPE) // numerical operand detected, push to postfix vector
        { postfix.push_back(t); }
        else if(t->type == OPERATOR_TYPE) // found an operator, push operators from stack onto postfix until precedence requirement is met
        {
            while (!operatorStack.empty() && equalPrecedenceOrHigher(operatorStack.top()->data._op, t->data._op)) // operator from stack has higher precedence, push to postfix vector and pop from stack
            {
                postfix.push_back(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.push(t); // Finally, add the operator to the stack now that precedence requirement is fulfilled
        }
        else if(t->data._op == ')') // detected a ')' marker, balance
        {
            while (!operatorStack.empty() && operatorStack.top()->data._op != '(') // add operators to postfix and remove from stack so long as they arent '(' markers
            {
                postfix.push_back(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.pop();
        }
    }

    if(_DEBUG_)
    {
        std::cout<<"\nCreated Postfix expression: \n";
        for(Token* _t : postfix)
        { _t->PrintInfo(); }
        std::cout<<std::endl;
    }

    return postfix; // return created postfix vector
}

// Given a vector of tokens (in post fix notation), evaluate and reduce their operations to a final result and return it.
float Calculator::EvaluatePostFix(std::vector<Token*> postfix)
{
    Token *Operand1, *Operand2; // Token holders for operand calculations when operator is detected
    float result; // float for storing operand calculations, and eventually pushing back onto stack
    std::stack<Token*> tokenStack;

    for(Token* t : postfix) // for all the tokens in the postfix expression, perform calculations and push new results onto token stack
    {
        if(t->type == OPERAND_TYPE) // found a numerical token, push onto the stack
        { tokenStack.push(t); }
        else if( t->type == OPERATOR_TYPE && isValidOperator(t->data._op)) // found an operator, grab the top two tokens from the stack (the operands) and perform respective calculation
        {
            Operand1 = tokenStack.top();
            tokenStack.pop();

            Operand2 = tokenStack.top();
            tokenStack.pop();

            // perform respective calculation for given operator
            switch (t->data._op)
            {
                case '+':
                    result = Operand2->data.value + Operand1->data.value; break;
                case '-':
                    result = Operand2->data.value - Operand1->data.value; break;
                case '*':
                    result = Operand2->data.value * Operand1->data.value; break;
                case '/':
                    result = Operand2->data.value / Operand1->data.value; break;
                default:
                    std::cout<<"Unkown operation in Evaluation of postfix token vector.";
                    break;
            }
            tokenStack.push(new Token(OPERAND_TYPE, result));
        }
    }

    float final_result = tokenStack.top()->data.value;
    if(_DEBUG_)
    { std::cout<< "Final result: " << final_result << std::endl; }
    return final_result;
}