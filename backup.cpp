#include <GLFW/glfw3.h>
void drawLint()
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear (GL_COLOR_BUFFER_BIT);
    glLineWidth(2);//设置线段宽度
    glBegin(GL_LINES);
    glColor3f(1.0,0.0,0.0);
    glVertex2f(0.8,1); //定点坐标范围
    glVertex2f(0,-1);
    glEnd();
}

int main(void)
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(480, 320, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /*your draw*/
        // drawPoint();
        drawLint();
        // drawTriangle();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}