#version 330 core

layout (location = 0) in vec3 aPos;

out vec2 vTexCoord;

const float pi=3.1415926535897932384626433832795;

void main()
{
    vec2 l;  // Longitude Latitude
    vec3 u;  // Projected position
    
    float centerx = 0.5;
    float scalex = 0.5;
    
    l.x = aPos.x * pi;
    l.y = aPos.y * 0.5 * pi;

    if (aPos.x < 0)    
       centerx = -centerx;
    else 	
       scalex = -scalex;
    
    u.x = centerx + scalex * cos(l.x) * cos(l.y);
    u.z = 0.5 * sin(l.x) * cos(l.y);
    u.y = sin(l.y);
    
    gl_Position = vec4(u, 1.0);
    vTexCoord = vec2( (aPos.x + 1.0) /2.0, ( 1.0 - aPos.y) /2.0);
}

