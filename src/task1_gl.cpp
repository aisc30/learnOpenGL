#include <stdio.h>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>
#include <math.h>
#include "ShaderClass.h"


using namespace std;


const char* vertexShaderSource = "\n" 
	//"#version 330 core\n" 
	"#version 300 es\n" 
	"layout (location = 0) in vec3 aPos;\n" 
	"layout (location = 1) in vec3 aColor;\n" 
	"out vec3 ourColor;\n" 
	"uniform float offset;\n"
	"void main()\n" 
	"{\n" 
	"    gl_Position = vec4(aPos.x+offset, aPos.y, aPos.z, 1.0);\n" 
	"    ourColor = aColor;\n" 
	"}";

const char* fragmentShaderSource = "\n" 
	//"#version 330 core\n" 
	"#version 300 es\n" 
	"precision mediump float;\n"
	"out vec4 FragColor;\n" 
	"uniform vec4 ourColor;\n"
	"void main()\n" 
	"{\n" 
	"    FragColor = ourColor;\n" 
	"}";


const char* fragmentShaderSource2 = "\n" 
	//"#version 330 core\n" 
	"#version 300 es\n" 
	"precision mediump float;\n"
	"out vec4 FragColor;\n" 
	"in vec3 ourColor;\n" 
	"void main()\n" 
	"{\n" 
	"    FragColor = vec4(ourColor, 1.0f);\n" 
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
	Shader object2(vertexShaderSource, fragmentShaderSource2);



	float vertices[] = {
	    0.5f, 0.5f, 0.0f,   // 
	    0.5f, -0.5f, 0.0f,  // 
	    -0.5f, -0.5f, 0.0f, // 

	    -0.6f, 0.6f, 0.0f,
	    -0.6f, -0.6f, 0.0f,	
		0.0f, 0.0f, 0.0f
	};

	unsigned int indices[] = { // 注意索引从0开始! 
	    0, 1, 2, // 第一个三角形
	    3, 4, 5  // 第二个三角形
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO;
	glBindVertexArray(VAO);								//VAO对象用来包住VBO和EBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glBindVertexArray(0);									
	//glBindBuffer(GL_ARRAY_BUFFER, 0);  					//隐性地需要解绑，或者后面有绑定别处
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

															//绑和解绑：如果全程只有一个对象，就不用解绑
															//如果要切换，每次绑就是一次切换，这里隐藏了一个解绑，可做可不做


	float vertices2[] = {
	    // 位置              // 颜色
	     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下	//rgb	//<0=0
	    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
	     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};

	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);
	unsigned int VBO2;
	glBindVertexArray(VAO2);
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);


	/* Loop until the user closes the window */  
	while (!glfwWindowShouldClose(window))  
	{  
		/* Render here */  

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		float offset = 0.7f*sin(timeValue);

		object2.use();
		object2.setFloat(1, "offset", offset);							//设置uniform必须在当前program上

		glBindVertexArray(VAO2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);						//片元着色器为GL_FILL
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);											//隐性地需要解绑，或者后面有绑定别处



		object1.use();
		object1.setFloat(4, "ourColor", 0.0f, greenValue, 0.0f, 1.0f);	//设置uniform必须在当前program上

		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);						//片元着色器为GL_LINE
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		/* Swap front and back buffers */  
		glfwSwapBuffers(window);  

		/* Poll for and process events */  
		glfwPollEvents();  
	}  

	glfwTerminate();  

	return 0;

}
