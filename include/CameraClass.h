#include <stdio.h>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:

    Camera(){};
    void Init(GLFWwindow* inwindow, float width, float height);
    // 摄像机更新函数
    void update();
    glm::mat4 projection;
    glm::mat4 view;

    GLFWwindow* window;
    double lastX, lastY;
    double yaw, pitch;
    bool firstMouse;
    double fov;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float screenWidth, screenHeight;


    // 输入函数
    void processInput(GLFWwindow *window);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};

Camera camera;

void Camera::Init(GLFWwindow* inwindow, float width, float height)
{
    window = inwindow;
    screenWidth = width;
    screenHeight = height;
    lastX = 0.0f;
    lastY = 0.0f;
    yaw = 0.0f;
    pitch = 0.0f;
    fov = 45.0f;
    firstMouse = true;

    cameraPos   = glm::vec3(-10.0f, 0.0f, 0.0f);
    cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
}


void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;
    // printf("[%lf,%lf] - [%f,%f] ,[%f,%f]\r\n",xpos, ypos, xoffset, yoffset,yaw,pitch);

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    // cameraFront = glm::normalize(glm::vec3(0.0f ,0.0f, 1.0f));


    // glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraPos));
    // cameraUp = glm::normalize(glm::cross(cameraFront, cameraRight));
    
    printf("[%f,%f,%f]\r\n",cameraFront.x,cameraFront.y,cameraFront.z);
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 45.0f)
        fov = 45.0f;
}

void Camera::processInput(GLFWwindow *window)
{
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	float lastFrame = 0.0f; // 上一帧的时间
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	float cameraSpeed = 0.1f * deltaTime;


    // float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)										
        cameraPos -= cameraSpeed * cameraFront;												//前后移动，朝cameraFront方向移动
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;		//左右移动，朝cameraRight方向移动
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;		//cross()叉乘，normalize()标准化
}


void mouse_callback_(GLFWwindow* window, double xpos, double ypos)
{
    camera.mouse_callback(window, xpos, ypos);
}

void scroll_callback_(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.scroll_callback(window, xoffset, yoffset);
}

void Camera::update()
{
    processInput(window);

    // void (*mouse_callback_)(GLFWwindow* window, double xpos, double ypos) = this->mouse_callback;
    // void (*scroll_callback_)(GLFWwindow* window, double xoffset, double yoffset) = this->scroll_callback;

    glfwSetCursorPosCallback(window, mouse_callback_);
    glfwSetScrollCallback(window, scroll_callback_);


    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);		
    projection = glm::perspective((float)glm::radians(fov), screenWidth / screenHeight, 0.1f, 100.0f);
}

