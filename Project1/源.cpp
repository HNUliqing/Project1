#include<Windows.h>
#include <glad/glad.h>
#include<GL/GLU.h>

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
//鼠标点击回调函数
double mouse_pos[2]={0};
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

//使用Möller-Trumbore算法检测射线与三角形相交的示例代码：
bool IntersectTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, float& distance);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
//记录是否被鼠标点击
int index[10]={0};
// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f, -0.5f,  
        0.5f,  0.5f, -0.5f,  
        0.5f,  0.5f, -0.5f,  
    -0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  

    -0.5f, -0.5f,  0.5f,  
        0.5f, -0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f,  

    -0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  

        0.5f,  0.5f,  0.5f,  
        0.5f,  0.5f, -0.5f,  
        0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f,  

    -0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f,  0.5f,  
        0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f, -0.5f,  

    -0.5f,  0.5f, -0.5f,  
        0.5f,  0.5f, -0.5f,  
        0.5f,  0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f, -0.5f
};
// world space positions of our cubes
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 vertices_real[36];

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("7.3.camera.vs", "7.3.camera.fs");

    //将坐标转换成空间中真实坐标
    for(int i=0;i<36;i++)
    {
        vertices_real[i]=glm::vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
    }
    // glm::vec3 rayOrigin = glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z);
    // glm::vec3 rayDirection = glm::normalize(glm::vec3(worldX, worldY, worldZ) - rayOrigin);
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    // ourShader.setInt("texture1", 0);
    // ourShader.setInt("texture2", 1);
    

    //add flat 
    Shader ourShader_flat("6.1.coordinate_systems.vs", "6.1.coordinate_systems.fs");
    float vertices_flat[] = {
        // positions       
         10.5f,  10.5f, -10.0f,
         10.5f, -10.5f, -10.0f,
        -10.5f, -10.5f, -10.0f,
        -10.5f,  10.5f, -10.0f,
          0.05f,   0.0025f,   0.0f,
          0.05f,  -0.0025f,   0.0f,
         -0.05f,  -0.0025f,   0.0f,
         -0.05f,   0.0025f,   0.0f ,
           0.0025f,  0.05f,     0.0f,
          -0.0025f,  0.05f,     0.0f,
          -0.0025f,  -0.05f,    0.0f,
           0.0025f,  -0.05f,    0.0f 
    };
    unsigned int indices_flat[] = {
        0, 1, 3, // first triangle
        1, 2, 3,  // second triangle
        4, 5, 7,
        5, 6, 7,
        8, 9 ,11,
        9, 10 ,11

    };
    unsigned int VBO_flat, VAO_flat, EBO_flat;
    glGenVertexArrays(1, &VAO_flat);
    glGenBuffers(1, &VBO_flat);
    glGenBuffers(1, &EBO_flat);

    glBindVertexArray(VAO_flat);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_flat);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_flat), vertices_flat, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_flat);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_flat), indices_flat, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    ourShader_flat.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
          
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            if(index[i]==1)
                continue;
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // activate shader
        ourShader_flat.use();
      
        // create transformations
        glm::mat4 model_flat         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view_flat          = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection_flat    = glm::mat4(1.0f);
        model_flat      = glm::rotate(model_flat, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view_flat       = glm::translate(view_flat, glm::vec3(0.0f, 0.0f, -3.0f));
        projection_flat = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        ourShader_flat.setMat4("projection", projection_flat);
        ourShader_flat.setMat4("model", model_flat);
        ourShader_flat.setMat4("view", view_flat);
        // render container
        glBindVertexArray(VAO_flat);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO_flat);
    glDeleteBuffers(1, &VBO_flat);
    glDeleteBuffers(1, &EBO_flat);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // 获取鼠标当前位置，存储在xpos和ypos中
        glfwGetCursorPos(window, &mouse_pos[0], &mouse_pos[1]);
        // 输出鼠标位置，可以替换成其他想要进行的操作
        std::cout << "Clicked at position: " << mouse_pos[0] << "  " << mouse_pos[1] << std::endl;
   
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        GLdouble modelview[16];
        GLdouble projection[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);

        GLdouble worldX, worldY, worldZ;
        GLfloat depth;
        glReadPixels(mouse_pos[0], viewport[3] - mouse_pos[1], 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        depth = depth * 2.0f - 1.0f;
        
        gluUnProject(mouse_pos[0], viewport[3] - mouse_pos[1], 0, modelview, projection, viewport, &worldX, &worldY, &worldZ);
        glm::vec3 temp=glm::vec3(worldX,worldY,worldZ);
        std::cout << "Cursor at : " << worldX << " - " << worldY << " - " << worldZ << std::endl;
        float distance=0;
        bool judge=IntersectTriangle(cameraPos,temp,vertices_real[0],vertices_real[1],vertices_real[2],distance);
        if(judge==true)
        {
            std::cout<<"distance:"<<distance<<std::endl;
            index[0]=1;
        }
        
     }
}

bool IntersectTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, float& distance)
{
    const float EPSILON = 0.0000001f;
    glm::vec3 edge1, edge2, h, s, q;
    float a, f, u, v;

    edge1 = v2 - v1;
    edge2 = v3 - v1;

    h = glm::cross(rayDirection, edge2);
    a = glm::dot(edge1, h);

    if (a > -EPSILON && a < EPSILON)
        return false;   

    f = 1.0f / a;
    s = rayOrigin - v1;

    u = f * glm::dot(s, h);

    if (u < 0.0f || u > 1.0f)
        return false;

    q = glm::cross(s, edge1);

    v = f * glm::dot(rayDirection, q);

    if (v < 0.0f || u + v > 1.0f)
        return false;

    distance = f * glm::dot(edge2, q);

    if (distance > EPSILON)
        return true;

    return false;
}