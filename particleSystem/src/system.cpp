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
	//generiere mash gui
	//parameterGroup.add(uiAmount.set("amount", 0, 0, 6));
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


	ofVec2f endKnotA(10,10);
	ofVec2f endKnotB(600, 10);
	ofVec2f endKnotC(450, 10);
	ofVec2f endKnotD(ofGetWidth()-10, 10);
	ofVec2f endKnotE(10, ofGetHeight()-10);
	ofVec2f endKnotF(ofGetWidth() - 10, ofGetHeight() - 10);
	ofVec2f endKnotG(ofGetWidth() - 390, ofGetHeight() - 10);

	endpointList.push_back(endKnotA);
	endpointList.push_back(endKnotB);
	endpointList.push_back(endKnotC);
	endpointList.push_back(endKnotD);
	endpointList.push_back(endKnotE);
	endpointList.push_back(endKnotF);
	endpointList.push_back(endKnotG);


	//load image 
	emitterImage.load("images/ahorn2.jpg");
	//generate emitterlist
	emitterList = image2List(&emitterImage);
	//generate random attractor once
	generateAttractors(numKnots, endpointList);

	//generate MeshVertices
	//generateMesh(&emitterImage);
	//cam.setDistance(100);

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
		generateAttractors(numKnots, endpointList);
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
						particles[i]->attractor = ofVec2f(paths[particles[i]->pathId][particles[i]->knotId]);
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

	/*//update Bild mash
	int count = 0;

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			ofVec3f position = mesh.getVertex(count);
			position.z = ofMap(ofNoise(count, ofGetElapsedTimef()), 0, 1, 0, uiAmount);
			mesh.setVertex(count, position);
			count++;
		}
	}
	*/

}

//-------------------------------------------------------------------

void ParticleSystem::draw()
{
	ofBackground(0); 
	ofFill();
	if (drawKnots) {
		for (int p = 0; p < paths.size() / 2; p++) {
			for (int k = 0; k < paths[p].size(); k++) {
				ofDrawCircle(paths[p][k], 2);
			}
		}
	}

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

	gui.draw();

	ofSetColor(255, 0, 255);
	ofDrawBitmapString("NumParticles: " + ofToString(particles.size()), 10, 10);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()), 10, 20);
	ofSetColor(255);

	//Matrix draw
	/*
	ofPushMatrix(); //speichere in Koordinatensystem
	cam.begin();
	//ofNoFill();
	ofSetColor(255);
	mesh.drawWireframe();
	cam.end();
	ofPopMatrix(); // Wiederherstellen
	gui.draw();
	*/

}

//-----------------------------------------------------------------------------

vector<ofVec2f> ParticleSystem::image2List(ofImage* img)
{
	vector<ofVec2f> list;

	//get Image properties
	ofPixels & pixels = img->getPixels();

	int x_bild = pixels.getWidth(); //x_bild
	int y_bild = pixels.getHeight();//y_bild

	int bpp = pixels.getBytesPerPixel();

	//interate through image and get values
	for (int y_partikel = 0; y_partikel < y_bild; y_partikel++) {
		for (int x_partikel = 0; x_partikel < x_bild; x_partikel++) {

			ofVec3f col(pixels[(x_partikel + (y_partikel * x_bild)) * 3 + 0, (x_partikel + (y_partikel * x_bild)) * 3 + 1, (x_partikel + (y_partikel * x_bild)) * 3 + 2]);

			if (col.length()) {//any color except black
				list.emplace_back(x_partikel, y_partikel);

			}
		}
	
	}
	if ((x_bild < ofGetWidth()) && (y_bild < ofGetHeight())) {
		for (int i = 0; i < list.size(); i++) {
			list[i].x += (ofGetWidth() - x_bild) / 2;
			list[i].y += (ofGetHeight() - y_bild) / 2;
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

			//ofVec2f step((midToEndKnot.x * iteration),(midToEndKnot.y * iteration));
			//ofVec2f knot(mid.x + step.x, mid.y + step.y);

			ofVec2f step = mid.getInterpolated(endKnot, iteration);
			ofVec2f knot(step.x, step.y);

			/*if (k < numKnotsperPath) {
				knot.x = knot.x + ofRandom(-20, 60);
				knot.y = knot.y + ofRandom(-20, 60);
			}*/
			knots.push_back(knot);
		}
		paths.push_back(knots);
	}

	for (int p = 0; p < endpoints.size(); p++) {
		//copy path for branching
		//left
		vector<ofVec2f> pathsplit;
		pathsplit = paths[p];
		//---------------------------------------------------------------------
		for (int k = pathsplit.size() - 2; k < pathsplit.size(); k++) {
			ofVec2f r(pathsplit[k - 1].x, pathsplit[k - 1].y);
			pathsplit[k].rotate(25, r);
		}
		//copy path for branching
		//right
		vector<ofVec2f> pathsplit2;
		pathsplit2 = paths[p];
		//
		for (int k = pathsplit.size() - 2; k < pathsplit.size(); k++) {

			ofVec2f r(pathsplit2[k - 1].x, pathsplit2[k - 1].y);
			pathsplit2[k].rotate(-35, r);
		}

		paths.push_back(pathsplit);
		paths.push_back(pathsplit2);
	}

	cout << numPaths << "_" << numKnotsperPath << endl;

	for (int r = 0; r < paths.size(); r++) { //print all paths
		for (int k = 0; k < paths[r].size(); k++) {
			cout << "path: " << r << " knot: " << k << " x: " << paths[r][k].x << " y: " << paths[r][k].y << endl;
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------
	void ParticleSystem::generateMesh(ofImage* img)
	{
		//vector<ofVec2f> MeshList;

		//get Image properties
		ofPixels & pixels = img->getPixels();

		//interate through image and get values
		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {
				mesh.addVertex(ofPoint(x - size / 2, y - size / 2));
				//MeshList.emplace_back(ofPoint(x - size / 2, y - size / 2));
			}

		}

		for (int x = 0; x < size - 1; x++) {
			for (int y = 0; y < size - 1; y++) {
				mesh.addIndex(x + y * size);
				mesh.addIndex((x + 1) + y * size);
				mesh.addIndex(x + (y + 1) * size);
				mesh.addIndex((x + 1) + y * size);
				mesh.addIndex((x + 1) + (y + 1) * size);
				mesh.addIndex(x + (y + 1) * size);
			}
		}

}
