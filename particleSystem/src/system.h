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

	class Knoten {
	public:
		ofVec2f pos;
		int ebene; //radiusebene
		/*Knoten *parent = NULL;
		int childcount = 0;*/
	};
	vector<Knoten> knoten;
	static constexpr float knotenradius = 16;
	static constexpr int ebenenanzahl = 3;
	

	struct Ebene {
		struct Knoten {
			ofVec2f pos;
			Knoten *parent = NULL;
			/*int childcount = 0;*/
			vector<Knoten*> children;
		};
		vector<Knoten> knoten;
	};
	vector<Ebene> ebenen;


};

