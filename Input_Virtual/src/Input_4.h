//Micro is used for Input!
//#pragma once
#ifndef _passingParametersToClass_Input_4_
#define _passingParametersToClass_Input_4_



#include"ofMain.h"

class Input_4
{
public:
	Input_4();
	~Input_4();

	void setup();
	void draw();

	void audioIn(ofSoundBuffer &inBuffer); //für Audio
	float volume = 0;

};


#endif