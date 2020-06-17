//#pragma once
#ifndef _passingParametersToClass_Input_3_
#define _passingParametersToClass_Input_3_

#include"ofMain.h"


class Input_3
{
public:
	Input_3();
	~Input_3();

	void setup();
	void draw();

	ofParameterGroup input3_Parameter;

	ofParameter<int> intSlider;
	ofParameter<float> floatSlider;
	ofParameter<bool> clicker;


};


#endif