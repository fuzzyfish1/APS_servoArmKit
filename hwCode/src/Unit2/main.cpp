#include <Arduino.h>
#include <Servo.h>

/**
 * System -
 *   OS:  [Linux Mint 22.1 x86 Cinnamon]
 *   IDE: [CLion + PlatformIO]
 * Author: Zain Ali
 *
 * APS ReadySetCode Servo Kit Unit 2: Joystick control
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

//Pick any Analog pin for joystick
#define JOYX_AXS_PIN A0
#define JOYY_AXS_PIN A1

// from datasheet
#define SRV_MIN_US 1000
#define SRV_MAX_US 2000

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
	Serial.println("Unit 2");

	pinMode(JOYY_AXS_PIN, INPUT);
	pinMode(JOYX_AXS_PIN, INPUT);
	pinMode(JOY_BTN_PIN, INPUT);

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

int baseServoPos = 0;

void loop() {

	// you can see these prints in the serial graph, it would be cool to see for troubleshooting
	int joyx = analogRead(JOYX_AXS_PIN);
	Serial.print("Joystick X: ");
	Serial.println(joyx);

	// we could do this branchless, but joystick left, right gives us some extra print conditions for troubleshooting
	if (joyx < 1024/2 - JOY_DEADBAND) {
		Serial.println("Joystick Left");
		baseServoPos -= 1; // decrement
		baseServoPos = min(max(baseServoPos, 0), 180); // constrain to 0-180 (servo range)
		baseServo.write(baseServoPos);
		delay(100);
	} else if (joyx > 1024/2 + JOY_DEADBAND) {
		Serial.println("Joystick Right");
		baseServoPos += 1; // increment
		baseServoPos = min(max(baseServoPos, 0), 180); // constrain to 0-180 (servo range)
		baseServo.write(baseServoPos);
		delay(100);
	} else {
		// in deadband (close enough to zero) don't do anything/move

		// weird print statement to not clog up your serial terminal, its just for troubleshooting
		static int lastPrint = 0;
		if (millis() - lastPrint > 100) { // 100 ms apart prints
			lastPrint = millis();
			Serial.println("Joystick Center");
		}
	}
}