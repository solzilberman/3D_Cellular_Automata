#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class Shader {
 public:
  int id;

  Shader () {}

  Shader (const char* vertexPath, const char* fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
      // open files
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      std::stringstream vShaderStream, fShaderStream;
      // read file's buffer contents into streams
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      // close file handlers
      vShaderFile.close();
      fShaderFile.close();
      // convert stream into string
      vertexCode   = vShaderStream.str();
      fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
      std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  // activate the shader
  void use () { 
    glUseProgram(id); 
  }

  void unuse () {
    glUseProgram(0);
  }
  
  void free () { glDeleteProgram(id); }
  
  // utility uniform functions
  void setBool (const std::string &name, bool value) const {         
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); 
  }
  
  void setUInt (const std::string &name, unsigned int value) const { 
        glUniform1ui(glGetUniformLocation(id, name.c_str()), value); 
    }


  void setInt (const std::string &name, int value) const { 
        glUniform1i(glGetUniformLocation(id, name.c_str()), value); 
    }

  void setFloat (const std::string &name, float value) const { 
    glUniform1f(glGetUniformLocation(id, name.c_str()), value); 
  }

  void setVec2 (const std::string &name, float x, float y) const { 
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y); 
  }

  void setVec3 (const std::string &name, float x, float y, float z) const { 
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z); 
  }

  void setMat4 (const std::string &name, float *m) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_TRUE, m);
  }
private:
  // utility function for checking shader compilation/linking errors.
  void checkCompileErrors (unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
	glGetShaderInfoLog(shader, 1024, NULL, infoLog);
	std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
		  << std::endl << infoLog << std::endl;
      }
    }
    else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
	glGetProgramInfoLog(shader, 1024, NULL, infoLog);
	std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type
		  << std::endl << infoLog << std::endl;
      }
    }
  }
};

#endif
