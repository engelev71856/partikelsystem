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
	//Liste erzeugen zufällig:
	ebenen.resize(ebenenanzahl);
	constexpr int knotenanzahl = 50;
	for (int i = 0; i < 50; i++) {
		Knoten k;
		//zufallsgenerator
		k.pos.x = ofRandom(0, ofGetWidth());
		k.pos.y = ofRandom(0, ofGetHeight());
		//radiusebene, Abstand zur Mitte berechnen
		const float dx = k.pos.x - ofGetWidth() / 2;
		const float dy = k.pos.y - ofGetHeight() / 2;
		/*const float d = sqrt(dx*dx + dy*dy);*/
		const float d = hypot(dx, dy);
		//Ebenen festlegen:
		//constexpr int  ebenenanzahl = 6;
		const int ebene = d / (ofGetWidth() / 2) * ebenenanzahl;
		if (ebene >= ebenenanzahl) {
			i--;
			continue;
		}

		k.ebene = ebene;
		knoten.push_back(k);

		Ebene::Knoten k2;
		k2.pos = k.pos;
		ebenen[ebene].knoten.push_back(k2);
	}

	//prüfuen elternebene/kindebene
	/*for (int i = ebenenanzahl - 1; i >= 1; i--) {
		for (Ebene::Knoten &k : ebenen[i].knoten) {
			Ebene::Knoten *kBest = NULL;
			float bestDist = FLT_MAX;
			for (Ebene::Knoten &k2 : ebenen[i - 1].knoten) {
				const ofVec2f diffVec = k.pos - k2.pos;
				const float dist = hypot(diffVec.x, diffVec.y);
				if (dist < bestDist) {
					bestDist = dist;
					kBest = &k2;
				}
			}
			k.parent = kBest;
			kBest->children.push_back(&k);
		}
	}
	for (Ebene& e : ebenen) {
		for (int i = 0; i < e.knoten.size();) {
			if (e.knoten[i].children.empty()){
				e.knoten.erase(e.knoten.begin()+i);
			} else {
				i++;
			}
		}
	}*/

	//attractor liste=berechnung wohin er sich bewegen muss, soll in update-methode
	//Normierung eines Vektors, 
	//Stelle der einen Liste ID 
	particles.clear();
	parameterGroup.add(activateSystem.set("clicker", true));
	gui.setup(parameterGroup);

	// Bild einlesen 
	dotsPicture.load("images/doublefalse.png"); //true
	falsePicture.load("images/false.png"); //false, aktiveiert

// emitter und attrocotrliste generieren statt lamda ist hilfunktion? 
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

	if (activateSystem.get()) {
		for (Particle *p : particles) {
			ofVec2f fSum{}; //objekt auf null
			bool ebenenknotenTouched = false;
			for (const Knoten &k : knoten) {
				if (k.ebene == p->ebene) {
					const ofVec2f diffVec = k.pos - p->getPos();
					const float d = hypot(diffVec.x, diffVec.y);
					const float f = 0.01 / d; //gravity,kraft, Richtung Knoten
					fSum += diffVec.getNormalized() * f; //Differnenz richtung Knoten

					if (d < knotenradius) {
						ebenenknotenTouched = true;
					}
				}
			}
			p->updateVel(fSum, timestep);
			
			if (ebenenknotenTouched) {
				p->ebene++;
			}
		}

	}else{

		//http://cplusplus.com/reference/random/
		const vector<ofVec2f> & list = activateSystem.get() ? dotsEmitterList : falseEmitterList;
		uniform_int_distribution<int> distribution(0, list.size()-1);
		const int count = lround(list.size() * 0.00001 * timestep); 
		for (int i = 0; i < count; i++) {
			const int dice_roll = distribution(generator);  // generates number in the range 0..list 
			Particle* newParticle = new Particle();
			newParticle->setup(list[dice_roll], activateSystem.get());
			particles.push_back(newParticle);
		}
	}
}
//ofrandom gib wert  0 und anzahl der partikel in der liste und diesem Wert um Koordinate dauraus zu erzegeugen --> gleiches für attractor id nutzten um 
//in attraktorliste zu schauen: wo wollen wir eigentlich hin --> System durchgehen. id 500 ^000 ^000 --- > muss berechnentn in welche Richtung er muss 
//liste erzeugen punkt für punkt 

void ParticleSystem::draw()
{
	ofBackground(0); 
	//dotsPicture.draw(0, 0);
	//knoten zeichnen zur kontrolle:
	ofNoFill();
	ofSetColor(48);
	for (int i = 1; i <= ebenenanzahl; i++) {
		ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, ((float)i / ebenenanzahl)*(ofGetWidth() / 2));

	}
	ofFill();
	for (const Knoten &k : knoten) {
		ofSetColor(k.ebene % 2 ? 128 : 64);
		ofDrawCircle(k.pos, knotenradius);
		
	}
	
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->draw();
	}
	gui.draw();
	ofSetColor(255, 0, 255);
	ofDrawBitmapString("NumParticles: " + ofToString(particles.size()), 10, 10);
	ofSetColor(255);

	//frame count 

		
}

