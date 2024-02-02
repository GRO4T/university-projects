#version 330 core

in vec3 Position;
in vec2 TexCoords;
in vec3 Normal;

out vec2 PassTexCoords;
out vec3 PassNormal;
out vec3 PassFragPos;
out vec3 PassCameraPosition;

uniform mat4 Transform;
uniform mat4 Projection;
uniform mat4 View;

uniform vec3 CameraPosition;


void main(void){
    gl_Position = Projection * View * Transform * vec4(Position, 1.0);
    PassTexCoords = TexCoords * 40.0;
    PassFragPos = vec3(Transform * vec4(Position, 1.0));
    PassCameraPosition = CameraPosition;
    PassNormal = Normal;
}