#version 330 core
in vec4 color;
in vec3 position;
uniform bool is_wire;
void main() 
{
    float x = position.x, y = position.y, z = position.z;
    float r = sqrt((x-10.5)*(x-10.5) + (y-10.5)*(y-10.5) + (z-10.5)*(z-10.5));
    r /= 25.0f;
    if (is_wire) r = 0.0f;
    gl_FragColor = vec4(r,r,r,color.w); // white color
}