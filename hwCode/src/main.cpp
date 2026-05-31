#include <Arduino.h>
#include <Servo.h>

/**
 * System -
 *   OS:  [Linux Mint 22.1 x86 Cinnamon]
 *   IDE: [CLion + PlatformIO]
 * Author: Zain Ali
 *
 * APS ReadySetCode Servo Kit Example Code
 *
 * Docs + links: labeled by skim or read or if you still want more\
 *		https://www.electronicoscaldas.com/datasheet/MG90S_Tower-Pro.pdf?srsltid=AfmBOooEwEH2tjnxAT4QEQ_bXN8yQCYZHra_2rE_CHEPRiJlAYR6e1zs
 *		read this, take a screenshot of the PWM for the slides
 *		https://naylampmechatronics.com/img/cms/Datasheets/000036%20-%20datasheet%20KY-023-Joy-IT.pdf
 *		shows a code example
 */

// pick any PWM pins for SERVOs
#define BASE_SRV_PIN 9
#define SHLD_SRV_PIN 6
#define ELBW_SRV_PIN 5
#define WRST_SRV_PIN 3

//Pick any Analog pin for joystick
#define JOYX_AXS_PIN A0
#define JOYY_AXS_PIN A1

// pick any remaining digital pin for joystick
#define JOY_BTN_PIN 2

// program ctrl
#define JOY_DEADBAND 10

Servo baseServo;
Servo shldServo;
Servo elbwServo;
Servo wrstServo;

void setup() {

	Serial.begin(115200);

	pinMode(JOYY_AXS_PIN, OUTPUT);
	pinMode(JOYX_AXS_PIN, OUTPUT);
	pinMode(JOY_BTN_PIN, INPUT);

	baseServo.attach(BASE_SRV_PIN);
	shldServo.attach(SHLD_SRV_PIN);
	elbwServo.attach(ELBW_SRV_PIN);
	wrstServo.attach(WRST_SRV_PIN);

	// start at some position,
	baseServo.write(0);
	delay(100);
	shldServo.write(0);
	delay(100);
	elbwServo.write(0);
	delay(100);
	wrstServo.write(0);
	delay(1000);
}

float baseServoPos = 0;
float shldServoPos = 0;
float elbwServoPos = 0;
float wrstServoPos = 0;

void loop() {
	/**UNIT 1
	 * you are going to need to find the positions to go to
	 * with the upgraded wiring you can theoretically remove the delay(100), but I reccomend keeping the delay(1000)
	 * this is to figure out how the servo works and what positions and all that
	 * then figure out how the joystick works
	 *
	 ***/
	// go to pos 1
	baseServo.write(100);
	delay(100);
	shldServo.write(100);
	delay(100);
	elbwServo.write(100);
	delay(100);
	wrstServo.write(100);
	delay(1000);

	// go to pos 2
	baseServo.write(100);
	delay(100);
	shldServo.write(100);
	delay(100);
	elbwServo.write(100);
	delay(100);
	wrstServo.write(100);
	delay(1000);

	// go to pos 3
	baseServo.write(100);
	delay(100);
	shldServo.write(100);
	delay(100);
	elbwServo.write(100);
	delay(100);
	wrstServo.write(100);
	delay(1000);

	/** ^^ UNIT 1 ^^*/

	/** UNIT 2 **
	 * Joystick X for Base Servo control
	 */

	int joyx = analogRead(JOYX_AXS_PIN);

	// you can see these prints in the serial graph, it would be cool to see for troublesooting
	Serial.print("Joystick X: ");
	Serial.println(joyx);

	// you can see this as well in the serial graph
	int btnPressed = digitalRead(JOY_BTN_PIN);
	Serial.print("Button Pressed: ");
	Serial.println(btnPressed);

	if (joyx < 1024/2 - JOY_DEADBAND) {
		Serial.println("Joystick Left");
		baseServoPos -= baseServoPos; // decrement
		baseServoPos = min(max(baseServoPos, 0), 180); // constrain to 0-180 (servo range
		baseServo.write(baseServoPos);
		delay(100);
	} else if (joyx > 1024/2 + JOY_DEADBAND) {
		Serial.println("Joystick Right");
		baseServoPos += baseServoPos; // increment
		baseServoPos = min(max(baseServoPos, 0), 180); // constrain to 0-180 (servo range
		baseServo.write(baseServoPos);
		delay(100);
	} else {
		// in deadband (close enough to zero) don't do anything/move
	}

	/** ^^ UNIT 2 ^^*/
	/** UNIT 3 **
	 *
	 */

	if (btnPressed) {
		// go to grab position
		// you need to find these angles, I apologize
		Serial.println("Button Pressed");

		shldServo.write(100);
		delay(100);
		elbwServo.write(100);
		delay(100);
		wrstServo.write(100);
		delay(1000); // this will stall the code for 1s, there isa better way to do this

	} else {
		// go back to home position
		Serial.println("Button Released");

		shldServo.write(100);
		delay(100);
		elbwServo.write(100);
		delay(100);
		wrstServo.write(100);

	}



}