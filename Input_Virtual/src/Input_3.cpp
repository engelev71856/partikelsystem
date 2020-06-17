#include "Input_3.h"

Input_3::Input_3()
{
}

Input_3::~Input_3()
{
}

void Input_3::setup() {


	input3_Parameter.setName("Particel_Cube");

	input3_Parameter.add(intSlider.set("int slider", 0, 0, 100));
	input3_Parameter.add(floatSlider.set("float slider", 0.0, 0.0, 100.0));
	input3_Parameter.add(clicker.set("clicker", false));
}

void Input_3::draw() {

}