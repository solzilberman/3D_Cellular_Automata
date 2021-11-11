#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
uniform float time;
out vec3 color;


void main()
{   
    color = col;
    gl_Position = vec4(pos, 1);
}
