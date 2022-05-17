//
// Created by Hugh on 2022/5/16.
//

#include "basic_renderer.h"
#include <vector>
#include <malloc.h>
#include <android/log.h>

#include "../glm/vec3.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.5f, 0.5f, 0.0f);

glm::vec3 lightPos = glm::vec3(1.2f, 0.0f, 2.0f);

int p = 15;
float ambientStrength = 0.2f;
float diffuseStrength = 1.0f;
float highStrength = 1.0f;

float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
};


float  QUAD[][3] = {
        {-0.0f, -0.0f,0.0f},
        {0.7f, -0.0f, 0.0f},
        {-0.0f, 0.7f, 0.0f},
        {0.7f, 0.7f, 0.0f}
};

const unsigned int ORDER1[6] = {0,1,2,3,2,1};

const unsigned int ORDER[36] = {0,1,2,3,4,5,
                                6,7,8,9,10,11,
                                12,13,14,15,16,17,
                                18,19,20,21,22,23,
                                24,25,26,27,28,29,
                                30,31,32,33,34,35};

// for shader loading
#define LOG_TAG "GLES C++"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

bool checkGlError(const char *funcName) {
    GLint err = glGetError();
    if(err != GL_NO_ERROR) {
        ALOGE("GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}

GLuint  createShader(GLenum shaderType, const char* src) {
    GLuint shader = glCreateShader(shaderType);
    if(!shader) {
        checkGlError("glCreateShader");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if(infoLogLen > 0) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if(infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                ALOGE("Could not compile %s shader:\n%s\n",
                      shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                      infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createProgram(const char* vtxSrc, const char* fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if(!vtxShader)
        goto exit;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if(!fragShader)
        goto exit;

    program = glCreateProgram();
    if(!program) {
        checkGlError("glCreateProgram");
        goto exit;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(!linked) {
        ALOGE("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if(infoLogLen) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if(infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                ALOGE("Could not link program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

    exit:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}

void basic_renderer::Init(AAssetManager* _mgr){
    mgr = _mgr;

    const GLubyte* OpenGLVersion = glGetString(GL_VERSION);

    pMyShader = new Shader("vert", "frag", mgr);
    pMyShader->use();
    // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)2340 / (float)1080, 0.1f, 100.0f);
    pMyShader->setMat4("projection", projection);

    glEnable(GL_DEPTH_TEST);

    // allocate a VAO and a VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    // bind attribute
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
/*


    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

    std::vector<unsigned int> testOrder(ORDER, ORDER + 36);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*testOrder.size(), &testOrder[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    static const char VERTEX_SHADER[] =
            "precision highp float;\n"
            "attribute vec4 inPosition;\n"
            "void main(){\n"
            "    gl_Position = inPosition;\n"
            "}\n";
    static const char FRAGMENT_SHADER[] =
            "precision highp float;\n"
            "void main() {\n"
            "    gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);\n"
            "}\n";

    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    mVertexAttribPos = glGetAttribLocation(mProgram, "inPosition");*/
}

// renderer
void basic_renderer::Draw() const{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    pMyShader->use();

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
    pMyShader->setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    static int time = 0;
    float angle = time++;
    model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    pMyShader->setMat4("model",model);

    // pass uniform parameters
    int pLoc = glGetUniformLocation(pMyShader->ID, "p");
    glUniform1i(pLoc, p);
    int ambientStrengthLoc = glGetUniformLocation(pMyShader->ID, "ambientStrength");
    glUniform1f(ambientStrengthLoc, ambientStrength);
    int diffuseStrengthLoc = glGetUniformLocation(pMyShader->ID, "diffuseStrength");
    glUniform1f(diffuseStrengthLoc, diffuseStrength);
    int highStrengthLoc = glGetUniformLocation(pMyShader->ID, "highStrength");
    glUniform1f(highStrengthLoc, highStrength);
    pMyShader->setVec3("objectColor", 1.0f, 0.5f, 0.3f);
    pMyShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    pMyShader->setVec3("lightPos", lightPos);
    pMyShader->setVec3("viewPos", cameraPos);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

/*    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glEnableVertexAttribArray(mVertexAttribPos);
    glVertexAttribPointer(mVertexAttribPos, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
}