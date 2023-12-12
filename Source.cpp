// Include necessary libraries
#include <GL/freeglut.h>
#include <GL/glu.h>
#include <cstdio>
#include <cmath>
#include <cstdlib>

// Define maximum number of shapes  
const int MAX_SHAPES = 100;

// Define PI if not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Enum to represent different shape types
enum ShapeType {
    LINE = 1,
    RECTANGLE,
    CIRCLE,
    TRIANGLE,
    SQUARE,
    ELLIPSE
};

// Structure for a point in 2D space
struct Point {
    float x, y;
};

// Structure for a shape
struct Shape {
    Point start, end;
    float r, g, b;
    ShapeType type;
    float thickness;
    int isFilled; // New field for filling shapes  
};

// Global variables
Shape shapes[MAX_SHAPES];
int shapesCount = 0;
char filename[256];
Point startDrag, endDrag;
int isMouseDragging = 0;
float red = 0.0, green = 0.0, blue = 0.0;
float bgRed = 1.0, bgGreen = 1.0, bgBlue = 1.0;
ShapeType currentShapeType = LINE;
float lineThickness = 1.0;
int isDrawingShape = 0;
int isDrawingLine = 0;
int isDrawingPencil = 0;

float startX, startY, endX, endY;
Shape currentShape;

// Function prototypes  
void drawCircle(Point center, float radius);
Shape convertLineToCircle(Shape lineShape);
Shape convertLineToRectangle(Shape drawnShape);
void drawShape(Shape shape);
void drawPencilStroke(int x, int y);
void saveShapesToFile(const char* filename);
void loadShapesFromFile(const char* filename);

// Function to draw a circle
void drawCircle(Point center, float radius) {
    int segments = 100;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(center.x + x, center.y + y);
    }
    glEnd();
}

// Function to convert a line to a circle shape
Shape convertLineToCircle(Shape lineShape) {
    float lineLength = sqrt(pow(lineShape.end.x - lineShape.start.x, 2) + pow(lineShape.end.y - lineShape.start.y, 2));
    float circleRadius = lineLength / 2; // Assume circle radius is half the length of the line

    // Find the center point of the circle (midpoint of the line)
    float centerX = (lineShape.start.x + lineShape.end.x) / 2;
    float centerY = (lineShape.start.y + lineShape.end.y) / 2;

    Shape circleShape;
    circleShape.start.x = centerX - circleRadius;
    circleShape.start.y = centerY - circleRadius;
    circleShape.end.x = centerX + circleRadius;
    circleShape.end.y = centerY + circleRadius;
    circleShape.type = CIRCLE;
    circleShape.r = lineShape.r; // You might want to adjust these attributes
    circleShape.g = lineShape.g;
    circleShape.b = lineShape.b;
    circleShape.thickness = lineShape.thickness;
    circleShape.isFilled = 0; // Initially not filled

    return circleShape;
}
// Function to convert a line to a rectangle shape
Shape convertLineToRectangle(Shape drawnShape) {
    float width = fabs(drawnShape.start.x - drawnShape.end.x);
    float height = fabs(drawnShape.start.y - drawnShape.end.y);

    // Consider a line as a rectangle with some width and small height
    float minWidth = 5.0;  // You can adjust this value as needed
    float minHeight = 10.0; // You can adjust this value as needed

    if (width < minWidth) {
        width = minWidth;
    }
    if (height < minHeight) {
        height = minHeight;
    }

    drawnShape.end.x = drawnShape.start.x + width;
    drawnShape.end.y = drawnShape.start.y + height;
    drawnShape.type = RECTANGLE;

    return drawnShape;
}
// Function to draw a specific shape based on its type
void drawShape(Shape shape) {
    glColor3f(shape.r, shape.g, shape.b);
    glLineWidth(shape.thickness);

    if (shape.type == LINE) {
        glBegin(GL_LINES);
        glVertex2f(shape.start.x, shape.start.y);
        glVertex2f(shape.end.x, shape.end.y);
        glEnd();
    }
    else if (shape.type == RECTANGLE) {
        if (shape.isFilled) {
            glColor3f(shape.r, shape.g, shape.b);
            glBegin(GL_QUADS);
            glVertex2f(shape.start.x, shape.start.y);
            glVertex2f(shape.end.x, shape.start.y);
            glVertex2f(shape.end.x, shape.end.y);
            glVertex2f(shape.start.x, shape.end.y);
            glEnd();
        }
        else {
            glColor3f(shape.r, shape.g, shape.b);
            glBegin(GL_LINE_LOOP);
            glVertex2f(shape.start.x, shape.start.y);
            glVertex2f(shape.end.x, shape.start.y);
            glVertex2f(shape.end.x, shape.end.y);
            glVertex2f(shape.start.x, shape.end.y);
            glEnd();
        }
    }
    else if (shape.type == CIRCLE) {
        float radius = fmin(fabs(shape.end.x - shape.start.x), fabs(shape.end.y - shape.start.y)) / 2;
        Point center = { (shape.start.x + shape.end.x) / 2, (shape.start.y + shape.end.y) / 2 };
        drawCircle(center, radius);
    }
    // Triangle
    else if (shape.type == TRIANGLE) {
        glBegin(GL_LINE_LOOP);
        glVertex2f(shape.start.x, shape.start.y + (shape.end.y - shape.start.y));
        glVertex2f(shape.start.x + (shape.end.x - shape.start.x) / 2, shape.start.y);
        glVertex2f(shape.end.x, shape.start.y + (shape.end.y - shape.start.y));
        glEnd();
    }
    // Square
    else if (shape.type == SQUARE) {
        glBegin(GL_LINE_LOOP);
        glVertex2f(shape.start.x, shape.start.y);
        glVertex2f(shape.end.x, shape.start.y);
        glVertex2f(shape.end.x, shape.end.y);
        glVertex2f(shape.start.x, shape.end.y);
        glEnd();
    }
    // Ellipse
    else if (shape.type == ELLIPSE) {
        if (shape.isFilled) {
            // Fill ellipse
            glColor3f(shape.r, shape.g, shape.b);
            float radiusX = fabs(shape.end.x - shape.start.x) / 2;
            float radiusY = fabs(shape.end.y - shape.start.y) / 2;
            Point center = { (shape.start.x + shape.end.x) / 2, (shape.start.y + shape.end.y) / 2 };

            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(center.x, center.y);
            for (int i = 0; i <= 360; i++) {
                float rad = i * M_PI / 180;
                glVertex2f(center.x + cos(rad) * radiusX, center.y + sin(rad) * radiusY);
            }
            glEnd();
        }
        else {
            // Draw unfilled ellipse
            glColor3f(shape.r, shape.g, shape.b);
            float radiusX = fabs(shape.end.x - shape.start.x) / 2;
            float radiusY = fabs(shape.end.y - shape.start.y) / 2;
            Point center = { (shape.start.x + shape.end.x) / 2, (shape.start.y + shape.end.y) / 2 };

            glBegin(GL_LINE_LOOP);
            for (int i = 0; i < 360; i++) {
                float rad = i * M_PI / 180;
                glVertex2f(center.x + cos(rad) * radiusX, center.y + sin(rad) * radiusY);
            }
            glEnd();
        }
    }
}

void display() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-250, 250, -250, 250);

    glClearColor(bgRed, bgGreen, bgBlue, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < shapesCount; ++i) {
        drawShape(shapes[i]);
    }
    // Additional code for Line Drawer tool
    if (isDrawingLine) {
        glLineWidth(lineThickness);
        glColor3f(red, green, blue);
        glBegin(GL_LINES);
        glVertex2f(startX, startY);
        glVertex2f(endX, endY);
        glEnd();
    }

    // Additional code for Pencil tool
    if (isDrawingPencil) {
        drawPencilStroke(endX, endY);
    }

    glFlush();
}

// Function to draw a single stroke of the pencil
void drawPencilStroke(int x, int y) {
    // Code to draw pencil stroke
    // You can customize this based on your pencil drawing logic
    // For example, you can use GL_POINTS or GL_LINE_STRIP
    glPointSize(lineThickness);
    glColor3f(red, green, blue);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}


void addShapeToList(Shape shape) {
    if (shapesCount < MAX_SHAPES) {
        shapes[shapesCount++] = shape;
    }
    else {
        printf("Shape limit reached!");
    }
}

void removeShape(int index) {
    if (index >= 0 && index < shapesCount) {
        for (int i = index; i < shapesCount - 1; ++i) {
            shapes[i] = shapes[i + 1];
        }
        shapesCount--;
    }
}
/*
void saveShapesToFile(char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < shapesCount; ++i) {
        Shape shape = shapes[i];
        // Fix format string warnings
        fprintf((FILE*)file, "%d %f %f %f %f %f %f %f %f %d\n",
            shape.type,
            shape.start.x,
            shape.start.y,
            shape.end.x, 
            shape.end.y, 
            shape.r, 
            shape.g, 
            shape.b, 
            shape.thickness, 
            shape.isFilled);
    }

    fclose(file);
}

void loadShapesFromFile(char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    shapesCount = 0;
    while (!feof(file) && shapesCount < MAX_SHAPES) {
        Shape shape;
        int type, filled;
        fscanf((FILE*)file, "%d %f %f %f %f %f %f %f %f %f %d\n", &type, &shape.start.x, &shape.start.y, &shape.end.x, &shape.end.y, &shape.r, &shape.g, &shape.b, &shape.thickness, &filled);
        shape.type = (ShapeType)type;
        shape.isFilled = filled;
        shapes[shapesCount++] = shape;
    }

    fclose(file);
}
*/
Shape detectShape(Shape drawnShape) {
    float width = fabs(drawnShape.start.x - drawnShape.end.x);
    float height = fabs(drawnShape.start.y - drawnShape.end.y);

    float aspectRatio = width / height;

    // Simplified criteria for detecting a square or rectangle
    if (aspectRatio >= 0.9 && aspectRatio <= 1.1) {
        if (fabs(width - height) < 10.0) {
            drawnShape.type = SQUARE;
        }
        else {
            drawnShape.type = RECTANGLE;
        }
    }

    // Check if the shape is a line and convert it to a rectangle
    if (drawnShape.type == LINE) {
        drawnShape = convertLineToRectangle(drawnShape);
    }

    // Add more shape recognition checks here

    return drawnShape;
}


void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        startX = x - 250;
        startY = 250 - y;
        isMouseDragging = 1;

        currentShape.start.x = startX;
        currentShape.start.y = startY;
        currentShape.end.x = startX;
        currentShape.end.y = startY;

        currentShape.r = red;
        currentShape.g = green;
        currentShape.b = blue;
        currentShape.type = currentShapeType;
        currentShape.thickness = lineThickness;
        currentShape.isFilled = 0; // Initially not filled

        isDrawingShape = 1;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && isDrawingShape) {
        endX = x - 250;
        endY = 250 - y;

        currentShape.end.x = endX;
        currentShape.end.y = endY;

        addShapeToList(currentShape);

        isMouseDragging = 0;
        isDrawingShape = 0;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        int mouseX = x - 250;
        int mouseY = 250 - y;

        for (int i = shapesCount - 1; i >= 0; --i) {
            Shape shape = shapes[i];
            if (mouseX >= shape.start.x && mouseX <= shape.end.x && mouseY >= shape.start.y && mouseY <= shape.end.y) {
                removeShape(i);
                break;
            }
        }
    }
}

void mouseMove(int x, int y) {
    if (isDrawingLine) {
        // Code to draw a line dynamically as the mouse moves
        endX = x - 250;
        endY = 250 - y;
        glutPostRedisplay(); // Trigger display function
    }

    if (isDrawingPencil) {
        // Code to draw pencil strokes dynamically as the mouse moves
        int currentX = x - 250;
        int currentY = 250 - y;
        drawPencilStroke(currentX, currentY);
        glutPostRedisplay(); // Trigger display function
    }
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-250, 250, -250, 250);
}

Shape undoStack[MAX_SHAPES]; // Stack for undo operations
int undoCount = 0;

void addToUndoStack(Shape shape) {
    if (undoCount < MAX_SHAPES) {
        undoStack[undoCount++] = shape;
    }
    else {
        printf("Undo limit reached!");
    }
}

Shape popFromUndoStack() {
    if (undoCount > 0) {
        return undoStack[--undoCount];
    }
    else {
        printf("Undo stack is empty!");
        Shape emptyShape = { 0 };
        return emptyShape;
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'u': // Undo
    {
        Shape lastAction = popFromUndoStack();
        if (lastAction.type != 0) {
            addShapeToList(lastAction);
        }
    }
    break;
    case 'p':
        isDrawingLine = 0;
        isDrawingPencil = 1;
        break;
    case 'x':
        red = 0.0; green = 0.0; blue = 0.0; break;
    case 'r':
        red = 1.0; green = 0.0; blue = 0.0; break;
    case 'g':
        red = 0.0; green = 1.0; blue = 0.0; break;
    case 'b':
        red = 0.0; green = 0.0; blue = 1.0; break;
    case '1':
        currentShapeType = LINE; break;
    case '2':
        currentShapeType = RECTANGLE; break;
    case '3':
        currentShapeType = CIRCLE; break;
    case '4':
        currentShapeType = TRIANGLE; break;
    case '5':
        currentShapeType = SQUARE; break;
    case '6':
        currentShapeType = ELLIPSE; break;
    case 'q': // Change background color to red
        bgRed = 1.0; bgGreen = 0.0; bgBlue = 0.0; break;
    case 'w': // Change background color to green
        bgRed = 0.0; bgGreen = 1.0; bgBlue = 0.0; break;
    case 'e': // Change background color to blue
        bgRed = 0.0; bgGreen = 0.0; bgBlue = 1.0; break;
    case 'z': // Change background color to white
        bgRed = 1.0; bgGreen = 1.0; bgBlue = 1.0; break;
    case '+': // Increase line thickness
        lineThickness += 0.5; break;
    case '-': // Decrease line thickness
        lineThickness = fmax(1.0, lineThickness - 0.5); break;
    case 'f': // Toggle fill for the last drawn shape
        if (shapesCount > 0) {
            shapes[shapesCount - 1].isFilled = !shapes[shapesCount - 1].isFilled;
        }
        break;

    case 's': // Save shapes to a file
        saveShapesToFile("shapes.txt");
        break;

    case 'l': // Load shapes from a file
        loadShapesFromFile("shapes.txt");
        break;
    }
}
// Entry point
int main(int argc, char** argv) {
    // GLUT initialization and window setup...
    // Setting up callbacks...

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Paint Project");
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}
