#version 330 core
layout (location = 0) in vec4 vertex; 
// <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec3 playerPosition;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
    playerPosition=vec3(model*vec4(vertex.xy,0.0,1.0));
}