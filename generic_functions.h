#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#define PI 3.14159265358979323846

namespace direction {
	enum directions { LEFT, RIGHT };
}

GLuint LoadTexture(const char *image_path, GLuint imageFormat);
float randomNumber(float, float);
float lerp(float v0, float v1, float t);
void drawText(GLuint fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a);
float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax);
float easeIn(float from, float to, float time);
float easeOut(float from, float to, float time);
float easeInOut(float from, float to, float time);
float easeOutElastic(float from, float to, float time);