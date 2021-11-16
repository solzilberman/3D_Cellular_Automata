#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 norm;
uniform mat4 pvm;
uniform mat4 model;
out vec3 nw;
out vec3 pw;
out vec3 position;
out float col;


void main()
{   
    vec4 nw4 = model * vec4(norm.xyz,0.0f);
    vec4 pw4 = model * vec4(pos, 1.0f);
    pw = pw4.xyz;
    nw = nw4.xyz;
    col = norm.w;
    position = pos;
    gl_Position = pvm * vec4(pos, 1.0);
}
