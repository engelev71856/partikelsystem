#pragma once
#include "ofMain.h"
#include "particle.h"
#include "ofPixels.h"
#include "ofxGui.h"
#include <random>


class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void setup();
	void update();
	void draw();

	ofxPanel gui;
	
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
	ofImage falsePicture;

	ofParameter<bool> activateSystem;
	ofParameterGroup parameterGroup;

	vector<ofVec2f> dotsEmitterList;
	vector<ofVec2f> falseEmitterList;

	default_random_engine generator;

};

