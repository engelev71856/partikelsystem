#include "system.h"


//-------------------------------------------------------------------

ParticleSystem::ParticleSystem()
{
	emitterPos = ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);
	time0 = ofGetElapsedTimeMillis();
}

//-------------------------------------------------------------------

ParticleSystem::~ParticleSystem()
{
}

//-------------------------------------------------------------------

void ParticleSystem::setup()
{
	//attractor liste=berechnung wohin er sich bewegen muss, soll in update-methode
	//Normierung eines Vektors, 
	//Stelle der einen Liste ID 
	
	particles.clear();

	//Gui generierung
	parameterGroup.add(rate.set("rate", 1,0,10));
	parameterGroup.add(lifeTime.set("lifetime", 5, 0, 20));
	parameterGroup.add(minSpeed.set("min speed", .1, 0, 1));
	parameterGroup.add(maxSpeed.set("max speed", .25, 0, 1));
	parameterGroup.add(ratio.set("vel ratio", 0, 0, 1));
	parameterGroup.add(distanceThreshold.set("distance threshold", 0, 0, 100));

	parameterGroup.add(numPaths.set("num paths", 5, 1, 20));
	parameterGroup.add(numKnots.set("num knots", 5, 1, 10));
	parameterGroup.add(generateAttractor.set("generate random attractor", false));
	parameterGroup.add(useAttractor.set("use attractor", false));
	parameterGroup.add(drawKnots.set("draw Knots", false));
	gui.setup(parameterGroup);

	//load image 
	emitterImage.load("images/pointmid.jpg");
	//generate emitterlist
	emitterList = image2List(&emitterImage);
	//generate random attractor once
	generateAttractors(numPaths, numKnots);
}

//-------------------------------------------------------------------

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

	if (generateAttractor.get()) {
		generateAttractors(numPaths, numKnots);
		generateAttractor = false;
	}
	
	//update old particles
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->update(timestep, ratio, distanceThreshold);
		if (useAttractor.get()) {
			if (particles[i]->wantNextAttractor) {
				if (particles[i]->knotId + 1 < paths[particles[i]->pathId].size()) { // is there one more knot in this path?
					particles[i]->knotId++;
				}
				particles[i]->attractor = ofVec2f(paths[particles[i]->pathId][particles[i]->knotId]); //set attractor to knot
				particles[i]->wantNextAttractor = false;
			}
		}
		
	}

	//delete old particle 
	for (int i = 0; i < particles.size();) {
		if (particles[i]->toBeKilled) {
			delete particles[i];
			particles.erase(particles.begin() + i);
		}
		else {
			i++;
		}
	}

	
	//generate new particles
	for (int i = 0; i < numNewParticles; i++) {
		Particle* newParticle = new Particle();
		newParticle->setup(emitterList[ofRandom(emitterList.size() - 1)], ofVec2f(minSpeed, maxSpeed), lifeTime, ofRandom(paths.size()));
		particles.push_back(newParticle);
	}
}
//ofrandom gib wert  0 und anzahl der partikel in der liste und diesem Wert um Koordinate dauraus zu erzegeugen --> gleiches für attractor id nutzten um 
//in attraktorliste zu schauen: wo wollen wir eigentlich hin --> System durchgehen. id 500 ^000 ^000 --- > muss berechnentn in welche Richtung er muss 
//liste erzeugen punkt für punkt 

//-------------------------------------------------------------------

void ParticleSystem::draw()
{
	ofBackground(0); 
	ofFill();
	if (drawKnots) {
		for (int p = 0; p < paths.size(); p++) {
			for (int k = 0; k < paths[p].size(); k++) {
				ofDrawCircle(paths[p][k], 2);
			}
		}
	}

	ofEnableAlphaBlending();
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->draw();
	}
	ofDisableAlphaBlending();

	gui.draw();

	ofNoFill();
	ofSetColor(48);
	for (int i = 1; i <= numKnots; i++) {
		ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, ((float)i / numKnots)*(ofGetWidth() / 2));

	}
	ofSetColor(255, 0, 255);
	ofDrawBitmapString("NumParticles: " + ofToString(particles.size()), 10, 10);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()), 10, 20);
	ofSetColor(255);


	

}

//-----------------------------------------------------------------------------

vector<ofVec2f> ParticleSystem::image2List(ofImage* img)
{
	vector<ofVec2f> list;

	//get Image properties
	ofPixels & pixels = img->getPixels();
	int w = pixels.getWidth();
	int h = pixels.getHeight();

	int bpp = pixels.getBytesPerPixel();
	
	//interate through image and get values
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			
			ofVec3f col(pixels[(x + (y * w)) * 3 + 0, (x + (y * w)) * 3 + 1, (x + (y * w)) * 3 + 2]);

			if (col.length()) {//any color except black
				list.emplace_back(x, y);
			}
		}
	}

	return list;
}

//-----------------------------------------------------------------------------

void ParticleSystem::generateAttractors(int numPaths, int numKnotsperPath)
{

	paths.clear();

	//NOT CORRECT!!!
	//calc stepsize
	/*float deltaW = ofGetWidth() / numKnotsperPath;
	float deltaH = ofGetHeight() / numKnotsperPath;*/

	
	//Vektor zur zum beliebigen Ring
	/*ofVec2f ring;
	const float vX = ofGetWidth() / 2 - ring.x;
	const float vY = ofGetHeight() / 2  - ring.y;*/
	
	//Mittelpunkt als Vector mid gesetzt:
	ofVec2f out(ofGetWidth(), ofGetHeight());
	ofVec2f mid(0,0);
	mid.middle(out);

	//Vector von Mittelpunkt zum Outring
	ofVec2f sum = out - mid;
	
	float oneX = sum.x / numKnotsperPath;
	float oneY = sum.y / numKnotsperPath;
	
	for (int p = 0; p < numPaths; p++) {
		vector<ofVec2f> knots;
		

		for (int k = 0; k < numKnotsperPath; k++) {
			//ofVec2f knot(ofRandom(k * deltaW, (k + 1) * deltaW), ofRandom(k * deltaH, (k + 1) * deltaH)); //generate point 		
			//ofVec2f knot(ofRandom(k * oneX, (k + 1) * oneX), ofRandom(k * oneY, (k + 1) * one)); //generate point

			

			//generieren der ringe:
			float iteration = float(k) / float(numKnotsperPath);

			//punkt auf Geraden zwischen out und mid Mit der Gewichtung von interation  
			//knot position auf einer Linie 
			ofVec2f steps = (mid.getInterpolated(out, iteration));

			float angle = float(360) / float(numPaths);
			angle = ((p * angle) + ofRandom(-10,30));

		
			steps.rotate(angle, mid);

			float iteration2 = float(k-1) / float(numKnotsperPath);
			ofVec2f steps2 = (mid.getInterpolated(out, iteration2));
			float angle2 = ofRandom(10,90);
			steps2.rotate(angle, mid);
			float randomIteration = ofRandom(0.1,0.9);
			ofVec2f turn = (steps2.getInterpolated(steps, randomIteration));
			turn.rotate(angle2, steps2);
			//getting values
			float dX = turn.x;
			float dY = turn.y;
			//getting values for first knot 
			float aX = steps.x;
			float aY = steps.y;

			if (k == 0) {
				//generate point
				ofVec2f knot(aX, aY);
				knots.push_back(knot);// add point to path
			}
			else {
				//generate point
				ofVec2f knot(dX, dY);
				knots.push_back(knot);// add point to path
			}
		
		}
		
		paths.push_back(knots); // add path to all paths
	}

	for (int r = 0; r < paths.size(); r++) { //print all paths
		for (int k = 0; k < paths[r].size(); k++) {
			cout << "path: " << r << " knot: " << k << " x: " << paths[r][k].x << " y: " << paths[r][k].y << endl;
		}
	}



}
