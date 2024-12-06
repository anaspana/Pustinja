#version 330 core
layout(location = 0) in vec3 aPos; // Vertex positions

// Uniform za poziciju ribice
uniform float fishX; // Horizontalna pozicija ribice

void main()
{
    // Pomakni poziciju ribice na osnovu fishX
    gl_Position = vec4(aPos.x + fishX, aPos.y, aPos.z, 1.0);
}
