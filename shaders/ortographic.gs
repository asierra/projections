#version 330 core
#extension GL_EXT_geometry_shader4: enable

layout(triangles) in;  
layout(triangle_strip, max_vertices=9) out;

in vec2 vTexCoord[3];
out vec2 TexCoord;

vec4 in_v[3];

bool isEast(in vec4 x)
{
  return (x.x >= 0.0);
}

void ProduceVertex(int i)
{
  TexCoord = vTexCoord[i];
  gl_Position = in_v[i];
  EmitVertex();
}

void main()
{
  int in_west = 0;
  int in_east = 0;
  
  in_v[0] = gl_PositionIn[0];
  in_v[1] = gl_PositionIn[1];
  in_v[2] = gl_PositionIn[2];

  for (int i=0; i < 3; i++) {
    if (isEast(in_v[i]))
      in_east++;
    else
      in_west++;
  }
 
  if (in_east > 0 && in_west > 0) {
    for (int i=0; i < 3; i++) {
      if (!isEast(in_v[i]))
	in_v[i].x = -in_v[i].x;
    }
  }
  
  ProduceVertex(0);
  ProduceVertex(1);
  ProduceVertex(2);
}
