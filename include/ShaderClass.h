#include <stdio.h>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>
#include <math.h>

class Shader
{
public:
    // 程序ID
    unsigned int ProgramID;

    // 构造器读取并构建着色器
    Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
    // 使用/激活程序
    void use();
    // uniform工具函数
    void setInt(int element_num, const std::string &name, int value, int value2, int value3, int value4);   
    void setFloat(int element_num, const std::string &name, float value, float value2, float value3, float value4);  
	void setMatrix(int element_num, const std::string &name, float *ptr_tran);
};

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
{

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
	    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
	    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertexShader);
	glAttachShader(ProgramID, fragmentShader);
	glLinkProgram(ProgramID);

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &success);
	if(!success) {
	    glGetProgramInfoLog(ProgramID, 512, NULL, infoLog);
	    std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}


void Shader::use() 
{ 
    glUseProgram(ProgramID);
}

void Shader::setInt(int element_num, const std::string &name, int value, int value2 = 0, int value3 = 0, int value4 = 0) 
{ 
    if(element_num == 2)
    	glUniform2i(glGetUniformLocation(ProgramID, name.c_str()), value, value2); 
    else if(element_num == 3)
    	glUniform3i(glGetUniformLocation(ProgramID, name.c_str()), value, value2, value3); 
    else if(element_num == 4)
    	glUniform4i(glGetUniformLocation(ProgramID, name.c_str()), value, value2, value3, value4);
    else 
    	glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), value); 
}
void Shader::setFloat(int element_num, const std::string &name, float value, float value2 = 0, float value3 = 0, float value4 = 0) 
{ 
    if(element_num == 2)
    	glUniform2f(glGetUniformLocation(ProgramID, name.c_str()), value, value2); 
    else if(element_num == 3)
    	glUniform3f(glGetUniformLocation(ProgramID, name.c_str()), value, value2, value3); 
    else if(element_num == 4)
    	glUniform4f(glGetUniformLocation(ProgramID, name.c_str()), value, value2, value3, value4);
    else 
    	glUniform1f(glGetUniformLocation(ProgramID, name.c_str()), value); 
} 
void Shader::setMatrix(int element_num, const std::string &name, float *ptr_tran) 
{ 
    if(element_num == 2)
		glUniformMatrix2fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, ptr_tran);    
    else if(element_num == 3)
		glUniformMatrix3fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, ptr_tran);    
    else if(element_num == 4)
		glUniformMatrix4fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, ptr_tran);           
    //else 
	//	glUniformMatrix1fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, ptr_tran);    
} 