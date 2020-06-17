//#pragma once
#ifndef _passingParametersToClass_Input_1_
#define _passingParametersToClass_Input_1_

#include"ofMain.h"

/*Erläuterung: In dieser Klasse wir eine ofParametergroup (input1_Parameter)erstellt
Im Header (Input_1.h) wird sie initalisiert
und im Input_2.cpp wird die Funktion implementiert
*/

class Input_1
{
public:
	Input_1();
	~Input_1();

	void setup();
	void draw();

	//initieren einer ParameterGruppe, ofParameterGroup, mit int, float, boolean:
	ofParameterGroup input1_Parameter;

	ofParameter<int> intSlider;
	ofParameter<float> floatSlider;
	ofParameter<bool> clicker;
};


#endif
