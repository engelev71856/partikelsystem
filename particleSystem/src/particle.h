#pragma once
#include "ofMain.h"

class Particle
{
public:
	Particle();
	~Particle();

	//start particle
	void setup(ofVec2f emitterPos);
	//recalculate 
	void update(float timestep);
	//draw particle
	void draw();
	float getAgeNorm();

	void setVel(ofVec2f v);

	int getPosX();
	int getPosY();


private:
	//Position
	ofVec2f pos;
	//Velocity
	ofVec2f vel;
	//allowed lifetime 
	//constant value:the maximal time of living for the particle;
	//when age is greater than lifetime, the particle dies
	float lifetime;
	//time of living
	float age;
	//color 
	//oder ofVec4f color;
	ofColor color;
	float size;

	//soll partikel zerstört werden?
	bool toBeKilled;
};

