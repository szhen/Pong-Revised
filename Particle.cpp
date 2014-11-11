#include "Particle.h"

ParticleEmitter::ParticleEmitter() {}

ParticleEmitter::ParticleEmitter(unsigned int particleCount) {
	maxLifetime = 1.0f;
	startSize = 1.0f;
	endSize = 0.0f;
	sizeDeviation = 0.05f;
	for (unsigned int i = 0; i < particleCount; ++i) {
		Particle tmp;
		tmp.lifetime = randomNumber(0.0f, 1.0f) * maxLifetime;
		tmp.pos = pos;
		particles.push_back(tmp);
	}
}

void ParticleEmitter::update(float elapsed) {
	for (int i = 0; i<particles.size(); i++) {
		particles[i].velocity.x += gravity.x * elapsed;
		particles[i].velocity.y += gravity.y * elapsed;
		particles[i].pos.x += particles[i].velocity.x * elapsed;
		particles[i].pos.y += particles[i].velocity.y * elapsed;
		particles[i].lifetime += elapsed;

		if (particles[i].lifetime > maxLifetime) {
			particles[i].lifetime = 0.0f;
			particles[i].velocity.x = velocity.x - (velocityDeviation.x * 0.5f) + (velocityDeviation.x * (((float)rand() / (float)RAND_MAX)));
			particles[i].velocity.y = velocity.y - (velocityDeviation.y * 0.5f) + (velocityDeviation.y * (((float)rand() / (float)RAND_MAX)));
			particles[i].pos = pos;
		}
	}
}

void ParticleEmitter::render() {
	std::vector<float> particleVertices;

	for (int i = 0; i < particles.size(); i++) {
		particleVertices.push_back(particles[i].pos.x);
		particleVertices.push_back(particles[i].pos.y);
	}

	glVertexPointer(2, GL_FLOAT, 0, particleVertices.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glPointSize(2.0f);
	glDrawArrays(GL_POINTS, 0, particleVertices.size() / 2);
	glDisableClientState(GL_VERTEX_ARRAY);
}