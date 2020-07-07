#pragma once
#include "ofMain.h"
#include "particle.h"
#include "ofxGui.h"
#include "ofxOsc.h"
//#include "OSCElement.h"


class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void setup();
	void update();
	void draw();
			
private:

	//system
	vector<Particle*> particles;
	ofVec2f emitterPos;
	ofVec2f attractor;
	float numNewParticles; //number of new particles in this frame

	//timing
	float time0;
	float timeNow;
	float timestep;

	//emitter
	ofImage emitterImage;
	vector<ofVec2f> emitterList;
	//attractor
	vector<ofVec2f> endpointList;
	vector<vector<ofVec2f>> paths;

	//helper method
	vector<ofVec2f> image2List(ofImage* img);
	void generateAttractors(int numKnotsperRing, vector<ofVec2f> endpoints);

	//gui elements
	ofParameter<bool> generateAttractor;
	ofParameter<bool> useAttractor;
	ofParameter<bool> drawKnots;
	ofParameterGroup parameterGroup;
	ofxPanel gui;

	//feste Parameter
	int rate;
	int numKnots;
	int numPaths;   //brauchen wir das noch? 
	int distanceThreshold;
	int numSplitlists;
	float lifeTime;
	float minSpeed;
	float maxSpeed;
	float ratio;
	float fader;
	bool randomize;   //bool könnte eigentlich auch weg	
	
	ofFbo fbo;
};