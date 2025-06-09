
#version 330 core
layout(location = 0) in vec2 aPosition; // ex: [-1,-1], [1,-1], [1,1], [-1,1]

out vec2 fragCoord;

void main()
{
    fragCoord = aPosition;
    gl_Position = vec4(aPosition, 1.0, 1.0);
}