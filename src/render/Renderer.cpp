#include "render/Renderer.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char* vertexShaderSrc = R"(
    #version 460 core
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;
    
    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;
    
    out vec3 vertexColor;
    
    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0);
        vertexColor = color;
    }
)";

const char* fragmentShaderSrc = R"(
    #version 460 core
    in vec3 vertexColor;
    out vec4 FragColor;
    
    void main()
    {
        FragColor = vec4(vertexColor, 1.0);
    }
)";


Renderer::Renderer()
    : m_shaderProgram(0), m_VAO(0), m_VBO(0), m_EBO(0), m_indexCount(0), m_rotationAngle(0.0f) {
}
Renderer::~Renderer() {
    if (m_shaderProgram) {
        glDeleteProgram(m_shaderProgram);
    }
    if (m_VAO) {
        glDeleteVertexArrays(1, &m_VAO);
    }
    if (m_VBO) {
        glDeleteBuffers(1, &m_VBO);
    }
    if (m_EBO) {
        glDeleteBuffers(1, &m_EBO);
    }
}

void Renderer::Init() {
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    m_shaderProgram = CompileShader(vertexShaderSrc, fragmentShaderSrc);
    SetupCubeData();
}

void Renderer::Update(float deltaTime) {
    m_rotationAngle += deltaTime * 50.0f;
    if (m_rotationAngle >= 360.0f) {
        m_rotationAngle -= 360.0f;
    }
}

GLuint Renderer::CompileShader(const char* vertexSrc, const char* fragmentSrc) {
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSrc, nullptr);
    glCompileShader(vertex);

    int success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
    }

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSrc, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

void Renderer::SetupCubeData() {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    };
    
    float colors[] = {
        1.0f, 0.0f, 0.0f,  // Красный
        0.0f, 1.0f, 0.0f,  // Зелёный
        0.0f, 0.0f, 1.0f,  // Синий
        1.0f, 1.0f, 0.0f,  // Жёлтый
        1.0f, 0.0f, 1.0f,  // Пурпурный
        0.0f, 1.0f, 1.0f,  // Cyan
        1.0f, 1.0f, 1.0f,  // Белый
        0.5f, 0.5f, 0.5f,  // Серый
    };
    
    GLuint indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 6, 5, 4, 7, 6,
        4, 0, 3, 4, 3, 7,
        1, 5, 6, 1, 6, 2,
        3, 2, 6, 3, 6, 7,
        4, 5, 1, 4, 1, 0
    };
    
    m_indexCount = 36;
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    GLuint colorVBO;
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawCube() {
    glUseProgram(m_shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(m_rotationAngle), glm::vec3(1.0f, 1.0f, 0.0f));
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

    GLuint modelLoc = glGetUniformLocation(m_shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(m_shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(m_shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::Present() {
}