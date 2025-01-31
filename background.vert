#version 330 core

layout (location = 0) in vec3 aPos;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aPos.xy * 0.5 + 0.5; // Prebacuje iz [-1, 1] u [0, 1]
}
