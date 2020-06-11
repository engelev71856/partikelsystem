#include "system.h"


//-------------------------------------------------------------------
ParticleSystem::ParticleSystem()
	: generator(time(0)) // wird Konstruktor der memberklasse aufgerufen, generator=memberobjekt
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
	//attractor liste=berechnung wohin er sich bewegen muss, soll in update-methode
	//Normierung eines Vektors
	particles.clear();
	parameterGroup.add(activateSystem.set("clicker", true));
	gui.setup(parameterGroup);

	// Bild einlesen 
	dotsPicture.load("images/doublefalse.png"); //true
	falsePicture.load("images/false.png"); //false, aktiveiert

// emitter und attrocotrliste generieren statt lamda? 
	//lambda Capturelist in eckigeKlammer ist leer, lokale Funktion nur kurz genutzt=Hilfsfunktion
	auto genList = [](const ofImage & img) {
		vector<ofVec2f> list;
		const ofPixels & pixels = img.getPixels();
		const int w = pixels.getWidth();
		const int h = pixels.getHeight();
		const int bpp = pixels.getBytesPerPixel();
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				if (pixels[bpp * (x + y * w)] > 128) {
					//list.push_back(ofVec2f(x, y)); //erzeugt objektkopie durch -Copyconstructor
					list.emplace_back(x, y); //erzeugt obj durch Konstructor des Templatestypes (hier ofvec) mit gegebn param 
				}
			}
		}
		return list;
	};
	dotsEmitterList = genList(dotsPicture);
	falseEmitterList = genList(falsePicture);
	cout << dotsEmitterList.size() << "," << falseEmitterList.size() << endl;
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

	/*ofPixels & pixels = dotsPicture.getPixels();*/


	//!!!
	//const ofPixels & pixels = (activateSystem.get()? dotsPicture:falsePicture).getPixels(); //ternäre Operator 
	//const int w = pixels.getWidth();
	//const int h = pixels.getHeight();
	//!!!

	//for (int i = 0; i <  pixels.size(); i += 3) {

	//		//rot = index + 0
	//	//grün = index + 1
	//	//blau = index + 3

	//	/*float x;
	//	float y;*/

	//	if (pixels[i] > 0) {
	//		const int index = i / 3; //variable wird nur einmal geschrieben!
	//		/*if (index < w) {
	//			y = 1;
	//			x = index;
	//		}*/
	//		//if (index > w) {
	//			//int result = index % w;
	//		const int x = index % w;
	//			//y = round(index / w); //bei cpp wird immer ganzzahldiv gemacht, wenn beide ganzzahlen sind, im Kontrast zu javascript oder andere nicht streng typisierte scriptsprachen
	//		const int y = index / w;
	//		/*}*/
	//		ofVec2f v;
	//		v.set(x, y);

	//		Particle* newParticle = new Particle();
	//		newParticle->setup(v);
	//		particles.emplace_back(newParticle);

	//	}
	//}

	//!!!
	//for (int y = 0 ; y < h; y+=10) {
	//	for (int x = 0 ; x < w; x+=10) {
	//		if (pixels[3 * (x + y*w)] > 0)  {
	//							//const ofVec2f v(x,y);  //constructor
	//			Particle* newParticle = new Particle();
	//			newParticle->setup(ofVec2f(x,y));//übergabe, deshalb keine variabel und 131 kann weg
	//			particles.push_back(newParticle);
	//		}
	//	}
	//}
	//!!!

	//http://cplusplus.com/reference/random/
	const vector<ofVec2f> & list = activateSystem.get() ? dotsEmitterList : falseEmitterList;
	uniform_int_distribution<int> distribution(0, list.size()-1);
	const int count = lround(list.size() * 0.0001 * timestep); 
	for (int i = 0; i < count; i++) {
		const int dice_roll = distribution(generator);  // generates number in the range 0..list 
		Particle* newParticle = new Particle();
		newParticle->setup(list[dice_roll], activateSystem.get());
		particles.push_back(newParticle);
	}
}
//ofrandom gib wert  0 und anzahl der partikel in der liste und diesem Wert um Koordinate dauraus zu erzegeugen --> gleiches für attractor id nutzten um 
//in attraktorliste zu schauen: wo wollen wir eigentlich hin --> System durchgehen. id 500 ^000 ^000 --- > muss berechnentn in welche Richtung er muss 
//liste erzeugen punkt für punkt 

void ParticleSystem::draw()
{
	ofBackground(0); 
	//dotsPicture.draw(0, 0);
	
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->draw();
	}
	gui.draw();
	ofSetColor(255, 0, 255);
	ofDrawBitmapString("NumParticles: " + ofToString(particles.size()), 10, 10);
	ofSetColor(255);

	//frame count 

}