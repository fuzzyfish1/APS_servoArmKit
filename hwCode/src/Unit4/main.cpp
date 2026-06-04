#include <Arduino.h>
#include <Servo.h>

/**
 * System -
 *   OS:  [Linux Mint 22.1 x86 Cinnamon]
 *   IDE: [CLion + PlatformIO]
 * Author: Zain Ali
 *
 * APS ReadySetCode Servo Kit Unit 1
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
// #define JOY_BTN_PIN 2

// program ctrl
// #define JOY_DEADBAND 10
#define NUM_SERV 4
#define SRV_MAX_SPD (60.f / 1000.f) // 5 deg per seconds in deg /ms

Servo baseServo;
Servo shldServo;
Servo elbwServo;
Servo wrstServo;

float msToAngle(const int& posUs) {
	const float angle = (float)(posUs - SRV_MIN_US) * 180.0f / (float)(SRV_MAX_US - SRV_MIN_US);
	const float clampedAngle = max(0.f, min(angle, 180.f));
	return clampedAngle;
}

int angleToMs (const float& angle) {
	const float clampedAngle = max(0.f, min(angle, 180.f));
	const int pulseUs = SRV_MIN_US + (int)(clampedAngle * (float)(SRV_MAX_US - SRV_MIN_US) / 180.0f);
	return pulseUs;
}

void setup() {

	Serial.begin(115200);

	baseServo.attach(BASE_SRV_PIN, SRV_MIN_US, SRV_MAX_US);
	shldServo.attach(SHLD_SRV_PIN, SRV_MIN_US, SRV_MAX_US);
	elbwServo.attach(ELBW_SRV_PIN, SRV_MIN_US, SRV_MAX_US);
	wrstServo.attach(WRST_SRV_PIN, SRV_MIN_US, SRV_MAX_US);

	// start at some position
	baseServo.write(90);
	delay(100);
	shldServo.write(90);
	delay(100);
	elbwServo.write(0);
	delay(100);
	wrstServo.write(180);
	delay(1000);
}

float maxAbsList (const float list[NUM_SERV]) {

	float ret = abs(list[0]);

	for (int i = 1; i < NUM_SERV; i++) {
		ret = max(ret, abs(list[i]));
	}

	return ret;
}

// track current positions ourselves, don't rely on readMicroseconds after write()
float currentAngles[NUM_SERV] = {90.f, 90.f, 0.f, 180.f};  // match your setup() writes

void runSlow(const float pos[NUM_SERV]) {
	Servo* servList[NUM_SERV] = {&baseServo, &shldServo, &elbwServo, &wrstServo};

	float delt[NUM_SERV] = {};
	for (int i = 0; i < NUM_SERV; i++) {
		delt[i] = pos[i] - currentAngles[i];
	}

	float maxAbs = maxAbsList(delt);
	if (maxAbs < 0.01f) return;

	float moveDurationMs = maxAbs / SRV_MAX_SPD;

	Serial.print("moveDurationMs: ");
	Serial.println(moveDurationMs);  // should now be e.g. 90000 for 90 deg at 1deg/s

	float velList[NUM_SERV] = {};
	for (int i = 0; i < NUM_SERV; i++) {
		velList[i] = delt[i] / moveDurationMs;
	}

	long initUs = micros();
	long moveDurationUs = (long)(moveDurationMs * 1000.f);

	while (true) {
		long elapsedUs = micros() - initUs;
		if (elapsedUs >= moveDurationUs) break;

		float elapsedMs = elapsedUs / 1000.f;
		for (int i = 0; i < NUM_SERV; i++) {
			float angle = currentAngles[i] + velList[i] * elapsedMs;
			servList[i]->writeMicroseconds(angleToMs(angle));
		}
	}

	for (int i = 0; i < NUM_SERV; i++) {
		servList[i]->writeMicroseconds(angleToMs(pos[i]));
		currentAngles[i] = pos[i];
	}
}
void loop() {

	digitalWrite(LED_BUILTIN, HIGH);
	const float pos[] = {180.f, 90.f, 0.f, 45.f};
	runSlow(&pos[0]);
	// delay(2000);

	digitalWrite(LED_BUILTIN, LOW);
	const float pos2[] = {0.f, 45.f, 45.f, 135.f};
	runSlow(&pos2[0]);
	// delay(2000);
}