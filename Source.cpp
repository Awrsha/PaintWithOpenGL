#include <GL/freeglut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int startX, startY, endX, endY;
int isMouseDragging = 0;
float red = 0.0, green = 0.0, blue = 0.0;
int currentShapeType = 1; // 1 for line, 2 for rectangle, 3 for circle
int isDrawingShape = 0; // Flag to check if drawing shape

typedef struct {
    float x1, y1, x2, y2;
    float r, g, b;
    int type; // 1 for line, 2 for rectangle, 3 for circle
} Shape;

#define MAX_SHAPES 100
Shape shapes[MAX_SHAPES];
int shapesCount = 0;
Shape currentShape;

void drawCircle(float centerX, float centerY, float radius) {
    int segments = 100;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(centerX + x, centerY + y);
    }
    glEnd();
}

void display() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-250, 250, -250, 250);

    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < shapesCount; ++i) {
        glColor3f(shapes[i].r, shapes[i].g, shapes[i].b);
        if (shapes[i].type == 1) {
            glBegin(GL_LINES);
            glVertex2f(shapes[i].x1, shapes[i].y1);
            glVertex2f(shapes[i].x2, shapes[i].y2);
            glEnd();
        }
        else if (shapes[i].type == 2) {
            glBegin(GL_LINE_LOOP);
            glVertex2f(shapes[i].x1, shapes[i].y1);
            glVertex2f(shapes[i].x2, shapes[i].y1);
            glVertex2f(shapes[i].x2, shapes[i].y2);
            glVertex2f(shapes[i].x1, shapes[i].y2);
            glEnd();
        }
        else if (shapes[i].type == 3) {
            float radius = fmin(fabs(shapes[i].x2 - shapes[i].x1), fabs(shapes[i].y2 - shapes[i].y1)) / 2;
            drawCircle(shapes[i].x1, shapes[i].y1, radius);
        }
    }

    glFlush();
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        startX = x - 250;
        startY = 250 - y;
        isMouseDragging = 1;

        currentShape.x1 = startX;
        currentShape.y1 = startY;
        currentShape.x2 = startX;
        currentShape.y2 = startY;

        currentShape.r = red;
        currentShape.g = green;
        currentShape.b = blue;
        currentShape.type = currentShapeType;

        isDrawingShape = 1;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && isDrawingShape) {
        endX = x - 250;
        endY = 250 - y;

        if (endX < -250) endX = -250;
        else if (endX > 250) endX = 250;
        if (endY < -250) endY = -250;
        else if (endY > 250) endY = 250;

        currentShape.x2 = endX;
        currentShape.y2 = endY;

        shapes[shapesCount] = currentShape;
        shapesCount++;

        isMouseDragging = 0;
        isDrawingShape = 0;
    }
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-250, 250, -250, 250);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'r':
        red = 1.0;
        green = 0.0;
        blue = 0.0;
        break;
    case 'g':
        red = 0.0;
        green = 1.0;
        blue = 0.0;
        break;
    case 'b':
        red = 0.0;
        green = 0.0;
        blue = 1.0;
        break;
    case '1':
        currentShapeType = 1; // Set shape type to line
        break;
    case '2':
        currentShapeType = 2; // Set shape type to rectangle
        break;
    case '3':
        currentShapeType = 3; // Set shape type to circle
        break;
    }
}

int main(int argc, char** argv) {
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