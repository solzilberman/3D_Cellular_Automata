#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
<<<<<<< HEAD
uniform float time;
=======
uniform mat4 pvm;
>>>>>>> parent of 53ab33f... basic 3dCA working
out vec3 color;


void main()
{   
    color = col;
    gl_Position = vec4(pos, 1);
}
