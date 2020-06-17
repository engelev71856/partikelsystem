#pragma once

#include "ofParameter.h"
class Sensor
{
public:
	int id;
	
	virtual ofAbstractParameter &getParam() = 0;//rein abstrakt
	virtual ~Sensor();
};
//
//class LichtSensor : public Sensor {
//	virtual int getValue() {
//		return 6;
//	}
//	void test() {
//
//	}
//};

//int main() {
//	Sensor *s1 = new Sensor;
//	Sensor *s2 = new LichtSensor; //heap 
//	s1->getValue();
//	s2->getValue();
//
//	if (dynamic_cast<LichtSensor*>(s1)) {
//		std::cout << "es war eine Lichtsensor" << std::endl;
//	} else {
//		std::cout << "es war kein Lichtsensor" << std::endl;
//	}
//delete s1;
//delete s2; ---
//}