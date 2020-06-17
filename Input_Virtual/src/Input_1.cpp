#include "Input_1.h"

Input_1::Input_1()
{
}

Input_1::~Input_1()
{
}

void Input_1::setup() {


	input1_Parameter.setName("LED_Cube");

	input1_Parameter.add(intSlider.set("int slider", 0, 0, 100)); //wert beim programmstart, niedrigster wert, höchster wert
	input1_Parameter.add(floatSlider.set("float slider", 0.0, 0.0, 100.0));
	input1_Parameter.add(clicker.set("clicker", false));
}

void Input_1::draw() {
}