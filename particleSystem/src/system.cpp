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
	particles.clear();

	

	//Gui generierung
	//parameterGroup.add(rate.set("rate", 1,0,10));
	//parameterGroup.add(lifeTime.set("lifetime", 8.5, 0, 25));
	//parameterGroup.add(minSpeed.set("min speed", 0.1, 0, 1));
	//parameterGroup.add(maxSpeed.set("max speed", 0.055, 0, 1));
	//parameterGroup.add(ratio.set("vel ratio", 0.53, 0, 1));
	//parameterGroup.add(distanceThreshold.set("distance threshold", 39, 0, 100));
	
	parameterGroup.add(numPaths.set("num paths", 0, 1, 20));
	parameterGroup.add(numKnots.set("num knots", 8, 1, 10));
	parameterGroup.add(randomize.set("randomize", true));
	parameterGroup.add(generateAttractor.set("generate attractor", false));
	parameterGroup.add(useAttractor.set("use attractor", false));
	parameterGroup.add(drawKnots.set("draw Knots", false));
	parameterGroup.add(fader.set("fade particles", 0.755, 0,1));
	

	//feste Werte aus Gui
	rate = 1;
	lifeTime = 13.375;
	minSpeed = 0.025;
	maxSpeed = 0.035;
	ratio = 0.05;
	distanceThreshold = 0;

	parameterGroup.add(numSplitlists.set("num spits", 2, 0, 20));
	parameterGroup.add(splitSlider.set("splitknot position", 0,  0, 3));
	gui.setup(parameterGroup);

	int w = ofGetWidth();
	int h = ofGetHeight();
	fbo.allocate(w, h, GL_RGB32F_ARB);

	fbo.begin();
	fbo.end();
	

	ofVec2f endKnotA(200, -10);
	ofVec2f endKnotB(950, -70);
	ofVec2f endKnotC(1700, -10);
	ofVec2f endKnotD(200, ofGetHeight()+10);
	ofVec2f endKnotE(800, ofGetHeight()+100);
	ofVec2f endKnotF(1700, ofGetHeight()+10);

	endpointList.push_back(endKnotA);
	endpointList.push_back(endKnotB);
	endpointList.push_back(endKnotC);
	endpointList.push_back(endKnotD);
	endpointList.push_back(endKnotE);
	endpointList.push_back(endKnotF);


	//load image 
	emitterImage.load("images/baumringe.jpg");
	//generate emitterlist
	emitterList = image2List(&emitterImage);
	//generate random attractor once
	generateAttractors(numKnots, endpointList);
	
	

}

//-------------------------------------------------------------------

void ParticleSystem::update()
{
	//fix values
	if (useAttractor) {
		lifeTime = 13.125;
		minSpeed = 0.035;
		maxSpeed = 0.095;
		ratio = 0.38;
		distanceThreshold = 68;
	}

	/*
	if (ofGetElapsedTimef() > 6) {
		maxSpeed = 0.45;
		ratio = 0.65;
	}
	*/


	// Parameter timestep: the time in seconds between the current and the previous callings of this function.
	// Will be used for Particle:update(float timestep) Methode
	//Compute timestep
	timeNow = ofGetElapsedTimeMillis();
	timestep = timeNow - time0;
	time0 = timeNow;

	//calculate number of new particles
	numNewParticles = round(timestep * rate);

	if (useAttractor) {
		numNewParticles = round((timestep * rate)/2.0);
	}

	if (generateAttractor.get()) {
		generateAttractors(numKnots, endpointList);
		generateAttractor = false;
	}
	
	//update old particles
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->update(timeNow, timestep, ratio, distanceThreshold);
		if (useAttractor.get()) {
					if (particles[i]->wantNextAttractor) {
						if (particles[i]->knotId + 1 < paths[particles[i]->pathId].size()) { // is there one more knot in this path?
							particles[i]->knotId++;
						}
						ofVec2f att(paths[particles[i]->pathId][particles[i]->knotId]);
						particles[i]->attractor = ofVec2f(att.x + ofRandom(-1,1), att.y + ofRandom(-1, 1)); //random +-, damit Pfade nicht so eng sind, sondern breiter
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
		newParticle->setup(emitterList[ofRandom(emitterList.size() - 1)], ofVec2f(minSpeed, maxSpeed), lifeTime);
		newParticle->setAttractor(ofRandom(paths.size()));
		//
		particles.push_back(newParticle);
	}

	emitterList = image2List(&emitterImage);
}

//-------------------------------------------------------------------

void ParticleSystem::draw()
{
	ofBackground(0); 
	ofFill();
	
	fbo.begin();
	ofEnableAlphaBlending();
	float alpha = (1 - fader) * 255;
	ofSetColor(0, 0, 0, alpha);
	ofFill();
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	ofDisableAlphaBlending();


	ofNoFill();
	for (int i = 1; i <= numKnots; i++) {
		ofSetColor(50, 70, 108);
		if (useAttractor == true) { ofSetColor(48);}
		ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, ((float)i / numKnots)*(ofGetWidth() / 2));

	}

	ofFill();
	ofEnableAlphaBlending();
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->draw();
	}
	ofDisableAlphaBlending();
	fbo.end();
	//ofSetColor(255, 255, 255);
	fbo.draw(0, 0);

	gui.draw();

	ofFill();
	if (drawKnots) {
		for (int p = 0; p < paths.size(); p++) {
			for (int k = 0; k < paths[p].size(); k++) {
				ofDrawCircle(paths[p][k], 2);
			}
		}
	}

	ofSetColor(255, 0, 255);
	ofDrawBitmapString("numNewParticles: " + ofToString(numNewParticles), 10, 10);

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
//void ParticleSystem::generateAttractors(int numPaths, int numKnotsperPath, vector<ofVec2f> endpointList)

void ParticleSystem::generateAttractors(int numKnotsperPath, vector<ofVec2f> endpoints)
{
	//liste mit Unterpfaden 
	paths.clear();
	//int si_l = endpoints.size() * 3;

	ofVec2f mid(ofGetWidth() / 2, ofGetHeight() / 2);

	for (int p = 0; p < endpoints.size(); p++) {
		vector<ofVec2f> knots;

		ofVec2f endKnot;

		for (int k = 0; k < numKnotsperPath; k++) {

			int testPath = p % endpoints.size();
			endKnot = endpoints[testPath];


			//generieren der Abstände:
			float iteration = float(k) / float(numKnotsperPath);

			ofVec2f midToEndKnot((endKnot.x - mid.x), (endKnot.y - mid.y));

			ofVec2f step = mid.getInterpolated(endKnot, iteration);
			ofVec2f knot(step.x, step.y);


			if (randomize) {
				//fixierter Endpoint, damit die Partikel zum Bildrand gelangen können 
				if (k < numKnotsperPath-1 && k > 0) {
					ofVec2f pri(knots[k - 1].x, knots[k - 1].y);
					knot.rotate(ofRandom(-40, 40), pri);
				}
			}
		
			knots.push_back(knot);
		}

		
		paths.push_back(knots);
	}

	for (int s = 0; s < numSplitlists; s++) {
		for (int p = 0; p < endpoints.size(); p++) {

			//copy path for branching
			//left
			vector<ofVec2f> pathsplit;
			pathsplit = paths[p];

			//---------------------------------------------------------------------
			for (int k = pathsplit.size() / 2 + splitSlider; k < pathsplit.size(); k++) {
				ofVec2f r(pathsplit[k - 1].x, pathsplit[k - 1].y);
				pathsplit[k].rotate(ofRandom(5, 55), r);
			}

			//copy path for branching
			//right

			vector<ofVec2f> pathsplit2;
			pathsplit2 = paths[p];

			for (int k = pathsplit.size() / 2 + splitSlider; k < pathsplit.size(); k++) {

				ofVec2f r(pathsplit2[k - 1].x, pathsplit2[k - 1].y);
				pathsplit2[k].rotate(ofRandom(-55, -5), r);
			}

			paths.push_back(pathsplit);
			paths.push_back(pathsplit2);

		}
	}

	cout << numPaths << "_"<<numKnotsperPath << endl;

	for (int r = 0; r < paths.size(); r++) { //print all paths
		for (int k = 0; k < paths[r].size(); k++) {
			cout << "path: " << r << " knot: " << k << " x: " << paths[r][k].x << " y: " << paths[r][k].y << endl;
		}
	}
}