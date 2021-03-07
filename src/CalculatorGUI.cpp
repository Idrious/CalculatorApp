#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "CalculatorGUI.h"
#include "main.h"

// Clear the InputBuffer from the calculator, if the inputBuffer is empty, clear the expression vector as well
void CalculatorGUI::ClearButton()
{
    if(calculator->GetInputBufferLength() == 0) // clear the expression and results if clearbutton pressed on an empty input buffer
    { calculator->ClearExpressionTokens(); calculator->ClearResultsBuffer();} 
    calculator->ClearInputBuffer();
}

// front end wrapper function to send input to process to the calculator
void CalculatorGUI::GUIProcessInput(char c) 
{
    // std::cout<<"GUI process input event.\n";
    calculator->ProcessInput(c);
}

// Draw function for CalculatorGUI. Checks input for all buttons and calls corresponding functions if an OnClick event is triggered,
void CalculatorGUI::Draw()
{
    GuiGroupBox(layoutRecs[0], "Calculator UI");

    std::vector<Token*> expressionTokens = calculator->GetTokens(); // get current vector of tokens for expression printing
    char expression[512]; // expression buffer
    char bit[32]; // smaller additive buffer
    strcpy(expression, "exp: "); // TODO: unsafe function, change to something better
    for(Token* t : expressionTokens)
    {
        if(t->type==OPERAND_TYPE){ sprintf(bit, "%g", t->data.value); } // if token is an operand, print its value
        else { sprintf(bit, " %c ", t->data._op); } // otherwise, print its single character operator
        strcat(expression, bit); // TODO: unsafe function, change to something better
    }

    // Draw the contents of the expression vector formatted slightly nicer
    GuiDrawText(expression,(Rectangle){layoutRecs[1].x, layoutRecs[1].y - 16, layoutRecs[1].width, layoutRecs[1].height }, 2, BLACK);
    
    // Draw the contents of the results buffer, not using GetResult() for formatting reasons
    GuiDrawText(calculator->GetResultsBuffer(), (Rectangle){layoutRecs[1].x, layoutRecs[1].y - 32, layoutRecs[1].width, layoutRecs[1].height }, 2, BLACK);
    
    // Draw the contents of the input buffer
    GuiDrawText(calculator->GetInputBuffer(), layoutRecs[1], 2, BLACK);
    
    // raygui Button Logic functions for button controls
    if (GuiImageButtonEx(layoutRecs[2],  "0", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 })) GUIProcessInput('0');
    if (GuiImageButtonEx(layoutRecs[3],  ".", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 })) GUIProcessInput('.');
    if (GuiImageButtonEx(layoutRecs[4],  "+/-", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 })) GUIProcessInput('~');
    if (GuiImageButtonEx(layoutRecs[5],  "1", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 }))  GUIProcessInput('1');
    if (GuiImageButtonEx(layoutRecs[6],  "2", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 }))  GUIProcessInput('2');
    if (GuiImageButtonEx(layoutRecs[7],  "3", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 }))  GUIProcessInput('3');
    if (GuiImageButtonEx(layoutRecs[8],  "4", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 }))  GUIProcessInput('4');
    if (GuiImageButtonEx(layoutRecs[9],  "5", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 }))  GUIProcessInput('5');
    if (GuiImageButtonEx(layoutRecs[10], "6", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 }))  GUIProcessInput('6');
    if (GuiImageButtonEx(layoutRecs[11], "7", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 }))  GUIProcessInput('7');
    if (GuiImageButtonEx(layoutRecs[12],  "8", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 })) GUIProcessInput('8');
    if (GuiImageButtonEx(layoutRecs[13],  "9", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 })) GUIProcessInput('9');
    if (GuiImageButtonEx(layoutRecs[14],  "x", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 })) GUIProcessInput('*');
    if (GuiImageButtonEx(layoutRecs[15],  "/", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 })) GUIProcessInput('/');
    if (GuiImageButtonEx(layoutRecs[16],  "+", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 })) GUIProcessInput('+');
    if (GuiImageButtonEx(layoutRecs[17],  "-", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 })) GUIProcessInput('-');
    if (GuiImageButtonEx(layoutRecs[18],  "Clear", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 })) ClearButton();
    if (GuiImageButtonEx(layoutRecs[19],  "Equals", GetTextureDefault(), (Rectangle){ 0, 0, 1, 1 })) GUIProcessInput('=');
}

CalculatorGUI::CalculatorGUI(Calculator* calc)
{
    calculator = calc;
}

CalculatorGUI::~CalculatorGUI()
{
    // Default deconstructor
}
