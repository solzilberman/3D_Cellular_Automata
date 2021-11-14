#version 330 core
in vec4 color;
in vec3 position;
uniform bool is_wire;
void main() 
{
    float x = position.x, y = position.y, z = position.z;
    float r = sqrt((x-4.5)*(x-4.5) + (y-4.5)*(y-4.5) + (z-4.5)*(z-4.5));
    r /= 15.0f;
    if (is_wire) r = 0.0f;
    gl_FragColor = vec4(r,r,r,color.w); // white color
}