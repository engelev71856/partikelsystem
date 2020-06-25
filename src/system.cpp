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

	/*for (vector<ofVec2f>::iterator itl = list.begin(); itl != list.end(); ++itl) {
		list.x *= ofGetWidth() / 2;
		list.y *= ofGetHeight() / 2;
	}*/
	if ((w < ofGetWidth()) && (h < ofGetHeight())) {
		for (int i = 0; i < list.size(); i++) {
			float calX = (ofGetWidth() - w) / 2;
			float calY = (ofGetHeight() - h) / 2;
			list[i].x += calX;
			list[i].y += calY;
		}
	}


	return list;
}

//-----------------------------------------------------------------------------

void ParticleSystem::generateAttractors(int numPaths, int numKnotsperPath)
{
	paths.clear();

	ofVec2f mid(ofGetWidth() / 2, ofGetHeight() / 2);

	for (int p = 0; p < numPaths; p++) {
		vector<ofVec2f> knots;
		ofVec2f endKnot;
		ofVec2f endKnotA(10,10);
		ofVec2f endKnotB(ofGetWidth()-10, 10);
		ofVec2f endKnotC(10, ofGetHeight()-10);
		ofVec2f endKnotD(ofGetWidth() - 10, ofGetHeight() - 10);

		
		for (int k = 0; k < numKnotsperPath; k++) {

			int testPath = p % 4; 
			
			switch (testPath) {
			case 0:
				endKnot = ofVec2f(endKnotA);
				break;
			case 1:
				endKnot = ofVec2f(endKnotB);
				break;
			case 2:
				endKnot = ofVec2f(endKnotC);
				break;
			case 3:
				endKnot = ofVec2f(endKnotD);
				break;
			default:
				// ----
				break;
			}


			//generieren der Abstände:
			float iteration = float(k) / float(numKnotsperPath);

			ofVec2f midToEndKnot((endKnot.x - mid.x),(endKnot.y - mid.y));

			ofVec2f step((midToEndKnot.x * iteration),(midToEndKnot.y * iteration));

			ofVec2f knot(mid.x + step.x, mid.y + step.y);

			if (k < numKnotsperPath) {
				knot.x = knot.x + ofRandom(-40, 90);
				knot.y = knot.y + ofRandom(-40, 90);
			}
			knots.push_back(knot);
		}
		paths.push_back(knots); 

		//copy path for branching
		vector<ofVec2f> pathsplit;
		pathsplit = paths[p];
		//
		for (int k = ((numKnotsperPath /2) + 1 ); k >= ((numKnotsperPath /2) + 1) && k < numKnotsperPath; k++) {
			pathsplit[k].x = pathsplit[k].x + ofRandom(-120, 160);
			pathsplit[k].y = pathsplit[k].y + ofRandom(-120, 160);
		}
		paths.push_back(pathsplit);	
	}

	for (int r = 0; r < paths.size(); r++) { //print all paths
		for (int k = 0; k < paths[r].size(); k++) {
			cout << "path: " << r << " knot: " << k << " x: " << paths[r][k].x << " y: " << paths[r][k].y << endl;
		}
	}
}
