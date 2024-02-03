#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;
layout (location = 2) in vec3 Normal;

uniform mat4 Transform;
uniform mat4 View;
uniform mat4 Projection;

uniform vec4 CameraPosition;
uniform float Time;
uniform float WaveSize;

out vec3 FragNormal;
out vec3 FragPos;
out vec3 CameraPos;

float wave_2d(float x, float time, float height, float speed, float width) {
    float angle = 3.14 * (x + time * speed) / width;
	return sin(angle) * height;
}

vec2 wave_2d_normal(float x, float time, float height, float speed, float width) {
    float angle = 3.14 * (x + time * speed) / width;
    float side = -cos(angle) / width * height * 5; // * 10 to make it more visible
	return normalize(vec2(side, 1.0));

}

float wave(float x, float z, vec2 direction, float time, float height, float speed, float width) {
	vec2 dir = normalize(direction);
	return wave_2d(x * dir.x + z * dir.y, time, height, speed, width);
}

vec3 wave_normal(float x, float z, vec2 direction, float time, float height, float speed, float width) {
	vec2 dir = normalize(direction);
	vec2 normal2d = wave_2d_normal(x * dir.x + z * dir.y, time, height, speed, width);
	return vec3(normal2d.x * dir.x, normal2d.y, normal2d.x * dir.y);
}

void main(){
    vec4 t_pos = Transform * vec4(Position, 1);
    vec3 vertex = vec3(t_pos) / t_pos.w;
	vertex.y += wave(vertex.x, vertex.z, vec2(1.0, -1.0), Time, 0.5 * WaveSize, 0.5, 150.0);
	vertex.y += wave(vertex.x, vertex.z, vec2(1.0, 1.0), Time, 0.5 * WaveSize, 1.0, 50.0);
	vertex.y += wave(vertex.x, vertex.z, vec2(-1.0, 0.5), Time, 0.3 * WaveSize, 3.0, 25.0);
	vertex.y += wave(vertex.x, vertex.z, vec2(0.3, 1.0), Time, 0.25 * WaveSize, 5.0, 10.0);
	vertex.y += wave(vertex.x, vertex.z, vec2(-1.0, -1.0), Time, 0.2 * WaveSize, 10.0, 5.0);

	vec3 normals = vec3(0.0, 0.0, 0.0);
	normals += wave_normal(vertex.x, vertex.z, vec2(1.0, -1.0), Time, 0.5 * WaveSize, 0.5, 150.0) * 0.5;
	normals += wave_normal(vertex.x, vertex.z, vec2(1.0, 1.0), Time, 0.5 * WaveSize, 1.0, 50.0) * 0.5;
	normals += wave_normal(vertex.x, vertex.z, vec2(-1.0, 0.5), Time, 0.3 * WaveSize, 3.0, 25.0) * 0.3;
	normals += wave_normal(vertex.x, vertex.z, vec2(0.3, 1.0), Time, 0.25 * WaveSize, 5.0, 10.0) * 0.25;
	normals += wave_normal(vertex.x, vertex.z, vec2(-1.0, -1.0), Time, 0.2 * WaveSize, 10.0, 5.0) * 0.2;

    gl_Position = Projection * View * vec4(vertex, 1.0);
    FragNormal = normalize(normals);
    CameraPos = vec3(CameraPosition);
    vec4 fp = vec4(vertex, 1.0);
    FragPos = vec3(fp) / fp.w;

}