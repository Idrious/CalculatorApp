/*
Author: Joshua Dempsey
Date: 3/6/21

Project Goal:
    Write a calculator app with a User Interface

Project Requirements:
    User Interface Requirements:
        The UI should have numeric buttons 0-9, period, +/- [+]
        The UI should have operators plus, minus, multiply, divide, clear, equal [+]
        The UI should have a results view [+]
        The input should accept numbers by UI button press and keyboard keys [+]

Feature Requirements:
    The calculations should work with negative numbers [+]
    The calculations should work with decimal numbers [+]
    The entered number should be displayed [+]
    The result of the operation should be displayed [+]

Code Requirements:
    The code should include comments [+]
    The code should include unit tests [+]
    The code should demonstrate best design practices [+]
*/

#include "main.h"

// main driver function, Initialize and run application
int main() 
{
    // screen dimensions
    int screenWidth = 360;
    int screenHeight = 425;

    // initialize window
    InitWindow(screenWidth, screenHeight, "Simple Calculator.");
    SetTargetFPS(60);

    Calculator* calculator = new Calculator();
    calculator->GUI = new CalculatorGUI(calculator); // Set the GUI here to help UnitTesting, GUI dependancies too much of a headache to manage, this is an easier workaround for now.
    // set up calculator

    // Main game loop should check for input and draw the GUI
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        int c = GetKeyPressed(); // get a key press (for keyboard input numericals)
        calculator->ProcessInput(c);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            calculator->GUI->Draw();  // Draw calculator GUI
        EndDrawing();
    }

    delete calculator->GUI;
    delete calculator;
    return 0;
}
