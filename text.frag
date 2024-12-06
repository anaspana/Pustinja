#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text; // The character texture
uniform vec3 textColor; // The text color

void main()
{
    vec4 texColor = texture(text, TexCoords);
    if (texColor.a < 0.1)
        discard; // Discard transparent pixels
    FragColor = vec4(textColor, 1.0f) * texColor; // Apply color to text
}
