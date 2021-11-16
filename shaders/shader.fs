#version 330 core
in vec3 position;
in vec3 nw;
in float col;
in vec3 pw;
uniform bool is_wire;
uniform float side_length;
uniform vec3 eye;
vec3 lighting()
{
    float x = position.x;
    float y = position.y;
    float z = position.z;
    // float r = sqrt((x-10.5)*(x-10.5) + (y-10.5)*(y-10.5) + (z-10.5)*(z-10.5));
    float red = position.x/side_length;
    float green = position.y/side_length;
    float blue = position.z/side_length;
    float range2 = 0.5f;
    red = (red*range2);
    green = (green*range2);
    blue = (blue*range2);
    // r /= 25.0f;
    if (is_wire){
        red = 1.0f;
        green = 1.0f;
        blue = 1.0f;
        // r = 0.0f;
    }
    vec3 amb = vec3(red,green,blue);
    vec3 diff = vec3(red,green,blue);
    vec3 spec = vec3(red,green,blue);
    vec3 lpos = vec3(15, 15, 15);
    float kl = 0;
    float kq = .01;
    //d s ns
    vec3 md = vec3(red,green,blue);
    vec3 ms = vec3(red,green,blue);
    float ns = 50;
    vec3 n = normalize(nw);
    vec3 v = normalize(eye - pw);
    vec3 c = vec3(0.0f);
    c += amb*md;
    vec3 ll = lpos - pw;
    if (dot(ll, n) > 0.0f) {
        float d = length(ll), k = 1.0f/(1.0f + kl*d + kq*d*d);
        vec3 kv = vec3(k, k, k), l = vec3(ll.x/d, ll.y/d, ll.z/d);
        c += diff*md*dot(n, l)*kv;
    
        vec3 h = normalize(l + v);
        float nh = dot(n, h);
        c += spec*ms*pow(nh, ns)*kv;
      
    }
    return vec3(c);
}

void main() 
{
    
    float x = position.x;
    float y = position.y;
    float z = position.z;
    // float r = sqrt((x-10.5)*(x-10.5) + (y-10.5)*(y-10.5) + (z-10.5)*(z-10.5));
    float red = position.x/side_length;
    float green = position.y/side_length;
    float blue = position.z/side_length;
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
    vec3 c = lighting();
    gl_FragColor = vec4(c.x,c.y,c.z,col); // white color
}