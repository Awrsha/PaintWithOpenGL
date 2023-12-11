# Paint Project

This Paint Project is a simple drawing application built using OpenGL and FreeGLUT. It allows users to draw various shapes, change colors, adjust line thickness, and save/load drawings to/from a file.

## Features

- **Drawing Tools**: Includes tools for drawing lines, rectangles, circles, triangles, squares, and ellipses.
- **Color Selection**: Users can select colors for drawing shapes and lines.
- **Line Thickness**: Adjust the thickness of lines for drawing.
- **Fill Option**: Toggle fill mode for shapes.
- **Save/Load**: Save drawn shapes to a file and load shapes from a file.
- **Undo Functionality**: Supports undo for the last action performed.

## How to Use

- **Drawing Shapes**: Select a shape from the keyboard shortcuts (1-6) and click/drag to draw on the canvas.
- **Color Selection**: Use keyboard shortcuts (x, r, g, b) to change colors (black, red, green, blue).
- **Background Color**: Change canvas background color using keyboard shortcuts (q, w, e, z).
- **Line Thickness**: Increase (+) or decrease (-) line thickness using keyboard shortcuts.
- **Fill Toggle**: Toggle shape fill using the 'f' key.
- **Undo**: Press 'u' to undo the last action.
- **Save/Load**: Use 's' to save shapes to a file and 'l' to load shapes from a file.

## How to Run

1. Ensure OpenGL and FreeGLUT are installed.
2. Compile the code using a C/C++ compiler with the necessary OpenGL and FreeGLUT libraries.
3. Run the compiled executable.

## Project Structure

- `main.cpp`: Contains the main code for the Paint application.
- `README.md`: This file, providing an overview and instructions for the project.
- `shapes.txt`: Sample file for saving/loading shapes.
