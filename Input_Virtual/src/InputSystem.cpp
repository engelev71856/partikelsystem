#include "InputSystem.h"

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::setup()
{
	//setup von Input, welche im header bereits initalisiert wurden:
	input1_Group.setup();
	input2_Group.setup();
	input3_Group.setup();
	inputSound.setup();

	//zufuegen parametergroup, setup gui:
	parameterGroup.add(input1_Group.input1_Parameter);
	parameterGroup.add(input2_Group.input2_Parameter);
	parameterGroup.add(input3_Group.input3_Parameter);
	//parameterGroup.add(input4_Group.input4_Parameter);
	gui.setup(parameterGroup);

	//label für moegliche Erlaeuterungen:
	gui.add(label.setup("label", "THIS IS A LABEL"));


	//setup('IP', PORT)
	inputInterfaceSender1.setup("localhost", INPUTINTERFACEPORT1);
	inputInterfaceSender2.setup("localhost", INPUTINTERFACEPORT2);
	inputInterfaceSender3.setup("localhost", INPUTINTERFACEPORT3);
	inputInterfaceSender4.setup("localhost", INPUTINTERFACEPORT4);

	//setup Test Receiver
	//testReceiver.setup(INPUTINTERFACEPORT);

	string title = "Input_Virtual";
	ofSetWindowTitle(title);
}

void InputSystem::update()
{
	//Wert der von Eva uebertragen werden soll
	intSliderSent1 = input1_Group.intSlider;
	intSliderSent2 = input2_Group.intSlider;
	intSliderSent3 = input3_Group.intSlider;
	//intSliderSent4 = input4_Group.intSlider;

	floatSliderSent1 = input1_Group.floatSlider;
	floatSliderSent2 = input2_Group.floatSlider;
	floatSliderSent3 = input3_Group.floatSlider;
	//floatSliderSent4 = input4_Group.floatSlider;

	clickerSent1 = input1_Group.clicker;
	clickerSent2 = input2_Group.clicker;
	clickerSent3 = input3_Group.clicker;
	//clickerSent4 = input4_Group.clicker;

	//OSC-Message Setup
	ofxOscMessage inputInterfaceMessage1, inputInterfaceMessage2, inputInterfaceMessage3, inputInterfaceMessage4;

	//Sendeadresse definieren - !!! Muss mit Receiver uebereinstimmen !!!
	inputInterfaceMessage1.setAddress("/inputsensor/values1");
	inputInterfaceMessage2.setAddress("/inputsensor/values2");
	inputInterfaceMessage3.setAddress("/inputsensor/values3");
	inputInterfaceMessage4.setAddress("/inputsensor/values4");

	//Hinzufuegen des zu uebertragenden Wertes
	inputInterfaceMessage1.addIntArg(intSliderSent1);
	inputInterfaceMessage2.addIntArg(intSliderSent2);
	inputInterfaceMessage3.addIntArg(intSliderSent3);
	inputInterfaceMessage4.addIntArg(intSliderSent4);

	inputInterfaceMessage1.addFloatArg(floatSliderSent1);
	inputInterfaceMessage2.addFloatArg(floatSliderSent2);
	inputInterfaceMessage3.addFloatArg(floatSliderSent3);
	inputInterfaceMessage4.addFloatArg(floatSliderSent4);

	inputInterfaceMessage1.addBoolArg(clickerSent1);
	inputInterfaceMessage2.addBoolArg(clickerSent2);
	inputInterfaceMessage3.addBoolArg(clickerSent3);
	inputInterfaceMessage4.addBoolArg(clickerSent4);

	//Versenden der Nachricht
	inputInterfaceSender1.sendMessage(inputInterfaceMessage1);
	inputInterfaceSender2.sendMessage(inputInterfaceMessage2);
	inputInterfaceSender3.sendMessage(inputInterfaceMessage3);
	inputInterfaceSender4.sendMessage(inputInterfaceMessage4);

	//Abgreifen der empfangengen OSC-Message
	/*while (testReceiver.hasWaitingMessages()) {
		ofxOscMessage msg;
		testReceiver.getNextMessage(msg);
		if (msg.getAddress() == "/inputsensor/value") {
			intSliderReceived = msg.getArgAsInt(0);
			floatSliderReceived = msg.getArgAsFloat(1);
			clickerReceived = msg.getArgAsBool(2);
		}
	}
	*/
}

void InputSystem::draw()
{
	//draw input und gui:
	input1_Group.draw();
	input2_Group.draw();
	input3_Group.draw();
	inputSound.draw();
	gui.draw();

	/*
ofDrawBitmapString("Value received - IntSlider = " + ofToString(intSliderReceived, 1), 50, 200);
ofDrawBitmapString("Value received - FloatSlider = " + ofToString(floatSliderReceived, 4), 50, 250);
ofDrawBitmapString("Value received - Clicker = " + ofToString(clickerReceived, 0), 50, 300);
*/

}

void InputSystem::audioIn(ofSoundBuffer & inBuffer)
{
	//std::cout << "ofApp" << std::endl;
	//inputSound.audioIn(inBuffer);
}
