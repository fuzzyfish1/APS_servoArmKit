#include <Servo.h>
#include <Arduino.h>

#ifndef HWCODE_SRVKIN_H
#define HWCODE_SRVKIN_H

/**
* System -
 *   OS:  [Linux Mint 22.1 x86 Cinnamon]
 *   IDE: [CLion + PlatformIO]
 * Author: Zain Ali
 *
 * Kinematics and helper function header files
 */


// Andy asked me to change these to default vals, rather than datasheet values. this is worth investigating

#define SRV_MIN_US 1000
#define SRV_MAX_US 2000
//#define SRV_MIN_US 544
//#define SRV_MAX_US 2400

#define NUM_SERV 4
#define SRV_MAX_SPD (180.f / 1000.f)
#define DEL 500

/** usToAngle() **
 * @param posUs - the width of the PWM wave sent to the servo
 * @return - the angle interpreted by the servo
 *
 * angle is constrained between 0 - 180
 */
float usToAngle(const int &posUs);

/** angleToUs() **
 * @param angle - the angle to go to
 * @return - the PWM width in MS
 *
 * constrains the angle between 0 - 180
 */
int angleToUs(const float &angle);

/** maxAbsList()
 * @param list a list of 4 floats
 * @return the number with the largest magnitude
 * **/
float maxAbsList(const float list[NUM_SERV]);

/** runSlow() **
 * @param pos - list of 4 float angle positions to go to {basePos, shoulder, elbow, and wrist}
 * speed will cap out at SRV_MAX_SPD
 * All servos aim to reach the end at the same time for a smooth consistent move
 * **/
void runSlow(const float pos[NUM_SERV], Servo *servList[NUM_SERV]);

/** initServos() **
 * @param pos - go to this in integer deg
 * @param servList - the servos to use
 ****/
void initServos(const int pos[NUM_SERV], Servo* servList[NUM_SERV]);

/* float currentAngles[]
 * allows the kinematics to remember last set position, defined in main.cpp
 * MUST HAVE STARTING POSITIONS to be accurate ...
 */
extern float currentAngles[NUM_SERV];

#endif //HWCODE_SRVKIN_H