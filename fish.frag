#version 330 core

out vec4 FragColor;
uniform vec4 fishColor; // Boja ribice sa alfa kanalom

void main()
{
    FragColor = fishColor; // Alfa kanal kontroliše providnost
}
