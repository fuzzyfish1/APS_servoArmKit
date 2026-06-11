#include <Arduino.h>
#include <Servo.h>
#include <srvKin.h>

/**
 * System -
 *   OS:  [Linux Mint 22.1 x86 Cinnamon]
 *   IDE: [CLion + PlatformIO]
 * Author: Zain Ali
 *
 * APS ReadySetCode Servo Kit Final Code Unit 4: Arrays, Pointers and Kinematics
 *
 * give this to the child, this is what they will start with,
 * also I split the code up better now due to now having the ability to give the kids an include directory
 * this is controlling code only
 *
 * The code that you can teach with this is lists, arrays, and pointers
 *		- A pointer, points to an object
 *		- An Array, a space in memory with an indexable reference
 *		- kinematics, calculating where things are supposed to be,
 *		but that was kinda abstracted away sadly to focus more on code than math but we should seriously reconsider the math isn't that bad
 *
 * I have abstracted away kinematics and it's helpers into srvKin.h + srvKin.cpp
 * it does have Doxygen Comments that should be recognizable to your IDE
 *
 * runSlow() -> blinks when running, as a sanity check for troubleshooting
 * if things are blinking and nothing is moving, the problem might not be a software one
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

Servo baseServo;
Servo shldServo;
Servo elbwServo;
Servo wrstServo;

// a list of Servo, its useful for passing as a parameter
Servo* servoList[NUM_SERV] = {&baseServo, &shldServo, &elbwServo, &wrstServo};

const float homePos[] = {90.f, 90.f, 0.f, 180.f};
const float grabPos[] = {0.f, 180.f, 0.f, 90.f};
const float dropPos[] = {180.f, 180.f, 0.f, 90.f};

void setup() {

	// for print statements
	Serial.begin(115200);
	Serial.println("Servo Unit 4");

	baseServo.attach(BASE_SRV_PIN, SRV_MIN_US, SRV_MAX_US);
	shldServo.attach(SHLD_SRV_PIN, SRV_MIN_US, SRV_MAX_US);
	elbwServo.attach(ELBW_SRV_PIN, SRV_MIN_US, SRV_MAX_US);
	wrstServo.attach(WRST_SRV_PIN, SRV_MIN_US, SRV_MAX_US);

	const int startPos[NUM_SERV] = {90,90,0,180};
	initServos(&startPos[0], servoList);

	delay(1000);
}

void loop() {

	// start at home
	runSlow(homePos, servoList);
	delay(2000);

	// grab ball
	runSlow(grabPos, servoList);
	delay(2000);

	// go home
	runSlow(homePos, servoList);
	delay(2000);

	// drop it
	runSlow(dropPos, servoList);
	delay(2000);
}
