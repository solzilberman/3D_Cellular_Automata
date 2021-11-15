#version 330 core
in vec4 color;
in vec3 position;
uniform bool is_wire;
void main() 
{
    float x = position.x;
    float y = position.y;
    float z = position.z;
    // float r = sqrt((x-10.5)*(x-10.5) + (y-10.5)*(y-10.5) + (z-10.5)*(z-10.5));
    float red = position.x/20.0;
    float green = position.y/20.0;
    float blue = position.z/20.0;
    float range2 = 0.5f;
    red = (red*range2);
    green = (green*range2);
    blue = (blue*range2);
    // r /= 25.0f;
    if (is_wire){
        red = 0.0f;
        green = 0.0f;
        blue = 0.0f;
        // r = 0.0f;
    }
    gl_FragColor = vec4(red,blue,green,color.w); // white color
}