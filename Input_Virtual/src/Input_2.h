//#pragma once
#ifndef _passingParametersToClass_Input_2_
#define _passingParametersToClass_Input_2_

#include"ofMain.h"


class Input_2
{
public:
	Input_2();
	~Input_2();

	void setup();
	void draw();

	ofParameterGroup input2_Parameter;

	ofParameter<int> intSlider;
	ofParameter<float> floatSlider;
	ofParameter<bool> clicker;


};


#endif
