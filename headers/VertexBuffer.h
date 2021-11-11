#ifndef VERTEXARRAY
#define VERTEXARRAY
#include <GL/glew.h>

class VertexBuffer {
  unsigned int vbo, vao;
public:
  float* vertices;
  VertexBuffer (unsigned int nv, float *vs) {
    unsigned long stride = 6u*sizeof(float);
    vertices = vs;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, stride*nv, vs, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*) (3*sizeof(float)));
    glEnableVertexAttribArray(1);
    

  }

  void free () {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
  }
  
  void use () {
    glBindVertexArray(vao);
  }
};

class VertexBufferIndex : public VertexBuffer {
  unsigned int ebo;
public:
  VertexBufferIndex (unsigned int nv, float *vs, unsigned int ni, unsigned int *is)
    : VertexBuffer(nv, vs) {
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ni*sizeof(unsigned int), is, GL_STATIC_DRAW); 
  }

  void free () {
    glDeleteBuffers(1, &ebo);
    VertexBuffer::free();
  }
};

#endif
