#pragma once

#include "ofMain.h"
#include "InputSystem.h"
#include "InputGroup.h"


#include "ofxGui.h"
#include "ofxOsc.h"


//
//#include "Input_1.h"
//#include "Input_2.h"
//#include "Input_3.h"
//#include "Input_4.h"
//
//
//#define INPUTINTERFACEPORT1 8888 //Port fuer input1_Group
//#define INPUTINTERFACEPORT2 7777 //Port fuer input2_Group
//#define INPUTINTERFACEPORT3 6666 //Port fuer input3_Group
//#define INPUTINTERFACEPORT4 5555 //Port fuer input4_Group

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void audioIn(ofSoundBuffer &inBuffer);

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	////setting-up gui:
	//ofxPanel gui;
	//ofxLabel label;


	////Sender zum Uebertragen der Sensorwerte von InputVirtual zu BrainUnit
	//ofxOscSender inputInterfaceSender1, inputInterfaceSender2, inputInterfaceSender3, inputInterfaceSender4;
	////ofxOscReceiver testReceiver;

	///* TODO rebase for 4 Ins*/

	////Gesendete Parameter
	//int intSliderSent1, intSliderSent2, intSliderSent3, intSliderSent4;
	//float floatSliderSent1, floatSliderSent2, floatSliderSent3, floatSliderSent4;
	//bool clickerSent1, clickerSent2, clickerSent3, clickerSent4;

	////Empfangene Parameter
	////int intSliderReceived;
	////float floatSliderReceived;
	////bool clickerReceived;

	////input initialisierung:
	//ofParameterGroup parameterGroup;
	//Input_1 input1_Group;
	//Input_2 input2_Group;
	//Input_3 input3_Group;
	Input_4 inputSound;

	InputSystem inputsystem;
	InputGroup inputGroup;

};