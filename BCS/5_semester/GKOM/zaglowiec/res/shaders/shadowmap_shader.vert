#version 330 core
layout (location = 0) in vec3 Position;

uniform mat4 LightSpace;
uniform mat4 Transform;

void main()
{
    gl_Position = LightSpace * Transform * vec4(Position, 1.0);
}  