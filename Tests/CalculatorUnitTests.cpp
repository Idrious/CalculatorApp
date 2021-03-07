#include "../src/main.h"
#include <vector>
#include <gtest/gtest.h>

TEST(SanityCheckTest, SanityCheck)
{
    EXPECT_EQ(true, true);
    EXPECT_EQ(false, false);
    EXPECT_NE(true, false);
}

/*
----------------------------------------------------------------
------------------ General Functions Tests ---------------------
----------------------------------------------------------------
*/

// Test the isIn() function
TEST(GeneralFunctionTests, isInFunctionTest)
{
    std::vector<int> testV = { 'A', 'B', 'C'};

    EXPECT_TRUE(isIn('A', testV));
    EXPECT_FALSE(isIn('a', testV));

    testV.clear();
}

// Test the isValidNum() function
TEST(GeneralFunctionTests, isValidNumFunctionTest)
{
    EXPECT_TRUE(isValidNum('0')); // valid input
    EXPECT_TRUE(isValidNum(48)); // (int)'0'
    EXPECT_EQ(isValidNum('0'), isValidNum(48)) << printf("Character input did not match its integer equivalent."); // matching input
    EXPECT_NE(isValidNum('0'), isValidNum(0)); // not matching input

    for(int i = 0; i < 10; i++) // numerical constants
    { EXPECT_TRUE(isValidNum('0'+i)) << printf("Numerical input was not valid.");  }

    EXPECT_TRUE(isValidNum('9'));
    EXPECT_FALSE(isValidNum(0));
}

// Test the isValidOperator() function
TEST(GeneralFunctionTests, isValidOperatorFunctionTest)
{
    EXPECT_FALSE(isValidOperator(0)); // should never accept 0
    EXPECT_FALSE(isValidOperator('x')); // not actual operators
    EXPECT_FALSE(isValidOperator('X')); // not actual operators

    char c = '0';
    for(int i = 0; i < 10; i++) // not operators, numerical constants
    { EXPECT_FALSE(isValidOperator(c+i)); }

    EXPECT_TRUE(isValidOperator('+')); // valid input
    EXPECT_TRUE(isValidOperator('-')); // valid input
    EXPECT_TRUE(isValidOperator('/')); // valid input
    EXPECT_TRUE(isValidOperator('*')); // valid input
}

// Test the ifModifiesInput() function
TEST(GeneralFunctionTests, ifModifiesInputFunctionTest)
{
    EXPECT_FALSE(ifModifiesInput(0)); // should never accept 0
    for(int i = 0; i < 10; i++) // not operators, numerical constants
    { EXPECT_FALSE(ifModifiesInput('0'+i)); }

    EXPECT_FALSE(ifModifiesInput('-')); // valid input
    EXPECT_FALSE(ifModifiesInput('+')); // valid input
    EXPECT_FALSE(ifModifiesInput('=')); // valid input

    EXPECT_TRUE(ifModifiesInput('~')); // valid input
    EXPECT_TRUE(ifModifiesInput('.')); // valid input
}

// Test the precedence() and equalPrecedenceOrHigher() functions
TEST(GeneralFunctionTests, precedenceFunctionsTests)
{
    char op1 = '*'; // test a multiplicative operator
    char op2 = '+';

    EXPECT_EQ(precedence('+'),precedence('-')) << printf("Operators + and - did NOT have same calculated precedence.");
    EXPECT_EQ(precedence('*'),precedence('/')) << printf("Operators * and / did NOT have same calculated precedence.");

    bool result = precedence(op1) > precedence(op2);
    EXPECT_TRUE(equalPrecedenceOrHigher(op1, op2));
    EXPECT_EQ(equalPrecedenceOrHigher(op1, op2), result);

    EXPECT_EQ(precedence('='), 0);
}

/*
----------------------------------------------------------------
--------------- Constructor Validation Tests -------------------
----------------------------------------------------------------
*/

// Test the Token constructor
TEST(CalculatorUnitTests, TokenCreationTests)
{
    float testValue = 2.5f;
    char testOp = '-';

    // Operand type token check
    Token* newOperandToken = new Token(OPERAND_TYPE, testValue);
    EXPECT_EQ(newOperandToken->type, OPERAND_TYPE)  << printf("created an operator type token, but internal type incorrect");
    EXPECT_EQ(newOperandToken->data.value, testValue);

    // Operator type token check
    Token* newOperatorToken = new Token(OPERATOR_TYPE, testOp);
    EXPECT_EQ(newOperatorToken->type, OPERATOR_TYPE) << printf("created an Operator type token, but internal type incorrect");
    EXPECT_EQ(newOperatorToken->data._op, testOp);

    delete newOperandToken;
    delete newOperatorToken;
}

// Test the Calculator constructor
TEST(CalculatorUnitTests, CalculatorCreationTests)
{
    Calculator* calc = new Calculator();
    
    std::vector<Token*> exp = calc->GetTokens();
    EXPECT_EQ(exp.size(), 0) << printf("token vector not empty on calculator creation.");
    
    char* inputBuffer = calc->GetInputBuffer();
    EXPECT_EQ(inputBuffer[0], 0) << printf("inputBuffer was not empty/cleared on calculator creation.");

    size_t inputSize = calc->GetInputBufferLength();
    EXPECT_EQ(inputSize, 0) << printf("inputSize was not 0 on calculator creation.");

    char* resultBuffer = calc->GetResultsBuffer();
    EXPECT_EQ(resultBuffer[0], 0) << printf("resultBuffer was not empty/cleared on calculator creation.");
}

/*
----------------------------------------------------------------
------------------ Calculator Function Tests -------------------
----------------------------------------------------------------
*/

// fixture struct for CalculatorUnitTests 
struct CalculatorFunctionsUnitTests : public ::testing::Test
{
    Calculator* calculator;
    virtual void SetUp() override
    { calculator = new Calculator(); }

    // given a constructed string of testing input, process it one character at a time and pass it to the calculators respective function 
    CalculatorInfo AddTestInputToCalculator(std::string testInput)
    {
        for(char c : testInput) 
        { calculator->ProcessInput(c); }
        return calculator->GetCalcInfo();
    }

    virtual void TearDown() override
    { delete calculator; }
};

// Test that buffers are cleared properly by the corresponding functions
TEST_F(CalculatorFunctionsUnitTests, CalculatorClearFunctionsTest)
{
    AddTestInputToCalculator("1"); // testing cuntion passes input to fixture 
    ASSERT_NE(calculator->GetInputBufferLength(), 0);
    calculator->ClearInputBuffer(); // clear buffer
    ASSERT_EQ(calculator->GetInputBufferLength(), 0);

    AddTestInputToCalculator("1+2");
    ASSERT_NE(calculator->GetTokens().size(), 0);
    calculator->ClearExpressionTokens();
    ASSERT_EQ(calculator->GetTokens().size(), 0);

    AddTestInputToCalculator("1+2=");
    ASSERT_NE(strlen(calculator->GetResultsBuffer()), 0);
    calculator->ClearResultsBuffer();
    ASSERT_EQ(strlen(calculator->GetResultsBuffer()), 0);
}

// Test the Calculator input functions test on a test input string with no operators (numbers not parsed)
TEST_F(CalculatorFunctionsUnitTests, CalculatorInputFunctionTestsNoOperators)
{
    std::string inputTest = "123"; // craft some test input (In this scenario it's all valid and has yet to tokenize)
    AddTestInputToCalculator(inputTest); // testing cuntion passes input to fixture 

    EXPECT_EQ(inputTest.size(), calculator->GetInputBufferLength()) << printf("Given a string of valid numerical input, the sizes between the input string and the internal input sizes differ.");
    EXPECT_EQ(calculator->GetTokens().size(), 0) << printf("No operator given in this test string, yet parsed tokens vector is NOT empty.");
}

// Test the Calculator input functions test on a test input string composed of basic instructions
TEST_F(CalculatorFunctionsUnitTests, CalculatorInputFunctionTestsBasicExpression)
{
    std::string inputTest = "1+2"; // craft some test input (In this scenario it's all valid)
    AddTestInputToCalculator(inputTest); // testing function passes input to fixture calculator

    EXPECT_EQ(calculator->GetTokens()[0]->type, OPERAND_TYPE) << printf("Expected token[0] to be a float operand, it was not.");
    EXPECT_EQ(calculator->GetTokens()[0]->data.value, 1.0f) << printf("Expected token[0] to have a value of 1, it was not.");

    EXPECT_EQ(calculator->GetTokens()[1]->type, OPERATOR_TYPE) << printf("Expected token[1] to be a character operator, it was not.");
    EXPECT_EQ(calculator->GetTokens()[1]->data._op, '+') << printf("Expected token[1] operator to be '+', it was not.");
    
    // last token is not tokenized
    EXPECT_STREQ(calculator->GetInputBuffer(), "2") << printf("Expected input buffer to be populated with '2', it was not.");
}

// Test standard decimal input
TEST_F(CalculatorFunctionsUnitTests, CalculatorInputFunctionTestsDecimalInput)
{
    std::string inputTest = "1.1+2.2+"; // craft some test input (In this scenario it's all valid and has yet to tokenize)
    CalculatorInfo info = AddTestInputToCalculator(inputTest); // testing function passes input to fixture calculator

    EXPECT_EQ(info.tokens[0]->type, OPERAND_TYPE) << printf("Expected token[0] to be a float operand, it was not.");
    EXPECT_EQ(info.tokens[0]->data.value, 1.1f);

    EXPECT_EQ(info.tokens[1]->type, OPERATOR_TYPE) << printf("Expected token[1] to be a character operator, it was not.");
    EXPECT_EQ(info.tokens[1]->data._op, '+');
    
    EXPECT_EQ(info.tokens[2]->type, OPERAND_TYPE) << printf("Expected token[2] to be a float operand, it was not.");
    EXPECT_EQ(info.tokens[2]->data.value, 2.2f);
}

// test if multiple decimals are properly ignored
TEST_F(CalculatorFunctionsUnitTests, CalculatorInputFunctionTestsMultipleDecimalInput)
{
    std::string inputTest = "1...1+"; // craft some test input (In this scenario it's all valid and has yet to tokenize last symbol)
    CalculatorInfo info = AddTestInputToCalculator(inputTest); // testing function passes input to fixture calculator

    EXPECT_EQ(info.tokens[0]->type, OPERAND_TYPE) << printf("Expected token[0] to be a float operand, it was not.");
    EXPECT_EQ(info.tokens[0]->data.value, 1.1f);
}

// test singular negative input
TEST_F(CalculatorFunctionsUnitTests, CalculatorInputFunctionTestsNegativeInput)
{
    std::string inputTest = "~11+"; // craft some test input (In this scenario it's all valid and has yet to tokenize last symbol)
    CalculatorInfo info = AddTestInputToCalculator(inputTest); // testing function passes input to fixture calculator

    EXPECT_EQ(info.tokens[0]->type, OPERAND_TYPE) << printf("Expected token[0] to be a float operand, it was not.");
    EXPECT_EQ(info.tokens[0]->data.value, -11);
}

// test multiple negative input onto a operand, see if value properly swapped
TEST_F(CalculatorFunctionsUnitTests, CalculatorInputFunctionTestsMultipleNegativeInput)
{
    std::string inputTest = "~~11+"; // craft some test input (In this scenario it's all valid and has yet to tokenize last symbol)
    CalculatorInfo info = AddTestInputToCalculator(inputTest); // testing function passes input to fixture calculator

    EXPECT_EQ(info.tokens[0]->type, OPERAND_TYPE) << printf("Expected token[0] to be a float operand, it was not.");
    EXPECT_EQ(info.tokens[0]->data.value, 11);
}

/*
----------------------------------------------------------------
------------- Calculator private functions Tests ---------------
----------------------------------------------------------------
*/

class CalculatorPrivateTEST : public ::testing::Test
{
    protected:
        float EvaluatePostFix(std::vector<Token*> postfix) { return calculator.EvaluatePostFix(postfix); } // Given a vector of tokens, evaluate and reduce their operations to a final result and return it.
        std::vector<Token*> convertToPostfix(std::vector<Token*> infix) { return calculator.convertToPostfix(infix); } // Given a vector of tokens in infix notation, convert and return a token vector in postfix notation.
        bool CheckForInputEndingControl(int c) { return calculator.CheckForInputEndingControl(c); } // Check for what type of operator was supplied, if '=', evaluate the expression
        void AddInput(std::string s) //wrapper function for processing input for calculator
        {  for(char c : s) { calculator.ProcessInput(c); } }
    private:
        Calculator calculator;
};


TEST_F(CalculatorPrivateTEST, TestInputEndingControls)
{
    AddInput("123");

    EXPECT_FALSE(CheckForInputEndingControl('1'));
    EXPECT_FALSE(CheckForInputEndingControl('~'));
    EXPECT_FALSE(CheckForInputEndingControl('.'));
    
    EXPECT_TRUE(CheckForInputEndingControl('+'));
    AddInput("123");

    EXPECT_TRUE(CheckForInputEndingControl('='));
}

/*
----------------------------------------------------------------
-------------- Infix/PostFix conversion Tests ------------------
----------------------------------------------------------------
*/


// test if algorithm produced a valid post fix token as expected
TEST_F(CalculatorPrivateTEST, convertToPostfixTestSimple)
{
    // 1 + 2 as a infix token vector
    std::vector<Token*> infixTestVector = {new Token(OPERAND_TYPE, 1.0f), new Token(OPERATOR_TYPE, '+') ,new Token(OPERAND_TYPE, 2.0f)  };
    // 1 2 + expected output vector
    std::vector<Token*> expectedOutput = {new Token(OPERAND_TYPE, 1.0f),new Token(OPERAND_TYPE, 2.0f), new Token(OPERATOR_TYPE, '+')};
    // converted vector
    std::vector<Token*> converted = convertToPostfix(infixTestVector);

    // check size
    ASSERT_EQ(expectedOutput.size(), converted.size()) << printf("expected postfix and converted result are NOT the same size.");
    for(int i = 0; i < expectedOutput.size(); i++)
    {
        ASSERT_EQ(expectedOutput[i]->type, converted[i]->type); // check type
        if(expectedOutput[i]->type == OPERAND_TYPE)
        { EXPECT_EQ(expectedOutput[i]->data.value, converted[i]->data.value); }
        else
        { EXPECT_EQ(expectedOutput[i]->data._op, converted[i]->data._op); }
    }
}

// test if algorithm produced a valid post fix token as expected, take operator precedence into account
TEST_F(CalculatorPrivateTEST, convertToPostfixTestComplicated)
{
    // 1 + 2 * 3 as a infix token vector
    std::vector<Token*> infixTestVector = {new Token(OPERAND_TYPE, 1.0f), new Token(OPERATOR_TYPE, '+') ,new Token(OPERAND_TYPE, 2.0f), new Token(OPERATOR_TYPE, '*'), new Token(OPERAND_TYPE, 3.0f)};
    // 1 2 3 * + expected output vector
    std::vector<Token*> expectedOutput = {new Token(OPERAND_TYPE, 1.0f),new Token(OPERAND_TYPE, 2.0f),new Token(OPERAND_TYPE, 3.0f),new Token(OPERATOR_TYPE, '*'), new Token(OPERATOR_TYPE, '+')};
    // converted vector
    std::vector<Token*> converted = convertToPostfix(infixTestVector);

    // check size
    ASSERT_EQ(expectedOutput.size(), converted.size()) << printf("expected postfix and converted result are NOT the same size.");
    for(int i = 0; i < expectedOutput.size(); i++)
    {
        ASSERT_EQ(expectedOutput[i]->type, converted[i]->type); // check type
        if(expectedOutput[i]->type == OPERAND_TYPE)
        { EXPECT_EQ(expectedOutput[i]->data.value, converted[i]->data.value); }
        else
        { EXPECT_EQ(expectedOutput[i]->data._op, converted[i]->data._op); }
    }
}

// Test the EvaluatePostFix function for accuracy
TEST_F(CalculatorPrivateTEST, evaluatePostFixTest)
{
    // 1 2 3 * + expected output vector
    std::vector<Token*> postfixTest = {new Token(OPERAND_TYPE, 1.0f),new Token(OPERAND_TYPE, 2.0f),new Token(OPERAND_TYPE, 3.0f),new Token(OPERATOR_TYPE, '*'), new Token(OPERATOR_TYPE, '+')};
    float expectedOutput = 7;
    EXPECT_EQ(EvaluatePostFix(postfixTest), 7);
}

/*
----------------------------------------------------------------
--------------------- Calculations Tests -----------------------
----------------------------------------------------------------
*/

// Test calculations of expressions given to the calculator and compare their results
TEST_F(CalculatorFunctionsUnitTests, CalculatorTestCalculations)
{
    std::string inputTest = "1+2="; // craft some valid test expression
    CalculatorInfo info = AddTestInputToCalculator(inputTest); // testing function passes input to fixture calculator
    EXPECT_EQ(calculator->GetResult(), 1+2);

    inputTest = "1+4+5=";
    AddTestInputToCalculator(inputTest); // testing function passes input to fixture calculator
    EXPECT_EQ(calculator->GetResult(), 1+4+5);

    inputTest = "16*4=";
    AddTestInputToCalculator(inputTest); // testing function passes input to fixture calculator
    EXPECT_EQ(calculator->GetResult(), 16*4);

    inputTest = "5/2=";
    AddTestInputToCalculator(inputTest); // testing function passes input to fixture calculator
    EXPECT_EQ(calculator->GetResult(), 5.0f/2.0f);

    inputTest = "1+4*5+~8/2=";
    AddTestInputToCalculator(inputTest); // testing function passes input to fixture calculator
    EXPECT_EQ(calculator->GetResult(), 1+4*5+(-8)/2);
}

// run tests
int main(int argc, char ** argv)
{
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}