#pragma once
#include "ofMain.h"
#include "particle.h"
#include "ofPixels.h"


class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void setup();
	void update();
	void draw();


private:

	vector<Particle*> particles;
	//emitter: New particles are generated from this point
	ofVec2f emitterPos;

	float rate; //particles per millisecond
	float numNewParticles;

	float time0;
	float timeNow;
	float timestep;

	ofImage dotsPicture;
};

