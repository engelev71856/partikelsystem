#pragma once
#include "Sensor.h"
#include "ofxGui.h"
#include <vector>
#include <memory>


class InputGroup
{
public:
	void update();
	void calibrate(int sensld);
	void setup();
	void draw();

private:
	std::vector<std::shared_ptr<Sensor>> sensList;

	
	//	OSCHandler osc;
	int initPort;
	int id;
	ofxPanel gui;


};

