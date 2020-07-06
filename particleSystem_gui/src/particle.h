#pragma once
#include "ofMain.h"




class Particle
{
public:
	Particle();
	~Particle();

	//setup particle
	void setup(ofVec2f emitterPos, ofVec2f speed, float lifetime); 
	void update(float timeNow, float timestep, float ratio, float distanceThreshold);
	void draw();
	void setAttractor(int pathId);

	float getAgeNorm();
	bool toBeKilled;

	ofVec2f attractor;
	bool wantNextAttractor;
	int pathId;
	//int subpathId;
	int knotId;
	

	ofVec2f speedRange;
	float speed;

private:
	//Position
	ofVec2f pos;
	//Velocity
	ofVec2f vel;
	//old Position for trail 
	ofVec2f oldpos;
	
	//allowed lifetime 
	//constant value:the maximal time of living for the particle;
	//when age is greater than lifetime, the particle dies
	float time;
	float lifetime;
	//time of living
	float age;
	//color 
	//color 
	float hue;
	float sat;
	float bri;
	//oder ofVec4f color;
	ofColor color;
	float size;
};