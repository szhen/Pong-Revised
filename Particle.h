#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <fstream>
#include <random>
#include <vector>
#include "Vector.h"
#include "generic_functions.h"

struct Color {
	float r;
	float g;
	float b;
	float a;
};

class Particle {
public:
	Vector pos;
	Vector velocity;
	float lifetime;
	float sizeDeviation;
	float rotation;
};

class ParticleEmitter {
public:
	ParticleEmitter();
	ParticleEmitter(unsigned int particleCount);

	void update(float elapsed);

	void render();

	std::vector<Particle> particles;

	int size;

	GLuint texture;
	Vector gravity;
	Vector pos;
	Vector velocity;
	Vector velocityDeviation;
	float maxLifetime;
	float startSize;
	float endSize;
	float sizeDeviation;
	bool additiveBlending;
	float rotationSpeed;
};