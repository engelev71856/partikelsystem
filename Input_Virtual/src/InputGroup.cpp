#include "InputGroup.h"
#include "GenericFloatSensor.h"

void InputGroup::update()
{
}

 void InputGroup::calibrate(int sensld)
{
	 for (auto s : sensList) {
		 if (s->id == sensld) {
			 GenericFloatSensor *gfs = dynamic_cast<GenericFloatSensor*>(s.get()); 
			 if (gfs) {
				 gfs->calibrate();
			 }
			 break;
		 }
	 }
}

 void InputGroup::setup()
 {
	 auto addfloatSensor = [&](const char* name) {
		 sensList.push_back(std::shared_ptr<Sensor>(new GenericFloatSensor(name)));
	 }; //hilfsfunktion mit lambda
	 addfloatSensor("sensor1");
	 addfloatSensor("sensor2");
	 addfloatSensor("sensor3");

	 ofParameterGroup parametergroup;
	 for (auto s : sensList) {
		 parametergroup.add(s->getParam()); 
	 }
	 gui.setup(parametergroup);
 }

 void InputGroup::draw()
 {
	 gui.draw();
 }
