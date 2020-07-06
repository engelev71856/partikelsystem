#pragma once
#include "ofMain.h"
#include "particle.h"
#include "ofxGui.h"


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
	//ofParameter<int> numPaths;
	//ofParameter<int> numKnots;
	ofParameter<bool> drawKnots;
	//ofParameter<bool> randomize;
	//ofParameter<int> rate;
	//ofParameter<float> lifeTime;
	//ofParameter<float> minSpeed;
	//ofParameter<float> maxSpeed;
	//ofParameter<float> ratio;
	//ofParameter<int> distanceThreshold;
	//ofParameter<int> numSplitlists;
	//ofParameter<int> splitSlider;
	//ofParameter<float> fader;

	int rate;
	float lifeTime;
	float minSpeed;
	float maxSpeed;
	float ratio;
	float fader;
	
	bool randomize;   //bool könnte eigentlich auch weg
	int numKnots; 
	int numPaths;   //brauchen wir das noch? 
	int distanceThreshold;
	int numSplitlists;


	ofParameterGroup parameterGroup;
	ofxPanel gui;
	
	ofFbo fbo;

	/*vector<Knoten> knoten;
	vector<Ebene> ebenen;*/
	static constexpr float knotenradius = 16;
	static constexpr int ebenenanzahl = 3;
};