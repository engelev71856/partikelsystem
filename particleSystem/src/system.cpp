#include "system.h"


//-------------------------------------------------------------------
ParticleSystem::ParticleSystem()
{
	emitterPos = ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);

	rate = 3;

	time0 = ofGetElapsedTimeMillis();

}

//-------------------------------------------------------------------

ParticleSystem::~ParticleSystem()
{
}

//-------------------------------------------------------------------

void ParticleSystem::setup()
{
	particles.clear();

	// Bild einlesen 
	dotsPicture.load("images/point.jpg");

}

void ParticleSystem::update()
{
	// Parameter timestep: the time in seconds between the current and the previous callings of this function.
		// Will be used for Particle:update(float timestep) Methode
		//Compute timestep
	timeNow = ofGetElapsedTimeMillis();
	timestep = timeNow - time0;
	time0 = timeNow;

	//calculate number of new particles
	numNewParticles = round(timestep * rate);

	//update "old" particles
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->update(timestep);
	}
	
	//UPDATE VARIANTE mit Pixelfarbe beeinflusst timestep 
	/*
	for (int i = 0; i < particles.size();) {

		int x = particles[i]->getPosX();
		int y = particles[i]->getPosY();

		ofColor c = dotsPicture.getColor(x, y);
		ofFloatColor col(c);
		float g = col.g; // wenn g: 128 dann float 0,5
		// g == 0, Partikle bewegen sich auf schwarz schneller
		if (g == 0) {
			float tt = 150;
			particles[i]->update(tt);
		}
		else {
			particles[i]->update(timestep);
		}
	}
	*/
		//delete old particle 

		for (int i = 0; i < particles.size();) {
			if (particles[i]->getAgeNorm() > 1.2) {
				delete particles[i];
				particles.erase(particles.begin() + i);
			}
			else {
				i++;
			}
		}

		//generate new particles
		/*
		for (int i = 0; i < numNewParticles; i++) {
			Particle* newParticle = new Particle();
			newParticle->setup(emitterPos);
			particles.emplace_back(newParticle);
		}
		*/


		
		ofPixels & pixels = dotsPicture.getPixels();
		int w = dotsPicture.getWidth();
		int h = dotsPicture.getHeight();

		for (int i = 0; i <  pixels.size(); i += 3) {

			//rot = index + 0
			//grün = index + 1
			//blau = index + 3

		float x;
		float y;

		if (pixels[i] > 0) {
			int index = i / 3;
			if (index < w) {
				y = 1;
				x = index;
			}
			if (index > w) {
				float result = index % w;
				x = result;
				y = round(index / w);
			}
			ofVec2f v;
			v.set(x, y);

			Particle* newParticle = new Particle();
			newParticle->setup(v);
			particles.emplace_back(newParticle);
		}
	}
		


}


void ParticleSystem::draw()
{
	dotsPicture.draw(0, 0);

	for (int i = 0; i < particles.size(); i++) {
		particles[i]->draw();
	}

	ofSetColor(255, 0, 255);
	ofDrawBitmapString("NumParticles: " + ofToString(particles.size()), 10, 10);
	ofSetColor(255);

	//frame count 

}