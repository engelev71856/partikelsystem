#include "GenericFloatSensor.h"

void GenericFloatSensor::calibrate()
{
}

GenericFloatSensor::GenericFloatSensor(const std::string &name)
{
	_param.setName(name);
	setBounds(0,40);
	setValue(20);
}

GenericFloatSensor::~GenericFloatSensor()
{
}

ofAbstractParameter & GenericFloatSensor::getParam()
{
	return _param;
}

float GenericFloatSensor::getCurrentValue()
{
	return _param.get();
}

void GenericFloatSensor::setValue(float newVal)
{
	_param.set(newVal);
}

void GenericFloatSensor::setBounds(float min, float max)
{
	_param.setMin(min);
	_param.setMax(max);

	
}
