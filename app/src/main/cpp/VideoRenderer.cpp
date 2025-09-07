#include "VideoRenderer.h"
#include <android/log.h>
#include <GLES2/gl2ext.h>

// 簡易シェーダー (OESテクスチャ対応)
static const char *VERTEX_SHADER = R"(
attribute vec4 aPosition;
attribute vec2 aTexCoord;
uniform mat4 uTexMatrix;
varying vec2 vTexCoord;
void main() {
    gl_Position = aPosition;
    vTexCoord = (uTexMatrix * vec4(aTexCoord, 0, 1)).xy;
}
)";

static const char *FRAGMENT_SHADER = R"(
#extension GL_OES_EGL_image_external : require
precision mediump float;
uniform samplerExternalOES sTexture;
varying vec2 vTexCoord;
void main() {
    gl_FragColor = texture2D(sTexture, vTexCoord);
}
)";

// 板ポリゴン (四角形)
static const GLfloat VERTICES[] = {
        -1.0f, -1.0f,   // 左下
        1.0f, -1.0f,   // 右下
        -1.0f,  1.0f,   // 左上
        1.0f,  1.0f    // 右上
};
static const GLfloat TEXCOORDS[] = {
        0.0f, 1.0f,   // 左下
        1.0f, 1.0f,   // 右下
        0.0f, 0.0f,   // 左上
        1.0f, 0.0f    // 右上
};

// --------- ヘルパー関数 ---------
static GLuint loadShader(GLenum type, const char *src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            char *log = new char[len];
            glGetShaderInfoLog(shader, len, nullptr, log);
            __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Shader compile error: %s", log);
            delete[] log;
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

static GLuint createProgram(const char *vsSrc, const char *fsSrc) {
    GLuint vs = loadShader(GL_VERTEX_SHADER, vsSrc);
    GLuint fs = loadShader(GL_FRAGMENT_SHADER, fsSrc);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            char *log = new char[len];
            glGetProgramInfoLog(program, len, nullptr, log);
            __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Program link error: %s", log);
            delete[] log;
        }
        glDeleteProgram(program);
        return 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void VideoRenderer::init() {
    // シェーダー作成省略（コンパイル→リンク）
    program = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (!program) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "Failed to create program!");
        return;
    }
    vPos = glGetAttribLocation(program, "aPosition");
    vTexCoord = glGetAttribLocation(program, "aTexCoord");
    uTexMatrix = glGetUniformLocation(program, "uTexMatrix");

    glUseProgram(program);
    GLint texLoc = glGetUniformLocation(program, "sTexture");
    glUniform1i(texLoc, 0); // GL_TEXTURE0 を使う
}

void VideoRenderer::resize(int width, int height) {
    glViewport(0, 0, width, height);
}

void VideoRenderer::draw(int textureId, const float *texMatrix) {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    // 頂点座標
    glVertexAttribPointer(vPos, 2, GL_FLOAT, GL_FALSE, 0, VERTICES);
    glEnableVertexAttribArray(vPos);

    // テクスチャ座標
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, TEXCOORDS);
    glEnableVertexAttribArray(vTexCoord);

    // 変換行列
    glUniformMatrix4fv(uTexMatrix, 1, GL_FALSE, texMatrix);

    // テクスチャをバインド
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, textureId);

    // 板ポリゴン描画
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
