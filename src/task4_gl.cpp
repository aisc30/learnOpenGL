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
#include <CameraClass.h>

using namespace std;


const char* vertexShaderSource = "\n" 
	//"#version 330 core\n" 
	"#version 300 es\n" 
	"layout (location = 0) in vec3 aPos;\n" 
	"layout (location = 1) in vec2 aTexCoord;\n" 
    "uniform mat4 model;\n" 
    "uniform mat4 view;\n" 
    "uniform mat4 projection;\n" 
	"uniform float scale;\n"
	"out vec2 ourTexCoord;\n" 
	"void main()\n" 
	"{\n" 
	"    gl_Position = projection * view * model * vec4(aPos.x*scale, aPos.y*scale, aPos.z*scale, 1.0);\n" 
	"    ourTexCoord = aTexCoord;\n" 
	"}";

const char* fragmentShaderSource = "\n" 
	//"#version 330 core\n" 
	"#version 300 es\n" 
	"precision mediump float;\n"
	"out vec4 FragColor;\n" 
	"in vec2 ourTexCoord;\n" 
	"uniform sampler2D ourTexture1;\n"
	"void main()\n" 
	"{\n" 
	"    vec2 UpDownCoord = vec2((ourTexCoord.x-0.5)+0.5, ((1.0 - ourTexCoord.y - 0.5)+0.5));\n"
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
	// Camera camera(window, 640, 480);
	camera.Init(window, 640, 480);

	// float vertices[] = {													//一张纹理图像对应0-1的坐标范围，超过的会扩展
	// // //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
	// 	0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上0
	// 	0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下1
	// 	-0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下2
	// 	-0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // 左上3

	// 	0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   -1.0f, -1.0f,   // 右上4
	// 	0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   -1.0f, 0.0f,   // 右下5
	// 	-0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下6
	// 	-0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, -1.0f    // 左上7
	// };
	// unsigned int indices[] = { // 注意索引从0开始! 
	//     0, 1, 2, // 第一个矩形
	//     2, 3, 0,  
	//     4, 5, 1, // 第二个矩形
	//     1, 0, 4,  
	//     5, 6, 2, // 第三个矩形
	//     2, 1, 5,  
	//     6, 7, 3, // 第四个矩形
	//     3, 2, 6,  
	//     7, 4, 0, // 第五个矩形
	//     0, 3, 7,  
	//     4, 5, 6, // 第六个矩形
	//     6, 7, 4        
	// };

        float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };



	// unsigned int EBO;
	// glGenBuffers(1, &EBO);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 纹理属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);



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

    glm::vec3 cubePositions[] = {
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
    };


    glEnable(GL_DEPTH_TEST);    //开启深度测试
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear depth buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);                       //clear window color buffer

               
		object1.use();
		object1.setFloat(1,"scale", scale);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        
		camera.update();
		glm::mat4 view = camera.view;		
        glm::mat4 projection = camera.projection;
		


        object1.setMatrix(4, "model", glm::value_ptr(model)); 
        object1.setMatrix(4, "view", glm::value_ptr(view)); 
        object1.setMatrix(4, "projection", glm::value_ptr(projection));   
		// glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// glBindVertexArray(0);


		for(unsigned int i = 0; i < 9; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i; 
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			object1.setMatrix(4, "model", glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		/* Swap front and back buffers */  
		glfwSwapBuffers(window);  

		/* Poll for and process events */  
		glfwPollEvents();  
	}  

	glfwTerminate();  

	return 0;

}
