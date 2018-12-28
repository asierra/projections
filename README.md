#  World Projections

World projections using OpenGL vertex and geometric shaders; an
interactive and educative application on geography and a practical
example of vertex shaders.

Compile it with 'make' and run in its own directory.  It compiles in
Linux and Mac OSX and should compile in any platform that supports
OpenGL and shaders with minimal adjustments.

To move to the next projection, use the vertical arrows of your
keyboard. To toggle between wired and filled rendering, use 'w'. To
exit use the escape key.

To create a new projection, just create a new vertex shader and, if
necessary, a geometric shader (if the map is torn apart like the
orthographic projection). Save them in the shaders directory.

All code is original, except stb_image.h, shaders.h and
glad.c. shaders.h was taken from https://learnopengl.com/ and perhaps
you should generate your own glad.c for your graphics
hardware. earth_2k.jpg was generated from
http://www.shadedrelief.com/natural/ .

Please send me any comment, improvement and shaders.

License: GPL 3.0

Copyright (c) 2018 Alejandro Aguilar Sierra (asierra@unam.mx)
