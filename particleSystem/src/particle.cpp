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

void Particle::setup(ofVec2f emitterPos, ofVec2f speed, float lifetime)
{
	pos.x = emitterPos.x;
	pos.y = emitterPos.y;

	attractor = pos;
	
	this->speedRange = speed;
	this->speed = ofRandom(speed.x, speed.y);

	vel = ofVec2f(ofRandomf() * this->speed, ofRandomf() * this->speed);

	this->lifetime = lifetime * 1000;
	time = 0; 
	age = 0;
	color = ofColor(155,48,255);
	color.a = 127;
	size = 2;
	toBeKilled = false;

	//this->subpathId = subpathId;
	//this->pathId = pathId;
	knotId = 0;
}

//-------------------------------------------------------------------

void Particle::update(float timeNow, float timestep, float ratio, float distanceThreshold)
{
	//
	time = timeNow;
	//save old pos 
	oldpos = ofVec2f(pos.x, pos.y);
	oldpos += vel * ((timestep / 2)/2);
	

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
	/*
	if (knotId == 3) {
		color = ofColor(126, 192, 238);
		color.a = 100;
	}
	if (knotId == 5) {
		color = ofColor(0, 255, 255);
		color.a = 70;
	}*/


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

	ofSetColor(color);

	if (knotId != 0) {
		float rangeInputHue = 1 - getAgeNorm();
		hue = ofMap(rangeInputHue, 0, 1, 170, 220);
		sat = 205;
		bri = 190;
		color = ofColor::fromHsb(hue, sat, bri);
	}
	
	if (attractor != pos){
		if (knotId > 2){
	ofSetLineWidth(2);
	ofDrawLine(oldpos.x, oldpos.y, pos.x, pos.y);
		}
	}
	
	/*if (toBeKilled != true) {
		//Compute size
		float size = ofMap(
			fabs(time - lifetime / 2), 0, lifetime / 2, 3, 1);
		//Compute color
		ofColor color = ofColor(155, 48, 255);
		float hue = ofMap(time, 0, lifetime, 128, 255);
		color.setHue(hue);
		ofSetColor(color);
	}*/
	//color faden von magenta/lila zu step ein cyan dann led farbe 

	//start Farbe bei age = 0;
	//end Farbe bei getAgeNorm() > 1

	//Farbänderung berechnen, bei age wird größer 


	ofDrawCircle(pos.x, pos.y, size);
	ofSetColor(255);
}

void Particle::setAttractor(int pathId)
{
	this->pathId = pathId;
}

//-------------------------------------------------------------------

float Particle::getAgeNorm()
{
	return age / lifetime;
}