#pragma config(Sensor, in1,    leftArmPot,     sensorPotentiometer)
#pragma config(Sensor, in2,    rightArmPot,    sensorPotentiometer)
#pragma config(Sensor, in3,    autonPot,       sensorPotentiometer)
#pragma config(Sensor, dgtl1,  limitSwitch,    sensorTouch)
#pragma config(Sensor, dgtl2,  rightEnc,       sensorQuadEncoder)
#pragma config(Sensor, dgtl4,  rightSonar,     sensorSONAR_cm)
#pragma config(Sensor, dgtl6,  leftEnc,        sensorQuadEncoder)
#pragma config(Sensor, dgtl8,  leftSonar,      sensorSONAR_cm)
#pragma config(Sensor, dgtl10, clawEnc,        sensorQuadEncoder)
#pragma config(Sensor, dgtl12, intakeLimit,    sensorTouch)
#pragma config(Motor,  port2,           right,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           right2,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           arm,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           claw,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           mangonel,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           intake,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           left,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           left2,         tmotorVex393_MC29, openLoop, encoderPort, None)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX2)
#pragma competitionControl(Competition)

#include "Vex_Competition_Includes.c"

#define abs(X) ((X < 0) ? -1 * X : X)

void nAutonomous();
void moveDegrees(int degrees); //Positive degrees turn left
void moveInches(int inches); //Positive inches moves forward

task intakeControl();
void toggleIntakeDown();
void toggleIntakeUp();

task clawControl();
void moveClaw(int degrees);//Positive moves claw up

task armControl();
void moveArm(int degrees); //Positive moves arm up

int clawState;
int leftArmState;
int rightArmState;
int leftPower;
int rightPower;
int intakeUpState;
int intakeDownState;
int rightBasePower;
int leftBasePower;
float baseTicks;

void pre_auton()
{
	bStopTasksBetweenModes = true;
}

task autonomous()
{
	nAutonomous();
}



//---------------------------------------------------------------------------------------------------------------
//
//																D R I V E R  C O N T R O L   S E C T I O N
//---------------------------
// Functions
//---------------------------
//
//---------------------------
// Controller 1
//---------------------------
// AccelX:
// AccelY:
// AccelZ:
// Btn5U: Mangonel
// Btn5D: Mangonel
// Btn6U: Intake
// Btn6D: Intake
// Btn7U: Auton Tester
// Btn7D: Switch Controller
// Btn7L:
// Btn7R:
// Btn8U:
// Btn8D: Switch Direction
// Btn8L: Change "Gear"
// Btn8R: Change "Gear"
// Ch1:
// Ch2: Right-Side Base Motors
// Ch3: Left-Side Base Motors
// Ch4:
//---------------------------
// Controller 2
//---------------------------
// AccelXXmtr2:
// AccelYXmtr2:
// AccelZXmtr2:
// Btn5UXmtr2: Claw
// Btn5DXmtr2: Claw
// Btn6UXmtr2: Arm
// Btn6DXmtr2: Arm
// Btn7UXmtr2: Auton Tester
// Btn7DXmtr2: Switch Controller
// Btn7LXmtr2:
// Btn7RXmtr2:
// Btn8UXmtr2:
// Btn8DXmtr2: Switch Direction
// Btn8LXmtr2: Change "Gear"
// Btn8RXmtr2: Change "Gear"
// Ch1Xmtr2:
// Ch2Xmtr2: Right-Side Base Motors
// Ch3Xmtr2: Left-Side Base Motors
// Ch4Xmtr2:
//---------------------------------------------------------------------------------------------------------------
task usercontrol()
{
	int driveGear = 1;
	int direction = 1;
	int driveController = 1;
	int trueCh2;
  int trueCh3;
  int mangonelState = 0;

  int threshold = 50;

  int lowCapDist = 12;
  int highCapDist = 12;

  startTask(intakeControl);
  startTask(clawControl);
  startTask(armControl);
  SensorValue[leftEnc] = 0;
  SensorValue[rightEnc] = 0;
  SensorValue[clawEnc] = 0;

	while (true){

		if(vexRT[Btn7U] == 1 && vexRT[Btn7UXmtr2] == 1) {
			nAutonomous();
		}
		else if(vexRT[Btn7R] == 1) {
			moveDegrees(-90);
		}
		else if(vexRT[Btn7L] == 1) {
			moveDegrees(90);
		}
		else if(vexRT[Btn7RXmtr2] == 1) {
			moveInches(24);
		}
		else if(vexRT[Btn7LXmtr2] == 1) {
			moveInches(-24);
		}

		//=======================BASE CONROL (BOTH CONTROLLERS)=======================
		if(driveController == 1 && direction == 1) {
			trueCh2 = (vexRT[Ch2])/driveGear;
			trueCh3 = (vexRT[Ch3])/driveGear;
		}
		else if(driveController == 1 && direction == -1){
			trueCh2 = (vexRT[Ch3]*direction)/driveGear;
			trueCh3 = (vexRT[Ch2]*direction)/driveGear;
		}
		else if(driveController == -1 && direction == 1){
			trueCh2 = (vexRT[Ch2Xmtr2]/driveGear);
			trueCh3 = (vexRT[Ch3Xmtr2]/driveGear);
		}
		else if(driveController == -1 && direction == -1){
			trueCh2 = (vexRT[Ch3Xmtr2]*direction)/driveGear;
			trueCh3 = (vexRT[Ch2Xmtr2]*direction)/driveGear;
		}

		if((vexRT[Btn8D] == 1)|| (vexRT[Btn8DXmtr2] == 1)) {
			direction = direction * -1;
			wait1Msec(0500);
		}
		if((vexRT[Btn7D] == 1) || (vexRT[Btn7DXmtr2] == 1)) {
			driveController = driveController * -1;
			wait1Msec(0500);
		}

		if(((vexRT[Btn8R] == 1) && (driveController == 1)) || ((vexRT[Btn8RXmtr2] == 1) && (driveController == -1))) {
			driveGear += 1;
			wait1Msec(0500);
		}
		else if((((vexRT[Btn8L] == 1) && (driveController == 1)) || ((vexRT[Btn8LXmtr2] == 1) && (driveController == -1)))&&(driveGear > 1)){
			driveGear -= 1;
			wait1Msec(0500);
		}

		if(trueCh2 > threshold){
			motor[right] = trueCh2;
			motor[right2] = trueCh2;
		}
		else if(trueCh2 > 10 && trueCh2 < threshold){
			motor[right] = threshold;
			motor[right2] = threshold;
		}
		else if(trueCh2 < -10 && trueCh2 > (-threshold)){
			motor[right] = -threshold;
			motor[right2] = -threshold;
		}
		else if(trueCh2 < -threshold){
			motor[right] = trueCh2;
			motor[right2] = trueCh2;
		}
		else{
			motor[right] = 0;
			motor[right2] = 0;
		}

		if(trueCh3 > threshold){
			motor[left] = trueCh3;
			motor[left2] = trueCh3;
		}
		else if(trueCh3 > 10 && trueCh3 < threshold){
			motor[left] = threshold;
			motor[left2] = threshold;
		}
		else if(trueCh3 < -10 && trueCh3 > (-threshold)){
			motor[left] = -threshold;
			motor[left2] = -threshold;
		}
		else if(trueCh3 < -threshold){
			motor[left] = trueCh3;
			motor[left2] = trueCh3;
		}
		else{
			motor[left] = 0;
			motor[left2] = 0;
		}

	      	//=======================MANGONEL CONTROL (MAIN JOYSTICK)===================
		if(vexRT[Btn5U]==1) {
			motor[mangonel] = 127;
			mangonelState = 1;
		}
		else if(mangonelState == 1 && SensorValue[limitSwitch] != 1) {
			motor[mangonel] = 127;
		}
		else if((SensorValue[limitSwitch] == 1 || vexRT[Btn5D] == 1) && vexRT[Btn6U] != 1) { //"Else if" due to conflicts with Btn6U in intake section; can't be left as just an "else" statement
			motor[mangonel] = 0;
			mangonelState = 0;
		}
	} //end of while loop
}

//---------------------------------------------------------------------------------------------------------------
//
//                                A U T O N O M O U S   C O N T R O L   S E C T I O N
//														Functions:
//																basicAutonomous - Simple sutonomous code
//																nAutonomous - Template autonomous code
//
//---------------------------------------------------------------------------------------------------------------

int limiter(int n, int lowerBound) {
	if(n > 127) {
			return 127;
	}
	else if(n < -127) {
			return -127;
	}
	else if(n > -lowerBound && n < lowerBound) {
			return 0;
	}
	else {
		return n;
	}
}

void moveDegrees(int degrees) { //Positive degrees turns left
	SensorValue[leftEnc] = 0;
  	SensorValue[rightEnc] = 0;
	baseTicks = 3.3 * degrees;
	rightBasePower = baseTicks - SensorValue[rightEnc];
	leftBasePower = - SensorValue[leftEnc] - baseTicks;
	int lowerBound = 30;
	while(limiter(rightBasePower,lowerBound) != 0 || limiter(leftBasePower,lowerBound) != 0) {
			motor[right] = limiter(rightBasePower,lowerBound);
			motor[right2] = limiter(rightBasePower,lowerBound);
			motor[left] = limiter(leftBasePower,lowerBound);
			motor[left2] = limiter(leftBasePower,lowerBound);
			rightBasePower = baseTicks - SensorValue[rightEnc];
			leftBasePower = - SensorValue[leftEnc] - baseTicks;
	}
}

void moveInches(int inches) { //Positive power moves forward
	SensorValue[leftEnc] = 0;
  SensorValue[rightEnc] = 0;
	baseTicks = 20 * inches;
	rightBasePower = baseTicks - SensorValue[rightEnc];
	leftBasePower = baseTicks - SensorValue[leftEnc];
	int lowerBound = 30;
	while(limiter(rightBasePower,lowerBound) != 0 || limiter(leftBasePower,lowerBound) != 0) {
			motor[right] = limiter(rightBasePower,lowerBound);
			motor[right2] = limiter(rightBasePower,lowerBound);
			motor[left] = limiter(leftBasePower,lowerBound);
			motor[left2] = limiter(leftBasePower,lowerBound);
			rightBasePower = baseTicks - SensorValue[rightEnc];
			leftBasePower = baseTicks - SensorValue[leftEnc];
	}
}

void nAutonomous(){
	moveDegrees(-30);
	moveInches(-12);
	moveClaw(-135);
	moveInches(12);
	motor[mangonel] = 127;
	wait1Msec(0500);
	while(SensorValue[limitSwitch] != 1) {
		//Do nothing
	}
	motor[mangonel] = 0;
	moveDegrees(30);
	moveInches(36);
	moveInches(-36);
	moveDegrees(90);
	moveInches(-36);
	toggleIntakeUp();
	while(SensorValue[intakeLimit] != 1) {
		//Do nothing
	}
	toggleIntakeDown();
	moveInches(-12);
	toggleIntakeDown(); //Turn intake off
	moveInches(48);
}

void moveClaw(int degrees) { //Positive moves claw up
	clawState = clawState + 3 * degrees; //around 5 potentiometer ticks per degree (according to calculations)
}

task clawControl() {
	clawState = 0;
	SensorValue[clawEnc] = 0;
	while(true) {
		if(vexRT[Btn5DXmtr2] == 1) {
			moveClaw(-30);
			wait1Msec(0100);
		}
		else if(vexRT[Btn5UXmtr2] == 1) {
			moveClaw(30);
			wait1Msec(0100);
		}

		int power = (int)((clawState - SensorValue[clawEnc])/3);
		if(power > 127) {
			power = 127;
		}
		else if(power < -127) {
			power = -127;
		}
		else if(abs(power) < 10) {
			power = 0;
		}

		motor[claw] = power;
	}
}

void moveArm(int degrees) { //Positive moves arm up
	leftArmState = leftArmState + 10 * degrees; //around 10 potentiometer ticks per degree (according to calculations)
	rightArmState = rightArmState - 10 * degrees;
}

task armControl() {
	/*
	while(true) {
	if(vexRT[Btn6DXmtr2] == 1) {
			motor[arm] = -100;
		}
		else if(vexRT[Btn6UXmtr2] == 1) {
			motor[arm] = 100;
		}	
		else {
			motor[arm] = 0;
		}
	*/
	leftArmState = SensorValue[leftArmPot];
	rightArmState = SensorValue[rightArmPot];
	while(true) {
		if(vexRT[Btn6DXmtr2] == 1) {
			moveArm(-3);
			wait1Msec(0050);
		}
		else if(vexRT[Btn6UXmtr2] == 1) {
			moveArm(3);
			wait1Msec(0050);
		}

		leftPower = (int)((leftArmState - SensorValue[leftArmPot])/4);
		rightPower = (int)((SensorValue[rightArmPot] - rightArmState)/4);
		
		if(leftPower > 127) {
			leftPower = 127;
		}
		else if(leftPower < -127) {
			leftPower = -127;
		}

		if(rightPower > 127) {
			rightPower = 127;
		}
		else if(rightPower < -127) {
			rightPower = -127;
		}
		
		int avgPower = (int)((rightPower + leftPower)/2);

		if(abs(avgPower) < 10) {
			avgPower = 0;
		}
		else if(abs(avgPower) < 30) {
			avgPower = 20;
		}

		motor[arm] = avgPower;
	} 
}

void toggleIntakeUp() {
	intakeUpState = intakeUpState * -1;
	if(intakeDownState == 1){
		intakeDownState = intakeDownState * -1;
		}
}

void toggleIntakeDown() {
	intakeDownState = intakeDownState * -1;
	if(intakeUpState == 1) {
		intakeUpState = intakeUpState * -1;
		}
}

task intakeControl(){
	intakeUpState = -1;
  	intakeDownState = -1;
	int ball = 0;

	while(true){
		if(vexRT[Btn6U] == 1) {
			toggleIntakeUp();
			wait1Msec(0100);
		}
		else if(vexRT[Btn6D] == 1) {
			toggleIntakeDown();
			wait1Msec(0100);
		}

		if(SensorValue[intakeLimit] == 1 && ball == 0) {
			ball = 1;
  			intakeUpState = -1;
  			intakeDownState = -1;
			motor[intake] = 0;

		}
		else if(intakeUpState == 1){
			motor[intake] = 127;
			ball = 0;
			wait1Msec(0200);
		}
		else if(intakeDownState == 1){
			motor[intake] = -127;
			ball = 0;
			wait1Msec(0200);
		}
		else{
			motor[intake] = 0;
		}
	}
}
