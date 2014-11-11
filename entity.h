#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#define PI 3.14159265358979323846

class Entity {
public:
	Entity();
	Entity(float, float, float, GLuint, float, float, float = 0, float = 0);
	void Draw();
	void setY(float);
	void setX(float);
	void setDire(float, float);
	float getX();
	float getY();
	float getDireX();
	float getDireY();
	void setXVel(float);
	void setYVel(float);
	float getXVel();
	float getYVel();

	void Scale(float, float);
	void Translate(float, float);
	void Rotate(float);
	void buildMatrix();

	Matrix mat;
	float scale_x;
	float scale_y;
	float translate_x;
	float translate_y;
	float rotation;
private:
	float x;
	float y;

	GLuint textureID;

	float width;
	float height;

	float xVel;
	float yVel;

	float direction_x;
	float direction_y;
};