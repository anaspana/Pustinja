#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform float timeOfDay; // Vreme dana (od 0.0 do 1.0)

void main()
{
    float transition = 0.6; // Granica za prelaz boja
    vec3 dayColor = vec3(0.5, 0.8, 1.0); // Plavo nebo
    vec3 nightColor = vec3(0.0, 0.0, 0.2); // Tamno plavo nebo

    // Interpolacija boje na osnovu vremena dana
    vec3 backgroundColor = mix(dayColor, nightColor, smoothstep(0.5, 1.0, timeOfDay));

    if (TexCoord.y > transition) {
        FragColor = vec4(backgroundColor, 1.0);
    } else {
        FragColor = vec4(0.8, 0.7, 0.5, 1.0); // Pesak
    }

}