#include "Application.h"
using namespace std;

Application::Application() {
	Init();
}

Application::~Application() {
	SDL_Quit();
}

void Application::Init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	paddle1 = Entity(-1.28f, 0.0f, 0.0f, LoadTexture("paddle.png", GL_BGRA), 0.1f, 0.3f);
	paddle2 = Entity(1.28f, 0.0f, 0.0f, LoadTexture("paddle.png", GL_BGRA), 0.1f, 0.3f);
	ball = Entity(0.0f, 0.0f, 0.0f, LoadTexture("pokeball.jpg", GL_BGRA), 0.1f, 0.1f, randomNumber(-1.5f, 1.5f), randomNumber(-1.5f, 1.5f));
	ball.setXVel(1.0f);
	ball.setYVel(1.0f);
	p1win = Entity(0.0f, 0.0f, 0.0f, LoadTexture("p1win.png", GL_RGBA), 0.7f, 0.5f, 0.0f, 0.0f);
	p2win = Entity(0.0f, 0.0f, 0.0f, LoadTexture("p2win.png", GL_RGBA), 0.7f, 0.5f,  0.0f, 0.0f);

	srand(time(NULL));
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;
	screenShakeVal = 1.0f;
	screenShakeIntensity = .00004f;
	fadeTime = 0.0f;
	shakeCD = 3.0;
	keys = SDL_GetKeyboardState(NULL);
	done = false;
	scored = true;
	p1Pt = 0;
	p2Pt = 0;
	fireworks = ParticleEmitter(500);
	fireworksTimer = 1.5f;
	fireworks.gravity = Vector(0.0f, -2.0f, 0.0f);
	fireworks.velocity = Vector(0.0f, 0.9f, 0.0f);
	fireworks.velocityDeviation = Vector(0.7f, 1.0f, 0.0f);
	flames = ParticleEmitter(500);
	flames.texture = LoadTexture("fire.png", GL_RGBA);
	flames.velocity = Vector(0.0f, 1.2f, 0.0f);
	flames.velocityDeviation = Vector(0.1f, 0.0f, 0.0f);
}

bool Application::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return true;
		}
	}
	// timestep 
	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}

	update(elapsed);

	timeLeftOver = fixedElapsed;

	render();
	return false;
}

void Application::update(float elapsed) {
	fireworksTimer += elapsed;
	screenShakeVal += elapsed;
	if (fadeTime < 2.0f)
		fadeTime += elapsed;
	shakeCD += elapsed;
	paddle1.setYVel(0.0f);
	paddle2.setYVel(0.0f);

	if (scored == true) {
		if (keys[SDL_SCANCODE_SPACE]) {
			scored = false;
			Vector tmp(randomNumber(-1.5f, 1.5f), randomNumber(-1.5f, 1.5f), 0.0f);
			tmp.normalize();
			ball.setDire(tmp.x, tmp.y);
			//ball.setDire(randomNumber(-1.5f, 1.5f), randomNumber(-1.5f, 1.5f));
		}
	}
	if (keys[SDL_SCANCODE_W]) {
		paddle1.setYVel(1.5f);
	}
	if (keys[SDL_SCANCODE_S]) {
		paddle1.setYVel(-1.5f);
	}
	if (keys[SDL_SCANCODE_UP]) {
		paddle2.setYVel(1.5f);
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		paddle2.setYVel(-1.5f);
	}

	if (p1Pt < 7 && p2Pt < 7) {
		if (paddle1.getY() + paddle1.getYVel() * elapsed > 0.85f || paddle1.getY() + paddle1.getYVel() * elapsed < -0.85f)
			paddle1.setYVel(0.0f);
		if (paddle2.getY() + paddle2.getYVel() * elapsed > 0.85f || paddle2.getY() + paddle2.getYVel() * elapsed < -0.85f)
			paddle2.setYVel(0.0f);

		//check if game started
		if (scored == false) {
			//ball logic
			if (ball.getY() > 0.95) {
				if (ball.getDireY() > 0) { //in case ball overshoots into the wall to prevent ball from "sliding" on surface
					ball.setDire(ball.getDireX(), -(ball.getDireY()));
					screenShakeVal = 0.0f;
					shakeCD = 0.0f;
				}
			}
			else if (ball.getY() < -0.95) {
				if (ball.getDireY() < 0) {//in case ball overshoots into the wall to prevent ball from "sliding" on surface
					ball.setDire(ball.getDireX(), -(ball.getDireY()));
					screenShakeVal = 0.0f;
					shakeCD = 0.0f;
				}
			}

			//paddle collision
			if ((ball.getX() + 0.05f > paddle2.getX() - 0.05f) && (ball.getY() + 0.05f > paddle2.getY() - 0.15f && ball.getY() - 0.05f < paddle2.getY() + 0.15f)) {
				if (ball.getDireX() > 0) { //in case ball overshoots into the paddle to prevent ball from "sliding" on surface
					ball.setDire(-(ball.getDireX()), ball.getDireY());
					screenShakeVal = 0.0f;
					shakeCD = 0.0f;
					Vector tmp(ball.getX(), ball.getY(), 0.0f);
					fireworks.pos = tmp;
					fireworksTimer = 0.0f;
					for (unsigned int i = 0; i < fireworks.particles.size(); ++i) {
						fireworks.particles[i].pos = fireworks.pos;
						fireworks.particles[i].lifetime = 0.0f;
					}
				}
			}
			else if ((ball.getX() - 0.05f < paddle1.getX() + 0.05f) && (ball.getY() + 0.05f > paddle1.getY() - 0.15f && ball.getY() - 0.05f < paddle1.getY() + 0.15f)) {
				if (ball.getDireX() < 0) { //in case ball overshoots into the paddle to prevent ball from "sliding" on surface
					ball.setDire(-(ball.getDireX()), ball.getDireY());
					screenShakeVal = 0.0f;
					shakeCD = 0.0f;
					Vector tmp(ball.getX(), ball.getY(), 0.0f);
					fireworks.pos = tmp;
					fireworksTimer = 0.0f;
					for (unsigned int i = 0; i < fireworks.particles.size(); ++i) {
						fireworks.particles[i].pos = fireworks.pos;
						fireworks.particles[i].lifetime = 0.0f;
					}
				}
			}
			else if (ball.getX() > 1.23) { //if ball passes paddle, score
				ball.setX(0.0f);
				ball.setY(0.0f);
				ball.setDire(randomNumber(-1.5f, 1.5f), randomNumber(-1.5f, 1.5f));
				scored = true;
				++p1Pt;
			}
			else if (ball.getX() < -1.23) {
				ball.setX(0.0f);
				ball.setY(0.0f);
				ball.setDire(randomNumber(-1.5f, 1.5f), randomNumber(-1.5f, 1.5f));
				scored = true;
				++p2Pt;
			}
		}
		else if (scored == true) {
			ball.setDire(0.0f, 0.0f);
		}

		fade = easeIn(1.0f, 0.0f, fadeTime);

		flames.pos.x = ball.getX();
		flames.pos.y = ball.getY();
		fireworks.update(elapsed);
		flames.update(elapsed);

		ball.setX(ball.getX() + ball.getDireX() * ball.getXVel() * elapsed);
		ball.setY(ball.getY() + ball.getDireY() * ball.getYVel() * elapsed);
		paddle1.setY(paddle1.getY() + paddle1.getYVel() * elapsed);
		paddle2.setY(paddle2.getY() + paddle2.getYVel() * elapsed);
	}
}

void Application::FixedUpdate() {
	// no physics in this game
}

void Application::render() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (shakeCD < 1.0f)
		screenShake();
	else
		glLoadIdentity();

	if (fireworksTimer < fireworks.maxLifetime)
		fireworks.render();

	//drawing line in the middle
	glMatrixMode(GL_MODELVIEW);
	Matrix mat;
	mat.identity();
	glPushMatrix();

	glMultMatrixf(mat.ml);
	GLfloat midLine[] = { -0.01f, 1.0f, -0.01f, -1.0f, 0.01f, -1.0f, 0.01f, 1.0f };
	glVertexPointer(2, GL_FLOAT, 0, midLine);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat midColor[] = { 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f };
	glColorPointer(3, GL_FLOAT, 0, midColor);
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_QUADS, 0, 4);
	glPopMatrix();
	glDisableClientState(GL_COLOR_ARRAY);

	if (p1Pt == 7) {
		p1win.Draw();
	}
	else if (p2Pt == 7) {
		p2win.Draw();
	}
	else {
		ball.Draw();
		paddle1.Draw();
		paddle2.Draw();
	}

	if (fadeTime < 1.0f) {
		drawBlackBox(fade);
	}

	SDL_GL_SwapWindow(displayWindow);
}

void Application::screenShake() {
	glTranslatef(cos(screenShakeVal * SCREENSHAKESPEED)* screenShakeIntensity, sin(screenShakeVal * SCREENSHAKESPEED)* screenShakeIntensity, 0.0f);
}

void Application::drawBlackBox(float alpha) {
	glMatrixMode(GL_MODELVIEW);
	Matrix mat2;
	mat2.identity();
	glPushMatrix();

	glMultMatrixf(mat2.ml);
	GLfloat midLine2[] = { -1.33f, -1.0f, 1.33f, -1.0f, 1.33f, 1.0f, -1.33f, 1.0f };
	glVertexPointer(2, GL_FLOAT, 0, midLine2);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat midColor2[] = { 0.0f, 0.0f, 0.0f, alpha, 0.0f, 0.0f, 0.0f, alpha, 0.0f, 0.0f, 0.0f, alpha, 0.0f, 0.0f, 0.0f, alpha };
	glColorPointer(4, GL_FLOAT, 0, midColor2);
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_QUADS, 0, 4);
	glPopMatrix();
	glDisableClientState(GL_COLOR_ARRAY);
}