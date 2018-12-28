#version 330 core

layout (location = 0) in vec3 aPos;

out vec2 TexCoord;

const float pi=3.1415926535897932384626433832795;

void main()
{
    vec2 l;
    vec3 u;

    l.x = aPos.x * 0.5 * pi;
    l.y = aPos.y * 0.5 * pi;

    float d = sqrt(1.0 + cos(l.y) * cos(l.x));
    u.x = cos(l.y) * sin(l.x)/d;
    u.y = sin(l.y)/d;
    u.z = 0.0;
    
    gl_Position = vec4(u, 1.0);
    TexCoord = vec2( (aPos.x + 1.0) /2.0, ( 1.0 - aPos.y) /2.0);
}

