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
	//switch intital
	//int setStatus;

	//system
	vector<Particle*> particles;
	ofVec2f emitterPos;
	ofVec2f attractor;
	float numNewParticles; //number of new particles in this frame
	ofParameter<int> rate;
	ofParameter<float> lifeTime;
	ofParameter<float> minSpeed;
	ofParameter<float> maxSpeed;
	ofParameter<float> ratio;
	ofParameter<int> distanceThreshold;

	//timing
	float time0;
	float timeNow;
	float timestep;

	//emitter
	ofImage emitterImage;
	vector<ofVec2f> emitterList;
	//vector<vector<ofVec2f>> transformation;
	
	vector<vector<ofVec2f>> paths;
	//vector<vector<vector<ofVec2f>>> paths;

	//helper method
	vector<ofVec2f> image2List(ofImage* img);
	void generateAttractors(int numRings, int numKnotsperRing);

	//gui elements
	ofParameter<bool> generateAttractor;
	ofParameter<bool> useAttractor;
	ofParameter<int> numPaths;
	ofParameter<int> numKnots;
	ofParameter<bool> drawKnots;
	ofParameterGroup parameterGroup;
	ofxPanel gui;

	//Mesh elements
	ofMesh mesh;
	ofParameter <float> uiAmount;
	void generateMesh(ofImage* img);
	void generateMeshUpdate(float uiAmount);
	ofEasyCam cam;
	int size = 96;//96
	
	/*vector<Knoten> knoten;
	vector<Ebene> ebenen;*/
	static constexpr float knotenradius = 16;
	static constexpr int ebenenanzahl = 3;
};