#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;

uniform mat4 Transform;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 CameraPosition;
uniform mat4 LightSpace;

out vec2 PassTexCoords;
out vec3 PassNormal;
out vec3 PassFragPos;
out vec3 PassCameraPosition;
out vec4 PassLightSpacePosition;

void main()
{
    PassTexCoords = TexCoords;
    PassFragPos = vec3(Transform * vec4(Position, 1.0));
    PassCameraPosition = CameraPosition;
    PassLightSpacePosition = LightSpace * Transform * vec4(Position, 1.0);
    
    mat3 transform_rotation = mat3(Transform);
    PassNormal = mat3(transpose(inverse(Transform))) * Normal;
    gl_Position = Projection * View * Transform * vec4(Position, 1.0);
}