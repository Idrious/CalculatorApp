#ifndef _CALCULATOR_GUI_H_
#define _CALCULATOR_GUI_H_

#include "raylib.h"
#include <iostream>

class Calculator;
// Simple holder class for holding rayGUI elements, their positions, and their logic
// Created using rayguiLayout.zero application
class CalculatorGUI
{
    public:
        CalculatorGUI(Calculator* calc);
        ~CalculatorGUI();
        void Draw();
    private:
        Calculator* calculator;

        Vector2 anchor01 = { 10, 10 };            // ANCHOR ID:1, the base anchor that GUI rectanlges are positioned off of

        // Define Control Functions
        void ClearButton();     // function for when the clear button is clicked
        void GUIProcessInput(char c); // front end wrapper function to send input to process to the calculator

        // Define controls rectangles
        Rectangle layoutRecs[20] = 
        {
            (Rectangle){ anchor01.x + 0, anchor01.y + 0, 340, 405 },        // GroupBox: CalculatorGroupBox
            (Rectangle){ anchor01.x + 210, anchor01.y + 100, 115, 25 },     // Label: InputLabel
            (Rectangle){ anchor01.x + 10, anchor01.y + 340, 50, 50 },       // ImageButtonEx: ZeroButton
            (Rectangle){ anchor01.x + 65, anchor01.y + 340, 50, 50 },       // ImageButtonEx: DecimalButton
            (Rectangle){ anchor01.x + 120, anchor01.y + 340, 50, 50 },      // ImageButtonEx: NegativeButton
            (Rectangle){ anchor01.x + 10, anchor01.y + 285, 50, 50 },       // ImageButtonEx: OneButton
            (Rectangle){ anchor01.x + 65, anchor01.y + 285, 50, 50 },       // ImageButtonEx: TwoButton
            (Rectangle){ anchor01.x + 120, anchor01.y + 285, 50, 50 },      // ImageButtonEx: ThreeButton
            (Rectangle){ anchor01.x + 10, anchor01.y + 230, 50, 50 },       // ImageButtonEx: FourButton
            (Rectangle){ anchor01.x + 65, anchor01.y + 230, 50, 50 },       // ImageButtonEx: FiveButton
            (Rectangle){ anchor01.x + 120, anchor01.y + 230, 50, 50 },      // ImageButtonEx: SixButton
            (Rectangle){ anchor01.x + 10, anchor01.y + 175, 50, 50 },       // ImageButtonEx: SevenButton
            (Rectangle){ anchor01.x + 65, anchor01.y + 175, 50, 50 },       // ImageButtonEx: EightButton
            (Rectangle){ anchor01.x + 120, anchor01.y + 175, 50, 50 },      // ImageButtonEx: NineButton
            (Rectangle){ anchor01.x + 175, anchor01.y + 175, 50, 50 },      // ImageButtonEx: MultiplyButton
            (Rectangle){ anchor01.x + 175, anchor01.y + 230, 50, 50 },      // ImageButtonEx: DivideButton
            (Rectangle){ anchor01.x + 175, anchor01.y + 285, 50, 50 },      // ImageButtonEx: PlusButton
            (Rectangle){ anchor01.x + 175, anchor01.y + 340, 50, 50 },      // ImageButtonEx: MinusButton
            (Rectangle){ anchor01.x + 230, anchor01.y + 175, 100, 50 },     // ImageButtonEx: ClearButton
            (Rectangle){ anchor01.x + 230, anchor01.y + 340, 100, 50 },     // ImageButtonEx: EqualsButton
        };
};

#endif