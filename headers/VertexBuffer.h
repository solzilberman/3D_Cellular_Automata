/*
 @author - Sol Zilberman
 */
#ifndef VERTEXARRAY
#define VERTEXARRAY
#include <GL/glew.h>

class VertexBufferInstanced {

  public:
    unsigned int ibo, iao;
    float *vertices;
    VertexBufferInstanced(unsigned int nc) {

        unsigned long tstride = sizeof(glm::vec3);
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ARRAY_BUFFER, ibo);
        glBufferData(GL_ARRAY_BUFFER, tstride*nc*1000, nullptr,
                     GL_DYNAMIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, tstride, (void *) 0);
        glVertexAttribDivisor(2, 1);
        glEnableVertexAttribArray(2);

    }

    void free() {
        glDeleteBuffers(1, &ibo);
        glDeleteVertexArrays(1, &iao);
    }

    void use() { glBindBuffer(GL_ARRAY_BUFFER,ibo); }
};
class VertexBuffer {
    unsigned int vbo, vao;

  public:
    float *vertices;
    VertexBuffer(unsigned int nv, float *vs) {
        unsigned long stride = 7u * sizeof(float);
        vertices = vs;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, stride * nv, vs, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride,
                              (void *)(3u * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void free() {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    void use() { glBindVertexArray(vao); }
};

class VertexBufferIndex : public VertexBuffer {
    unsigned int ebo;

  public:
    VertexBufferIndex(unsigned int nv, float *vs, unsigned int ni,
                      unsigned int *is)
        : VertexBuffer(nv, vs) {
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ni * sizeof(unsigned int), is,
                     GL_STATIC_DRAW);
    }

    void free() {
        glDeleteBuffers(1, &ebo);
        VertexBuffer::free();
    }
};

#endif
