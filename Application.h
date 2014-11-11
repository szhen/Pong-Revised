#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <fstream>
#include <ctime>
#include <cmath>
#include <random>
#include <string>
#include <vector>
#include "entity.h"
#include "Vector.h"
#include "Particle.h"
#include "generic_functions.h"
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define SCREENSHAKESPEED 10.0

class Application{
public:
	Application();
	~Application();

	void drawBlackBox(float);

	void screenShake();

	bool UpdateAndRender();
private:
	SDL_Event event;
	SDL_Window* displayWindow;

	Entity paddle1;
	Entity paddle2;
	Entity ball;
	Entity p1win;
	Entity p2win;

	ParticleEmitter fireworks;
	ParticleEmitter flames;

	const Uint8 *keys;

	bool done;
	bool scored;

	int p1Pt;
	int p2Pt;

	float fade;
	float fadeTime;
	float shakeCD;
	float lastFrameTicks;
	float timeLeftOver;
	float screenShakeVal;
	float screenShakeIntensity;
	float fireworksTimer;

	void Init();
	void update(float);
	void FixedUpdate();
	void render();
};