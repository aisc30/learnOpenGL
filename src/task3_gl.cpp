#include <stdio.h>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>
#include <math.h>
#include "ShaderClass.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;


const char* vertexShaderSource = "\n" 
	//"#version 330 core\n" 
	"#version 300 es\n" 
	"layout (location = 0) in vec3 aPos;\n" 
	"layout (location = 1) in vec3 aColor;\n"
	"layout (location = 2) in vec2 aTexCoord;\n" 
    "uniform mat4 transform;\n" 
	"out vec3 ourColor;\n" 
	"out vec2 ourTexCoord;\n" 
	"void main()\n" 
	"{\n" 
	"    gl_Position = transform*vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" 
	"    ourColor = aColor;\n" 
	"    ourTexCoord = aTexCoord;\n" 
	"}";

const char* fragmentShaderSource = "\n" 
	//"#version 330 core\n" 
	"#version 300 es\n" 
	"precision mediump float;\n"
	"out vec4 FragColor;\n" 
	"in vec3 ourColor;\n" 
	"in vec2 ourTexCoord;\n" 
	"uniform sampler2D ourTexture1;\n"
	"uniform float scale;\n"
	"void main()\n" 
	"{\n" 
	//"    FragColor = texture(ourTexture1, ourTexCoord);\n"
	"    vec2 UpDownCoord = vec2((ourTexCoord.x-0.5)*scale+0.5, ((1.0 - ourTexCoord.y - 0.5)*scale+0.5));\n"
	"    FragColor = texture(ourTexture1, UpDownCoord);\n" 
	"}";





int main()
{

	GLFWwindow* window;  

	/* Initialize the library */  
	if (!glfwInit())  
	return -1;  

	/* Create a windowed mode window and its OpenGL context */  
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);  
	if (!window)  
	{  
		glfwTerminate();  
		return -1;  
	}  

	/* Make the window's context current */  
	glfwMakeContextCurrent(window);  

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	    printf("Failed to initialize GLAD\r\n");
	    return -1;
	}



	Shader object1(vertexShaderSource, fragmentShaderSource);


	float vertices[] = {													//一张纹理图像对应0-1的坐标范围，超过的会扩展
	// //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		0.8f,  0.8f, 0.0f,   1.0f, 0.0f, 0.0f,   1.5f, 1.5f,   // 右上
		0.8f, -0.8f, 0.0f,   0.0f, 1.0f, 0.0f,   1.5f, -0.5f,   // 右下
		-0.8f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,   -0.5f, -0.5f,   // 左下
		-0.8f,  0.8f, 0.0f,   1.0f, 1.0f, 0.0f,   -0.5f, 1.5f    // 左上
	};
	unsigned int indices[] = { // 注意索引从0开始! 
	    0, 1, 2, // 第一个三角形
	    2, 3, 0  // 第二个三角形
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	// 纹理属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
	glEnableVertexAttribArray(2);



	// stbi_set_flip_vertically_on_load(true);	//输入图上下颠倒

	int width, height, nrChannels;
	unsigned char *data = stbi_load("test/test.jpg", &width, &height, &nrChannels, 0);
	printf("width is %d, height is %d,nrChannels is %d\r\n",width,height,nrChannels);

	object1.use();								//设置uniform必须在当前program上

	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);				//激活纹理单元0(默认))
	glBindTexture(GL_TEXTURE_2D, texture);		//绑定对象与当前激活纹理单元0，后续操作才能执行到该对象上
	object1.setInt(1,"ourTexture1", 0);			//设置uniform sampler2D ourTexture1为纹理单元0
												// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	// glBindTexture(GL_TEXTURE_2D, 0);			//如VAO，init的要绑和解绑，process也要绑，否则就init不解绑，保证process时要绑
	stbi_image_free(data);						//如果解绑，当前激活也会被关闭
												//但此处的GL_TEXTURE_2D分GL_TEXTURE0/1/2/3/4/5...，实则不是同一个对象



	float scale = 1.0f;
	/* Loop until the user closes the window */  
	while (!glfwWindowShouldClose(window))  
	{  

		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//按键判断
			glfwSetWindowShouldClose(window, true);
		else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			scale = scale + 0.05;
		else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			scale = scale - 0.05;


		/* Render here */  

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        float rotate = (float)glfwGetTime()*10;
		while(rotate >= 360)
			rotate -= 360;
		float offset = sin(glm::radians(rotate));
		

               
		object1.use();
		object1.setFloat(1,"scale", scale);

        //glm::mat4 trans;
        glm::mat4 trans = glm::mat4(1.0f);                                              //单位矩阵
        trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));                     //转换成位移矩阵
        trans = glm::rotate(trans, glm::radians(rotate), glm::vec3(1.0, 0.0, 1.0));      //旋转矩阵
        trans = glm::scale(trans, glm::vec3(0.75, 1.0, 1.0));                         //缩放矩阵
        object1.setMatrix(4, "transform", glm::value_ptr(trans));   
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0);

        glm::mat4 trans2 = glm::mat4(1.0f);                                              //单位矩阵
        trans2 = glm::translate(trans2, glm::vec3(1.0f, 1.0f, 0.0f));                     //转换成位移矩阵
        trans2 = glm::scale(trans2, glm::vec3(offset, offset, 1.0));                         //缩放矩阵
        object1.setMatrix(4, "transform", glm::value_ptr(trans2));      
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/* Swap front and back buffers */  
		glfwSwapBuffers(window);  

		/* Poll for and process events */  
		glfwPollEvents();  
	}  

	glfwTerminate();  

	return 0;

}
