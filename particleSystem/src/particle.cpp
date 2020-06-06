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

void Particle::setup(ofVec2f emitterPos)
{
	pos.x = emitterPos.x;
	pos.y = emitterPos.y;
	vel.x = ofRandom(-0.1, 0.2);
	vel.y = ofRandom(-0.1, 0.2);
	lifetime = 2000;
	age = 0;
	color = ofColor::red;
	size = 1;

}

//-------------------------------------------------------------------

void Particle::update(float timestep)
{

	//UPDATE POSITION
	//UPDATE VELOCITY
	//ALTE PARTIKEL ENTFERNEN

	//Update pos
	pos += vel * timestep; // geschwindigkeit gibt gleichzeitig die richtung an! neue 
	vel *= 0.95; //vel * = 0.1 * textwert

	//update time and check if particle should die
	age += timestep;

	//stay on screen
	
	if (pos.x > ofGetWidth()) {
		pos.x = ofGetWidth();
		vel.x *= -1.0;
	}
	else if (pos.x < 0) {
		pos.x = 0;
		vel.x *= -1.0;
	}
	if (pos.y > ofGetHeight()) {
		pos.y = ofGetHeight();
		vel.y *= -1.0;
	}
	else if (pos.y < 0) {
		pos.y = 0;
		vel.y *= -1.0;
	}

}

//-------------------------------------------------------------------

void Particle::draw()
{
	//ofSetColor(color);
	ofSetColor(255, 63, 180);
	ofDrawCircle(pos.x, pos.y, size);
	ofSetColor(255);


}

//-------------------------------------------------------------------

float Particle::getAgeNorm()
{
	return age / lifetime;
}

void Particle::setVel(ofVec2f v)
{
	vel.x = v.x;
	vel.y = v.y;
}

int Particle::getPosX()
{
	int px = pos.x;
	return px;
}

int Particle::getPosY()
{
	int py = pos.y;
	return py;
}
