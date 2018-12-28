#version 330 core

layout (location = 0) in vec3 aPos;

out vec2 TexCoord;

const float pi=3.1415926535897932384626433832795;

void main()
{
    vec2 l;
    vec3 u;

    l.x = aPos.x;
    l.y = aPos.y * 0.5 * pi;

    float scale = 0.26786774137962738;
    
    u.x = l.x;
    u.y = scale * log(tan(pi * 0.25 + l.y * 0.5));
    u.z = 0.0;
    
    gl_Position = vec4(u, 1.0);
    TexCoord = vec2( (aPos.x + 1.0) /2.0, ( 1.0 - aPos.y) /2.0);
}
