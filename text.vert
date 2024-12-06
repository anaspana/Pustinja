#version 330 core

layout(location = 0) in vec4 vertex; // Position and texture coordinates
out vec2 TexCoords;

void main()
{
    gl_Position = vec4(vertex.x, vertex.y, 0.0f, 1.0f); // Use screen-space coordinates directly
    TexCoords = vec2(vertex.z, vertex.w); // Texture coordinates
}
