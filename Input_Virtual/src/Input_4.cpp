//SOUND INPUT VIA MICRO
//https://openframeworks.cc/documentation/sound/
#include<iostream>


#include "Input_4.h"

Input_4::Input_4()
{
}

Input_4::~Input_4()
{
}

void Input_4::setup() {
	ofSoundStreamSetup(4, 2, 44100, 256, 3);
	//number of output channels, inputchannels we want, samplerate, buffersize, number of soundbuffers which soundcards use for recording screams>less number of buffers/faster responses
	//output channels an anzahl der klassen anpassen! wichtig
}


void Input_4::draw() {

	ofSetColor(225);
	ofDrawBitmapString("Audio Cube:", 20, 350);

	ofDrawBitmapStringHighlight("volume: " + ofToString(volume, 3), 20, 370);
}
void Input_4::audioIn(ofSoundBuffer &inBuffer)
{
	//std::cout << "input_4" << std::endl;
	volume = inBuffer.getRMSAmplitude();
	//std::cout << volume << std::endl;
	//rootmeansquere is typical measure for estimating volume's sound, output in draw
}