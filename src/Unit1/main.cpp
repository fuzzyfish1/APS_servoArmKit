#include <Arduino.h>
#include <Servo.h>

/**
 * System -
 *   OS:  [Linux Mint 22.1 x86 Cinnamon]
 *   IDE: [CLion + PlatformIO]
 * Author: Zain Ali
 *
 * APS ReadySetCode Servo Kit Unit 1: Servo.h Library functions
 *
 * This is a great place to show to kids how Servo.h uses PWM to talk to the servo
 * and explain what PWM is
 *
 * Docs + links: labeled by skim or read or if you still want more
 *		https://www.electronicoscaldas.com/datasheet/MG90S_Tower-Pro.pdf?srsltid=AfmBOooEwEH2tjnxAT4QEQ_bXN8yQCYZHra_2rE_CHEPRiJlAYR6e1zs
 *		read this, take a screenshot of the PWM for the slides
 *		https://naylampmechatronics.com/img/cms/Datasheets/000036%20-%20datasheet%20KY-023-Joy-IT.pdf
 *		shows a code example
 *		https://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf
 *		for your wiring concerns
 */

// pick any PWM pins for SERVOs
#define BASE_SRV_PIN 9
#define SHLD_SRV_PIN 6
#define ELBW_SRV_PIN 5
#define WRST_SRV_PIN 3

// from datasheet
#define SRV_MIN_US 1000
#define SRV_MAX_US 2000

Servo baseServo;
Servo shldServo;
Servo elbwServo;
Servo wrstServo;

void setup() {

	Serial.begin(115200);
	Serial.println("Unit 1");

	baseServo.attach(BASE_SRV_PIN, SRV_MIN_US, SRV_MAX_US);
	shldServo.attach(SHLD_SRV_PIN, SRV_MIN_US, SRV_MAX_US);
	elbwServo.attach(ELBW_SRV_PIN, SRV_MIN_US, SRV_MAX_US);
	wrstServo.attach(WRST_SRV_PIN, SRV_MIN_US, SRV_MAX_US);

	// start at some position
	baseServo.write(0);
	delay(100);
	shldServo.write(0);
	delay(100);
	elbwServo.write(0);
	delay(100);
	wrstServo.write(0);
	delay(1000);
}

void loop() {
	/** UNIT 1 **
	 * you are going to need to find the positions to go to
	 * with the upgraded wiring you can theoretically remove the delay(100), but I recommend keeping the delay(1000)
	 * this is to figure out how the servo works and what positions and all that
	 * then figure out how the joystick works
	 ***/

	// go to pos 1
	baseServo.write(0);
	delay(200);
	shldServo.write(90);
	delay(200);
	elbwServo.write(0);
	delay(200);
	wrstServo.write(90);
	delay(2000);

	// go to pos 2
	baseServo.write(90);
	delay(200);
	shldServo.write(45);
	delay(200);
	elbwServo.write(45);
	delay(200);
	wrstServo.write(135);
	delay(2000);

}