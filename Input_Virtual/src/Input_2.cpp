#include "Input_2.h"

Input_2::Input_2()
{
}

Input_2::~Input_2()
{
}

void Input_2::setup() {


	input2_Parameter.setName("Temperature_Cube");

	input2_Parameter.add(intSlider.set("int slider", 0, 0, 100));
	input2_Parameter.add(floatSlider.set("float slider", 18.0, 0.0, 37.0));
	input2_Parameter.add(clicker.set("clicker", false));
}

void Input_2::draw() {

}