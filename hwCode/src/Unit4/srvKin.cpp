#include <srvKin.h>
#include <Servo.h>
#include <Arduino.h>

/**
 * System -
 *   OS:  [Linux Mint 22.1 x86 Cinnamon]
 *   IDE: [CLion + PlatformIO]
 * Author: Zain Ali
 *
 * Kinematics and helper function implementations
 */


// don't touch this variable, it is extern-ed elsewhere
float currentAngles[NUM_SERV] = {90.f, 90.f, 0.f, 180.f};

float usToAngle(const int &posUs) {
	const float angle = (float) (posUs - SRV_MIN_US) * 180.0f / (float) (SRV_MAX_US - SRV_MIN_US);
	const float clampedAngle = max(0.f, min(angle, 180.f));
	return clampedAngle;
}

int angleToUs(const float &angle) {
	const float clampedAngle = max(0.f, min(angle, 180.f));
	const int pulseUs = SRV_MIN_US + (int) (clampedAngle * (float) (SRV_MAX_US - SRV_MIN_US) / 180.0f);
	return pulseUs;
}


float maxAbsList(const float list[NUM_SERV]) {
	float ret = abs(list[0]);

	for (int i = 1; i < NUM_SERV; i++) {
		ret = max(ret, abs(list[i]));
	}

	return ret;
}

void runSlow(const float pos[NUM_SERV], Servo *servList[NUM_SERV]) {

	// calc all the deltas (difference between final pos and curr pos)
	float delt[NUM_SERV] = {};
	for (int i = 0; i < NUM_SERV; i++) {
		delt[i] = pos[i] - currentAngles[i];
	}

	// find the largest delta, if we are close enough don't do anything
	float maxAbs = maxAbsList(delt);
	if (maxAbs < 0.01f) return;

	// calc the move time at that speed
	float moveDurationMs = maxAbs / SRV_MAX_SPD;

	Serial.print("moveDurationMs: ");
	Serial.println(moveDurationMs);

	// recalc all the velocities so that all the servos reach delta distance traveled at the same time
	float velList[NUM_SERV] = {};
	for (int i = 0; i < NUM_SERV; i++) {
		velList[i] = delt[i] / moveDurationMs;
	}

	// for elapsedTime ...
	for (long initMS = millis(), elapsedMS = 0; elapsedMS <= moveDurationMs; elapsedMS = millis() - initMS) {
		// for every servo
		for (int i = 0; i < NUM_SERV; i++) {
			// go to the angle, it should theoretically be at @ elapsedTime
			float angle = currentAngles[i] + velList[i] * elapsedMS;
			servList[i]->writeMicroseconds(angleToUs(angle));
		}

		// for troubleshooting, you can see if its trying to move by watching it blink
		// blink LED for DEL milliseconds, then turn off for DEL seconds
		digitalWrite(LED_BUILTIN, millis() % (DEL * 2) > DEL);
	}

	// delay for 500 milliseconds, make sure servos get set to final pos
	for (const long l = millis(); millis() - l < 500;) {
		for (int i = 0; i < NUM_SERV; i++) {
			servList[i]->writeMicroseconds(angleToUs(pos[i]));
			currentAngles[i] = pos[i];
		}
		digitalWrite(LED_BUILTIN, millis() % (DEL * 2) > DEL);
	}

	// turn LED off because we were blinking earlier
	digitalWrite(LED_BUILTIN, LOW);
}

void initServos(const int pos[NUM_SERV], Servo* servList[NUM_SERV]) {

	for (const long l = millis(); millis() - l < 500;) {
		for (int i = 0; i < NUM_SERV; i++) {
			servList[i]->write(pos[i]);
		}
		delay(100);
		digitalWrite(LED_BUILTIN, millis() % (DEL * 2) > DEL);
	}

	digitalWrite(LED_BUILTIN, LOW);

	for (int i = 0; i < NUM_SERV; i++) {
		currentAngles[i] = pos[i];
	}
}