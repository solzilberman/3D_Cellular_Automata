#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 col;
uniform mat4 pvm;
out vec4 color;
out vec3 position;


void main()
{   
    color = col;
    position = pos;
    gl_Position = pvm * vec4(pos, 1.0);
}
