#include "particle.h"

//-------------------------------------------------------------------

Particle::Particle()
{
	
}

//-------------------------------------------------------------------

Particle::~Particle()
{
}

//-------------------------------------------------------------------

void Particle::setup(ofVec2f emitterPos, ofVec2f speed, float lifetime, int pathId)
{
	pos.x = emitterPos.x;
	pos.y = emitterPos.y;

	attractor = pos;
	
	this->speedRange = speed;
	this->speed = ofRandom(speed.x, speed.y);

	vel = ofVec2f(ofRandomf() * this->speed, ofRandomf() * this->speed);

	this->lifetime = lifetime * 1000;
	age = 0;
	color = ofColor(155,48,255);
	color.a = 127;
	size = 2;
	toBeKilled = false;

	this->pathId = pathId;
	knotId = 0;
}

//-------------------------------------------------------------------

void Particle::update(float timestep, float ratio, float distanceThreshold)
{
	//Update pos
	pos += vel * timestep; // geschwindigkeit gibt gleichzeitig die richtung an! neue 

	//calculate new direction with ratio between old and new direction
	vel = ((vel.normalize() * ratio) + (attractor - pos).normalize() * (1 - ratio)).normalize() * speed;

	//update time and check if particle should die
	age += timestep;

	//check if distance to current attractor is under threshold
	if ((attractor - pos).length() < distanceThreshold) {
		wantNextAttractor = true;
	}

	if (knotId == 3) {
		color = ofColor(126, 192, 238);
		color.a = 100;
	}
	if (knotId == 5) {
		color = ofColor(0, 255, 255);
		color.a = 70;
	}


	if (getAgeNorm() > 1) {
		toBeKilled = true;
	}

	//kill if out of screen
	if (pos.x > ofGetWidth()) {
		toBeKilled = true;
	}
	else if (pos.x < 0) {
		toBeKilled = true;
	}
	if (pos.y > ofGetHeight()) {
		toBeKilled = true;
	}
	else if (pos.y < 0) {
		toBeKilled = true;
	}

}

//-------------------------------------------------------------------

void Particle::draw()
{
	//ofSetColor(color);
	ofSetColor(color);
	ofDrawCircle(pos.x, pos.y, size);
	ofSetColor(255);


}

//-------------------------------------------------------------------

float Particle::getAgeNorm()
{
	return age / lifetime;
}