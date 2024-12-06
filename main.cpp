// Autor: Ana Spanovic

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>
#include <cmath>
#include <map>
#include <chrono>
#include <glm/glm.hpp>

#include <GL/glew.h>   //Omogucava laksu upotrebu OpenGL naredbi
#include <GLFW/glfw3.h>//Olaksava pravljenje i otvaranje prozora (konteksta) sa OpenGL sadrzajem
//#include <ft2build.h>
//#include FT_FREETYPE_H

//// Store the texture for each character
//std::map<char, GLuint> characterTextures;
//
//FT_Library ft;
//FT_Face face;
//
//GLuint VAO, VBO;
//
//void initFreeType() {
//    // Initialize FreeType
//    if (FT_Init_FreeType(&ft)) {
//        std::cerr << "Could not init FreeType Library" << std::endl;
//        return;
//    }
//
//    // Load a font face (adjust the path to your font file)
//    if (FT_New_Face(ft, "LiberationSans-Regular.ttf", 0, &face)) {
//        std::cerr << "Could not load font" << std::endl;
//        return;
//    }
//
//    // Set font size
//    FT_Set_Pixel_Sizes(face, 0, 48);
//
//    // Load characters and create textures
//    for (GLubyte c = 0; c < 128; ++c) {
//        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
//            std::cerr << "Failed to load Glyph" << std::endl;
//            continue;
//        }
//
//        // Generate texture for the character
//        GLuint texture;
//        glGenTextures(1, &texture);
//        glBindTexture(GL_TEXTURE_2D, texture);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
//
//        // Set texture parameters
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        // Store the texture in the map
//        characterTextures.insert(std::pair<char, GLuint>(c, texture));
//    }
//
//    // Set up VAO and VBO for rendering
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW); // Each character consists of 6 vertices
//    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//}
//
//void renderText(const std::string& text, float x, float y, float scale, const glm::vec3& color, unsigned int shaderProgramText) {
//    glUseProgram(shaderProgramText);  // Assuming you've created a shader for text rendering
//    glUniform3f(glGetUniformLocation(shaderProgramText, "textColor"), color.x, color.y, color.z);
//    glActiveTexture(GL_TEXTURE0);
//    glBindVertexArray(VAO);
//
//    for (char c : text) {
//        // Retrieve character texture
//        GLuint texture = characterTextures[c];
//
//        // Calculate character position and scale
//        float xpos = x;
//        float ypos = y;
//        float w = face->glyph->bitmap.width * scale;
//        float h = face->glyph->bitmap.rows * scale;
//
//        // Update the VBO for the character
//        float vertices[6][4] = {
//            { xpos,     ypos + h,   0.0f, 0.0f },
//            { xpos + w, ypos,       1.0f, 1.0f },
//            { xpos,     ypos,       0.0f, 1.0f },
//
//            { xpos,     ypos + h,   0.0f, 0.0f },
//            { xpos + w, ypos + h,   1.0f, 0.0f },
//            { xpos + w, ypos,       1.0f, 1.0f }
//        };
//        // Update the VBO with new character data
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
//        glBindTexture(GL_TEXTURE_2D, texture);
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//
//        // Advance the cursor for the next character
//        x += (face->glyph->advance.x >> 6) * scale;  // 1/64th of a pixel
//    }
//}


unsigned int compileShader(GLenum type, const char* source); //Uzima kod u fajlu na putanji "source", kompajlira ga i vraca sejder tipa "type"
unsigned int createShader(const char* vsSource, const char* fsSource); //Pravi objedinjeni sejder program koji se sastoji od Verteks sejdera ciji je kod na putanji vsSource i Fragment sejdera na putanji fsSource
void getMonitorResolution(int& width, int& height);
void drawBackground(unsigned int shaderProgramBackground);
void drawPyramids(unsigned int shaderProgramPyramids, const std::vector<float>& pyramidVertices);
void drawGrass(unsigned int shaderProgramGrass, const std::vector<float>& verticesGrass);
void drawOasis(unsigned int shaderProgramOasis, const std::vector<float>& verticesOasis);
void addOasisData(std::vector<float>& vertices, float x, float y, float radius, int segments);
void addGrassData(std::vector<float>& vertices, float x, float y, float innerRadius, float outerRadius, int segments);
void drawStars(unsigned int shaderProgram, const std::vector<float>& verticesStars);
void calculateSunAndMoonPosition(float& sunX, float& sunY, float& moonX, float& moonY, float timeOfDay);
void createCircle(std::vector<float>& lampVertices, float x, float y, float radius, int segments);
void drawCircle(unsigned int shaderProgram, const std::vector<float>& verticesCircle);
void addStarsData(std::vector<float>& vertices, int numStars, int starPoints, float innerRadius, float outerRadius);
void addFishData(std::vector<float>& vertices, float x, float y, float width, float height, float tailSize);
void drawFish(unsigned int shaderProgramFish, const std::vector<float>&verticesFish);

void addPyramidData(std::vector<float>& vertices, float x, float y, float size);
void addCircleData(std::vector<float>& vertices, float x, float y, float radius, int segments);

float timeOfDay = 0.0f; // Trenutno vreme dana, 0.0 - jutro, 1.0 - no?
bool isPaused = false;  // Zaustavljanje vremena
bool grassVisible = true;
float aspect = 1920.0f / 1080.0f;
static float fishX = -0.2f;  // Po?etna pozicija ribice
float fishSpeed = 0.002f;   // Brzina kretanja
float fishRotation = 0.0f;  // Po?etna rotacija ribice (0 stepeni)

int main(void)
{
    using namespace std::chrono;

    auto lastFrameTime = high_resolution_clock::now();  // Zapo?nite pra?enje vremena
    const float targetFPS = 60.0f;
    const float targetFrameTime = 1.0f / targetFPS; // 1/60 sekundi
    // Inicijalizacija GLFW
    if (!glfwInit())
    {
        std::cout << "GLFW nije mogao da se inicijalizuje!" << std::endl;
        return 1;
    }

    // Full-screen mod
    int screenWidth, screenHeight;
    getMonitorResolution(screenWidth, screenHeight);
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Pustinja", glfwGetPrimaryMonitor(), nullptr);
    if (!window)
    {
        std::cout << "Prozor nije mogao da se kreira!" << std::endl;
        glfwTerminate();
        return 2;
    }
    glfwMakeContextCurrent(window);
    glewInit();
    glfwSwapInterval(1); // Sinhronizacija za 60 FPS

    // Shaderi
    unsigned int shaderProgramBackground = createShader("background.vert", "background.frag");

    unsigned int shaderProgramPyramids = createShader("basic.vert", "basic.frag");

    unsigned int shaderProgramGrass = createShader("grass.vert", "grass.frag");

    unsigned int shaderProgramSun = createShader("circle.vert", "sun.frag");

    unsigned int shaderProgramMoon = createShader("circle.vert", "moon.frag");

    unsigned int shaderProgramOasis = createShader("oasis.vert", "oasis.frag");

    unsigned int shaderProgramStars = createShader("star.vert", "star.frag");

    unsigned int shaderProgramFish = createShader("fish.vert", "fish.frag");

    //unsigned int shaderProgramText = createShader("text.vert", "text.frag");


    // Podaci za piramide
    std::vector<float> pyramidVertices;
    addPyramidData(pyramidVertices, -0.8f, -0.7f, 0.4f); // Piramida 1
    addPyramidData(pyramidVertices, -0.3f, -0.7f, 0.3f);  // Piramida 2
    addPyramidData(pyramidVertices, -0.5f, -0.4f, 0.5f);   // Piramida 3

    // Dodavanje podataka o kružnom obliku oaze
    std::vector<float> oasisVertices;
    addCircleData(oasisVertices, 0.4f, -0.4f, 0.4f, 100);

    // Trava oko oaze
    std::vector<float> grassVertices;
    addGrassData(grassVertices, 0.4f, -0.4f, 0.4f, 0.5f, 100);  

    // Kreiraj ribicu
    std::vector<float> fishVertices;
    addFishData(fishVertices, 0.4f, -0.4f, 0.1f, 0.05f, 0.05f); // Pozicija, veli?ina elipse i repa

    // Postavke pozadine
    glClearColor(0.5, 0.8, 1.0, 1.0);

    //initFreeType(); // Initialize FreeType for loading fonts

    // Glavna petlja
    while (!glfwWindowShouldClose(window))
    {

        // Ciscenje ekrana
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Iscrtavanje pozadine
        glUniform1f(glGetUniformLocation(shaderProgramBackground, "timeOfDay"), timeOfDay);
        drawBackground(shaderProgramBackground);

        //Ime i prezime
        //renderText("Ana Spanovic SV48/2021", 10.0f, 10.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), shaderProgramText); // White text in the top-left corner


        // Prikazivanje piramida 
        drawPyramids(shaderProgramPyramids, pyramidVertices); 

        // Kretanje Sunca i Meseca
        float sunX = cos(timeOfDay * 2.0f * M_PI) * 0.8f;
        float sunY = sin(timeOfDay * 2.0f * M_PI) * 0.8f;
        float moonX = -cos(timeOfDay * 2.0f * M_PI) * 0.8f;
        float moonY = -sin(timeOfDay * 2.0f * M_PI) * 0.8f;

        calculateSunAndMoonPosition(sunX, sunY, moonX, moonY, timeOfDay);


        // FPS
        auto now = high_resolution_clock::now();
        duration<float> elapsed = duration_cast<duration<float>>(now - lastFrameTime);
        float elapsedTime = elapsed.count();

        // Ako je prošlo manje od ciljanog vremena, pauziraj
        if (elapsedTime < targetFrameTime) {
            auto sleepTime = targetFrameTime - elapsedTime; // Odredite koliko treba da spava
            std::this_thread::sleep_for(milliseconds(static_cast<int>(sleepTime * 1000))); // Pauza u milisekundama
        }

        // Ažuriraj vreme za slede?i frejm
        lastFrameTime = high_resolution_clock::now();


        // Sunce
        std::vector<float> sunVertices;
        createCircle(sunVertices, sunX, sunY, 0.1f, 100);  // Sunce u gornjem desnom kutu

        // Mesec
        std::vector<float> moonVertices;
        createCircle(moonVertices, moonX, moonY, 0.1f, 100);  // Mesec u gornjem levom kutu

        // Iscrtavanje Sunca i Meseca
        drawCircle(shaderProgramSun, sunVertices);
        drawCircle(shaderProgramMoon, moonVertices);

        //////////////////////////////////////////////////////////////////////////////

        drawOasis(shaderProgramOasis, oasisVertices); // Prikazivanje oaze

        // Dodajemo na po?etku glavne petlje u okviru render petlje:
        if (!isPaused) {
            timeOfDay += 0.0005f; // Pove?avanje vremena dana
            if (timeOfDay > 1.0f) timeOfDay = 0.0f; // Resetovanje na jutro
        }

        // Iscrtavanje trave (ako je vidljiva)
        if (grassVisible) {
            drawGrass(shaderProgramGrass, grassVertices);
        }

        if (timeOfDay > 1.0f) {
            timeOfDay = 0.0f;
        }

        // Zvezde
        std::vector<float> starVertices;
        addStarsData(starVertices, 15, 5, 0.02f, 0.05f);

        // Iscrtavnaje zvezda
        if (timeOfDay > 0.5f) {
            drawStars(shaderProgramStars, starVertices);
        }

        // Obrada doga?aja

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            grassVisible = false; // Sakrij travu
        }

        if (!grassVisible) {
            // Kretanje levo-desno
            fishX += fishSpeed;
            if (fishX > 0.2f || fishX < -0.2f) {
                fishSpeed = -fishSpeed; // Obrnuti pravac
            }
            // Ažuriraj uniform promenljivu
            glUseProgram(shaderProgramFish);
            glUniform1f(glGetUniformLocation(shaderProgramFish, "fishX"), fishX);
            glUniform4f(glGetUniformLocation(shaderProgramFish, "fishColor"), 1.0f, 0.5f, 0.2f, 0.5f);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // Dodaj ribicu sa pomerenom pozicijom
            drawFish(shaderProgramFish, fishVertices);
            glDisable(GL_BLEND);
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            grassVisible = true; // Prikaži travu
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            isPaused = true; // Zaustavi vreme
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            isPaused = false;  // Nastavi vreme
            timeOfDay = 0.0f;  // Vrati na jutro
        }

        // Swap buffers i polling doga?aja
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Ciscenje resursa
    glDeleteProgram(shaderProgramPyramids);
    glDeleteProgram(shaderProgramGrass);
    glDeleteProgram(shaderProgramSun);
    glDeleteProgram(shaderProgramMoon);
    glDeleteProgram(shaderProgramOasis);
    glDeleteProgram(shaderProgramFish);
    glDeleteProgram(shaderProgramStars);

    //// Clean up
    //FT_Done_Face(face);
    //FT_Done_FreeType(ft);

    glfwTerminate();
    return 0;

}

void createCircle(std::vector<float>& circleVertices, float centerX, float centerY, float radius, int segments) {

    // Add center vertex
    circleVertices.push_back(centerX);
    circleVertices.push_back(centerY);

    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments; // Calculate angle for this segment
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * aspect * sin(angle);

        circleVertices.push_back(x);
        circleVertices.push_back(y);
    }
}

void drawCircle(unsigned int shaderProgram, const std::vector<float>& verticesCircle) {

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesCircle.size() * sizeof(float), verticesCircle.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, verticesCircle.size() / 2);
    glBindVertexArray(0);
}

void drawPyramids(unsigned int shaderProgramPyramids, const std::vector<float>& pyramidVertices) {
    // VAO i VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, pyramidVertices.size() * sizeof(float), pyramidVertices.data(), GL_STATIC_DRAW);

    // Podesavanje atributa verteksa
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Iscrtavanje piramida
    glUseProgram(shaderProgramPyramids);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, pyramidVertices.size() / 3);
    glBindVertexArray(0);
}

void drawStars(unsigned int shaderProgram, const std::vector<float>& verticesStars) {
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesStars.size() * sizeof(float), verticesStars.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, verticesStars.size() / 3);
    glBindVertexArray(0);
}

void addStarsData(std::vector<float>& vertices, int numStars, int starPoints = 5, float innerRadius = 0.02f, float outerRadius = 0.05f) {
    for (int i = 0; i < numStars; i++) {
        float centerX = ((float)rand() / RAND_MAX) * 2.0f - 1.0f; // X koordinata centra
        float centerY = 0.2f + ((float)rand() / RAND_MAX) * 0.8f; // Y koordinata centra

        float angleStep = 2.0f * M_PI / (starPoints * 2);
        for (int j = 0; j < starPoints * 2; j++) {
            float radius = (j % 2 == 0) ? outerRadius : innerRadius; // Naizmeni?no ve?i i manji polupre?nik
            float angle = j * angleStep;

            float x = centerX + cos(angle) * radius;
            float y = centerY + sin(angle) * radius;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0f); // Z koordinata
        }
    }
}

void drawBackground(unsigned int shaderProgramBackground) {
    std::vector<float> backgroundVertices = {
            -1.0f, -1.0f, 0.0f, // Donji levi
             1.0f, -1.0f, 0.0f, // Donji desni
            -1.0f,  1.0f, 0.0f, // Gornji levi
             1.0f, -1.0f, 0.0f, // Donji desni
             1.0f,  1.0f, 0.0f, // Gornji desni
            -1.0f,  1.0f, 0.0f  // Gornji levi
    };

    unsigned int bgVAO, bgVBO;
    glGenVertexArrays(1, &bgVAO);
    glGenBuffers(1, &bgVBO);

    glBindVertexArray(bgVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
    glBufferData(GL_ARRAY_BUFFER, backgroundVertices.size() * sizeof(float), backgroundVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Iscrtavanje u glavnoj petlji
    glUseProgram(shaderProgramBackground);
    glUniform1f(glGetUniformLocation(shaderProgramBackground, "timeOfDay"), timeOfDay);

    glBindVertexArray(bgVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void addCircleData(std::vector<float>& vertices, float x, float y, float radius, int segments) {
    float angleStep = 2.0f * M_PI / float(segments);

    for (int i = 0; i < segments; ++i)
    {
        float angle1 = angleStep * i;
        float angle2 = angleStep * (i + 1);

        // Prvi vrh (centar kruga)
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);

        // Drugi vrh (kružni vrh 1)
        vertices.push_back(x + radius * cos(angle1));
        vertices.push_back(y + radius * sin(angle1));
        vertices.push_back(0.0f);

        // Tre?i vrh (kružni vrh 2)
        vertices.push_back(x + radius * cos(angle2));
        vertices.push_back(y + radius * sin(angle2));
        vertices.push_back(0.0f);
    }
}

void addFishData(std::vector<float>& vertices, float x, float y, float width, float height, float tailSize) {
    // Telo ribice
    int segments = 50;
    float angleStep = 2.0f * M_PI / segments;

    for (int i = 0; i < segments; ++i) {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;

        // Centar elipse
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);

        // Prvi vrh na elipsi
        vertices.push_back(x + cos(angle1) * width);
        vertices.push_back(y + sin(angle1) * height);
        vertices.push_back(0.0f);

        // Drugi vrh na elipsi
        vertices.push_back(x + cos(angle2) * width);
        vertices.push_back(y + sin(angle2) * height);
        vertices.push_back(0.0f);
    }

    // Trougao (rep)
    vertices.push_back(x - width);      // Leva ivica repa
    vertices.push_back(y);             // Sredina
    vertices.push_back(0.0f);

    vertices.push_back(x - width - tailSize); // Kraj repa
    vertices.push_back(y + height / 2.0f);
    vertices.push_back(0.0f);

    vertices.push_back(x - width - tailSize); // Kraj repa
    vertices.push_back(y - height / 2.0f);
    vertices.push_back(0.0f);
}

void drawFish(unsigned int shaderProgramFish, const std::vector<float>& verticesFish) {
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesFish.size() * sizeof(float), verticesFish.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(shaderProgramFish);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, verticesFish.size() / 3);
    glBindVertexArray(0);
}

void calculateSunAndMoonPosition(float& sunX, float& sunY, float& moonX, float& moonY, float timeOfDay) {
    float angle = timeOfDay * 2.0f * M_PI; // Ciklus od 0 do 2?
    sunX = cos(angle) * 1.7f;  // Polupre?nik kružnice
    sunY = sin(angle) * 1.1f - 0.3f;

    moonX = -cos(angle) * 1.7f;
    moonY = -sin(angle) * 1.1f - 0.3f;
}

void addOasisData(std::vector<float>& vertices, float x, float y, float radius, int segments) {
    addCircleData(vertices, x, y, radius, segments); // Oaza kao krug
}

void addGrassData(std::vector<float>& vertices, float x, float y, float innerRadius, float outerRadius, int segments) {
    // Trava je napravljena kao "donut" (krofna) oko oaze
    float angleStep = 2.0f * M_PI / float(segments);

    for (int i = 0; i < segments; ++i) {
        float angle1 = angleStep * i;
        float angle2 = angleStep * (i + 1);

        // Vrh unutrašnje kružnice
        vertices.push_back(x + innerRadius * cos(angle1));
        vertices.push_back(y + innerRadius * sin(angle1));
        vertices.push_back(0.0f);

        // Vrh spoljne kružnice
        vertices.push_back(x + outerRadius * cos(angle1));
        vertices.push_back(y + outerRadius * sin(angle1));
        vertices.push_back(0.0f);

        // Drugi vrh spoljne kružnice
        vertices.push_back(x + outerRadius * cos(angle2));
        vertices.push_back(y + outerRadius * sin(angle2));
        vertices.push_back(0.0f);

        // Drugi vrh unutrašnje kružnice
        vertices.push_back(x + innerRadius * cos(angle2));
        vertices.push_back(y + innerRadius * sin(angle2));
        vertices.push_back(0.0f);

        // Ponavljanje za travu (krofnu)
        vertices.push_back(x + innerRadius * cos(angle1));
        vertices.push_back(y + innerRadius * sin(angle1));
        vertices.push_back(0.0f);
    }
}

void drawGrass(unsigned int shaderProgramGrass, const std::vector<float>& verticesGrass) {
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesGrass.size() * sizeof(float), verticesGrass.data(), GL_STATIC_DRAW);

    // Podesavanje atributa verteksa
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Iscrtavanje trave
    glUseProgram(shaderProgramGrass);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, verticesGrass.size() / 3);
    glBindVertexArray(0);
}

void drawOasis(unsigned int shaderProgramOasis, const std::vector<float>& verticesOasis) {
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesOasis.size() * sizeof(float), verticesOasis.data(), GL_STATIC_DRAW);

    // Podesavanje atributa verteksa
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Iscrtavanje oaze
    glUseProgram(shaderProgramOasis);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, verticesOasis.size() / 3);
    glBindVertexArray(0);
}

// Funkcija za dodavanje podataka o piramidama
void addPyramidData(std::vector<float>& vertices, float x, float y, float size)
{
    float halfSize = size / 2.0f;
    float height = size;

    // Teme piramide
    float px = x;
    float py = y + height;
    float pz = 0.0f;

    // Osnova piramide
    float v1x = x - halfSize, v1y = y, v1z = -halfSize;
    float v2x = x + halfSize, v2y = y, v2z = -halfSize;

    // Stranice piramide
    vertices.insert(vertices.end(), { px, py, pz, v1x, v1y, v1z, v2x, v2y, v2z }); // Prednja strana
}

void getMonitorResolution(int& width, int& height)
{
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
    width = videoMode->width;
    height = videoMode->height;
}

unsigned int compileShader(GLenum type, const char* source)
{
    //Uzima kod u fajlu na putanji "source", kompajlira ga i vraca sejder tipa "type"
    //Citanje izvornog koda iz fajla
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str(); //Izvorni kod sejdera koji citamo iz fajla na putanji "source"

    int shader = glCreateShader(type); //Napravimo prazan sejder odredjenog tipa (vertex ili fragment)
    
    int success; //Da li je kompajliranje bilo uspjesno (1 - da)
    char infoLog[512]; //Poruka o gresci (Objasnjava sta je puklo unutar sejdera)
    glShaderSource(shader, 1, &sourceCode, NULL); //Postavi izvorni kod sejdera
    glCompileShader(shader); //Kompajliraj sejder

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); //Provjeri da li je sejder uspjesno kompajliran
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog); //Pribavi poruku o gresci
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{
    //Pravi objedinjeni sejder program koji se sastoji od Vertex sejdera ciji je kod na putanji vsSource

    unsigned int program; //Objedinjeni sejder
    unsigned int vertexShader; //Verteks sejder (za prostorne podatke)
    unsigned int fragmentShader; //Fragment sejder (za boje, teksture itd)

    program = glCreateProgram(); //Napravi prazan objedinjeni sejder program

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource); //Napravi i kompajliraj vertex sejder
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource); //Napravi i kompajliraj fragment sejder

    //Zakaci verteks i fragment sejdere za objedinjeni program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program); //Povezi ih u jedan objedinjeni sejder program
    glValidateProgram(program); //Izvrsi provjeru novopecenog programa

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success); //Slicno kao za sejdere
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    //Posto su kodovi sejdera u objedinjenom sejderu, oni pojedinacni programi nam ne trebaju, pa ih brisemo zarad ustede na memoriji
    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}
