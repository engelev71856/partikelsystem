#pragma once
#include"ofMain.h"
#include "Sensor.h"

class GenericFloatSensor : public Sensor
{
public:
	void calibrate();

	GenericFloatSensor(const std::string &name);
	virtual ~GenericFloatSensor(); // virtual, dass auch im Falle von delete-Elternklasse aufgerufen wird

	 ofAbstractParameter &getParam();
	float getCurrentValue();
	void setValue(float newVal);
	void setBounds(float min, float max);

private:
	ofParameter<float> _param;


};


