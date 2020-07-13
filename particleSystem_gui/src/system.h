#pragma once
#include "ofMain.h"
#include "particle.h"
#include "ofxGui.h"


class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void setup();
	void update();
	void draw();
	
private:

	///particles
	//Liste aus Particle Objekten 
	vector<Particle*> particles;
	///emitterPos: Entstehungsposition des einzelnen Partikels
	ofVec2f emitterPos; 
	
	//numNewParticles: Anzahl der neu zu erstellenden Partikel
	float numNewParticles;
	
	///attractor:
	//Anziehungspunkt bei Status inaktiv: Emitterposition
	//Anziehungspunkt bei Status aktiv: KnotId Abfrage von einzelnen Pfadknotenpunkten
	ofVec2f attractor;

	///Gui Partikelwerte 
	///Im folgenden werden die veränderbaren Parameter zur Steuerung/Einstellung als GUI angezeigt. 
	///Dies hilft dabei den visuellen Eindruck abzuändern und die Werte schließlich ohne GUI in der Endversion festzulegen. 
	//rate: Wert zur Berechnung für die Anzahl der Partikel, die über einen bestimmten Zeitraum neu erstellt werden sollen
	ofParameter<int> rate;
	ofParameter<float> lifeTime;  //lifeTime: "Lebenszeit" der Partikel
	ofParameter<float> minSpeed;  //minSpeed: Minimale Geschwindigkeit der Partikel
	ofParameter<float> maxSpeed;  //maxSpeed: Maximale Geschwindigkeit der Partikel
	ofParameter<float> ratio;   //ratio: 
	ofParameter<int> distanceThreshold; //distanceThereshold: Nähe zum folgenden Knotenpunkt 

	///Gui Systemwerte
	ofParameter<bool> generateAttractor; //generateAttractor: Liste aus Anziehungspunkt (=Attractor) wird neu generiert
	ofParameter<bool> useAttractor;  //useAttractor: Partikelsystem wird aktiviert, d.h. Partikel bewegen sich zu den Anziehungspunkten
	ofParameter<int> numKnots; //numKnots: Anzahl der Verzweigungspunkte, d.h. Punkte ab denen Partikel die Richtung ändern auf ihrem Path nach außen
	ofParameter<bool> randomize; //randomize: Zufallsgenerator für Parameterwerte
	ofParameter<int> numSplitlists; //numSplitlists: Anzahl der Verzweigungen von einem Pfad
	ofParameter<int> splitSlider; //splitSlider: Punkt ab dem sich die Wurzelstränge (Partikelpade) auf jeweiligem Wurzelstrang (Path) verzweigt
	ofParameter<float> fader; //fader: verblassen der Partikelfarbe 

	ofParameterGroup parameterGroup;
	ofxPanel gui;
	
	ofFbo fbo;

	///timing
	float time0; //time0: Startzeitpunkt für die aktuelle Berechnung der vergangenen Zeit 
	float timeNow; //timeNow: jetziger Zeitpunkt 
	float timestep; //timestep: abgelaufende Zeit von time0 bis timeNow 

	///emitter
	ofImage emitterImage;
	//emitterList: Liste der Entstehungspunkte, die über das emitterImage herausgelesen werden
	//wird durch die Methode vector<ofVec2f> image2List(ofImage* img); erstellt
	vector<ofVec2f> emitterList;
	
	///attractor
	vector<ofVec2f> endpointList;
	//paths: wird durch die Methode generateAttractors(int numKnotsperRing, vector<ofVec2f> endpoints); befüllt 
	vector<vector<ofVec2f>> paths;

	///helper method
	vector<ofVec2f> image2List(ofImage* img);
	void generateAttractors(int numKnotsperRing, vector<ofVec2f> endpoints);	
};