#include "entity.h"

Entity::Entity(float x, float y, float rot, GLuint textID, float w, float h, float direX, float direY) : x(x), y(y), rotation(rot), 
	textureID(textID), width(w), height(h), direction_x(direX), direction_y(direY) {
	scale_x = 1.0f;
	scale_y = 1.0f;
	rotation = 0.0f;
}

Entity::Entity() {}

void Entity::setX(float newX) {
	x = newX;
}

void Entity::setY(float newY) {
	y = newY;
}

float Entity::getX() {
	return x;
}

float Entity::getY() {
	return y;
}

float Entity::getDireX() {
	return direction_x;
}

float Entity::getDireY() {
	return direction_y;
}

void Entity::setDire(float newX, float newY) {
	direction_x = newX;
	direction_y = newY;
}

float Entity::getXVel() { return xVel; }
float Entity::getYVel() { return yVel; }
void Entity::setXVel(float newVel) { xVel = newVel; }
void Entity::setYVel(float newVel) { yVel = newVel; }

void Entity::Scale(float x, float y) {
	scale_x = x;
	scale_y = y;
}

void Entity::Translate(float x, float y) {
	translate_x = x;
	translate_y = y;
}

void Entity::Rotate(float rad) {
	rotation = (rad * PI) / 180.0f;
}


void Entity::buildMatrix() {
	Matrix translate;
	Matrix scale;
	Matrix rotate;

	// translate
	translate.m[3][0] = translate_x;
	translate.m[3][1] = translate_y;

	// scale
	scale.m[0][0] = scale_x;
	scale.m[1][1] = scale_y;

	// rotate
	rotate.m[0][0] = cos(rotation);
	rotate.m[1][0] = -sin(rotation);
	rotate.m[0][1] = sin(rotation);
	rotate.m[1][1] = cos(rotation);

	mat = scale * rotate * translate;
}

void Entity::Draw() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	Translate(x, y);
	Rotate(rotation);
	buildMatrix();

	glMultMatrixf(mat.ml);

	GLfloat quad[] = { -width * 0.5f, height * 0.5f, -width * 0.5f, -height * 0.5f, width * 0.5f, -height * 0.5f, width * 0.5f, height * 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}