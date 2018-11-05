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

task moveAuton();
void oAutonomous();
void nAutonomous();

void moveToDistance(int rightDist, int leftDist);
void ctrlMoveToDistance(int rightDist, int leftDist);
void moveDegrees(int degrees);
void moveInches(int inches);
void moveTicks(int rightTicks, int leftTicks);

void cntrlMoveIntake();
void toggleIntakeDown();
void toggleIntakeUp();
void toggleIntakeOff();

void cntrlMoveClaw();
void moveClaw(int degrees);
void autoMoveClaw(int degrees);

void cntrlMoveArm();
void moveArm(int degrees);
void autoMoveArm(int degrees);

int clawState;
int clawButton;
int clawPower;

int rightArmState;
int leftArmState;
int rightArmPower;
int leftArmPower;
int armPower;
int armButton;

int intakeUpState;
int intakeDownState;

int rightBasePower;
int leftBasePower;
int needRightTicks;
int needLeftTicks;
int ball;
int ballButton;
float baseTicks;
float ratio;

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
// Btn7L: Aim for Low Flag
// Btn7R: Aim for High Flag
// Btn8U: Cancel any Aiming
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
// Btn7LXmtr2: Aim for Low Post
// Btn7RXmtr2: Aim for High Post
// Btn8UXmtr2: Claw Enc Reset
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

	int lowCapDist = 34;
	int highCapDist = 24;
	SensorValue[leftEnc] = 0;
	SensorValue[rightEnc] = 0;
	SensorValue[clawEnc] = 0;

	intakeUpState = -1;
	intakeDownState = -1;
	ball = 0;
	ballButton = 0;
	leftArmState = SensorValue[leftArmPot];
	rightArmState = SensorValue[rightArmPot];
	armButton = 0;
	clawState = 0;
	clawButton = 0;
	SensorValue[clawEnc] = 0;

	while (true){
		cntrlMoveIntake();
		cntrlMoveClaw();
		cntrlMoveArm();
		if(vexRT[Btn7U] == 1 && vexRT[Btn7UXmtr2] == 1) {
			nAutonomous();
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

		if(abs(trueCh2) > threshold){
			motor[right] = trueCh2;
			motor[right2] = trueCh2;
		}
		else if(trueCh2 > 20 && trueCh2 < threshold){
			motor[right] = threshold;
			motor[right2] = threshold;
		}
		else if(trueCh2 < -20 && trueCh2 > (-threshold)){
			motor[right] = -threshold;
			motor[right2] = -threshold;
		}
		else if(vexRT[Btn7L] == 1) {
			ctrlMoveToDistance(70,70);
		}
		else if(vexRT[Btn7R] == 1) {
			ctrlMoveToDistance(160,160);
		}
		else if(vexRT[Btn7LXmtr2] == 1) {
			ctrlMoveToDistance(24,24);
		}
		else if(vexRT[Btn7RXmtr2] == 1) {
			ctrlMoveToDistance(34,34);
		}
		else{
			motor[right] = 0;
			motor[right2] = 0;
		}

		if(abs(trueCh3) > threshold){
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
		else{
			motor[left] = 0;
			motor[left2] = 0;
		}

	     //=======================MANGONEL CONTROL (MAIN JOYSTICK)===================
		if(vexRT[Btn5U]==1) {
			motor[mangonel] = 127;
			wait1Msec(0500);
		}
		else if(SensorValue[limitSwitch] != 1) {
			motor[mangonel] = 127;
		}
		else {
			motor[mangonel] = 0;
		}
	} //end of while loop
}
//---------------------------------------------------------------------------------------------------------------
//
//                                A U T O N O M O U S   C O N T R O L   S E C T I O N
//				Functions:
//				Base Methods
//					movetoDistance 		- moves robot to a certain distance from the fence
//					ctrlMoveToDistance 	- moves robot to a certain distance from the fence, cancellable through 8U
//					moveDegrees			- positive degrees turns counterclockwise (waits until completed)
//					moveInches			- positive moves forward (waits until completed)
//					moveTicks				- positive moves forward (waits until completed)
//				Claw Methods
//					autoMoveClaw		- positive degrees turns claw upwards (waits until completed)
//					moveClaw 				- positive degrees turns claw upwards
//					cntrlMoveClaw		- remote control
//				Arm Methods
//					autoMoveArm			- positive degrees turns claw upwards (waits until completed)
//					moveArm 				- positive degrees turns claw upwards
//					cntrlMoveArm		- remote control
//				Intake Methods
//					toggleIntakeUp	- intake up or off
//					toggleIntakeDown- intake down or off
//					toggleIntakeOff	- intake off
//					cntrlMoveIntake	- remote control
//				Auton Methods
//					cntrlMoveAuton 	- autonomous task
//					oAutonomous			- old autonomous code
//					nAutonomous 		- incomplete new autonomous code
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

void moveToDistance(int rightDist, int leftDist) {
	rightBasePower = (rightDist - SensorValue[rightSonar])*10;
	leftBasePower = (leftDist - SensorValue[leftSonar])*10;
	int lowerBound = 30;
	while(limiter(rightBasePower,lowerBound) != 0 || limiter(leftBasePower,lowerBound) != 0) {
		motor[right] = limiter(rightBasePower,lowerBound);
		motor[right2] = limiter(rightBasePower,lowerBound);
		motor[left] = limiter(leftBasePower,lowerBound);
		motor[left2] = limiter(leftBasePower,lowerBound);
		rightBasePower = baseTicks - SensorValue[rightEnc];
		leftBasePower = baseTicks - SensorValue[leftEnc];
	}
	motor[right] = 0;
	motor[right2] = 0;
	motor[left] = 0;
	motor[left2] = 0;
}

void ctrlMoveToDistance(int rightDist, int leftDist) {
	rightBasePower = (rightDist - SensorValue[rightSonar])*10;
	leftBasePower = (leftDist - SensorValue[leftSonar])*10;
	int lowerBound = 30;
	while((limiter(rightBasePower,lowerBound) != 0 || limiter(leftBasePower,lowerBound) != 0) && vexRT[Btn8U] == 0) {
		motor[right] = limiter(rightBasePower,lowerBound);
		motor[right2] = limiter(rightBasePower,lowerBound);
		motor[left] = limiter(leftBasePower,lowerBound);
		motor[left2] = limiter(leftBasePower,lowerBound);
		rightBasePower = baseTicks - SensorValue[rightEnc];
		leftBasePower = baseTicks - SensorValue[leftEnc];
	}
	motor[right] = 0;
	motor[right2] = 0;
	motor[left] = 0;
	motor[left2] = 0;
}

void moveDegrees(int degrees) { //Positive degrees turns clockwise
	SensorValue[leftEnc] = 0;
	SensorValue[rightEnc] = 0;
	baseTicks = 3.8 * degrees;
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
	motor[right] = 0;
	motor[right2] = 0;
	motor[left] = 0;
	motor[left2] = 0;
}

void moveInches(int inches) { //Positive moves forward
	SensorValue[leftEnc] = 0;
	SensorValue[rightEnc] = 0;
	baseTicks = 30 * inches;
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
	motor[right] = 0;
	motor[right2] = 0;
	motor[left] = 0;
	motor[left2] = 0;
}

void moveTicks(int rightTicks, int leftTicks) { //Positive moves forward
	SensorValue[leftEnc] = 0;
	SensorValue[rightEnc] = 0;
	needRightTicks = rightTicks - SensorValue[rightEnc];
	needLeftTicks = leftTicks - SensorValue[leftEnc];
	int lowerBound = -127;
	int n = 0;

	if(rightTicks < 0 || leftTicks < 0) {
		n = -1;
	}
	else {
		n = 1;
	}

	if(abs(rightTicks) < abs(leftTicks)) {
		ratio = (float)(abs(rightTicks))/(float)(abs(leftTicks));
		rightBasePower = n * (int)(127*ratio);
		leftBasePower = n * 127;
	}
	else {
		ratio = (float)(abs(leftTicks))/(float)(abs(rightTicks));
		rightBasePower = n * 127;
		leftBasePower = n * (int)(127*ratio);
	}

	while((n == 1 && (needRightTicks > 30 || needLeftTicks > 30)) || (n == -1 && (needRightTicks < -30 || needLeftTicks < -30))) {
		needRightTicks = rightTicks - SensorValue[rightEnc];
		needLeftTicks = leftTicks - SensorValue[leftEnc];
		if((n == 1 && needRightTicks < 30)||(n == -1 && needRightTicks > -30)) {
			motor[right] = 0;
			motor[right2] = 0;
		}
		else {
			motor[right] = limiter(rightBasePower,lowerBound);
			motor[right2] = limiter(rightBasePower,lowerBound);
		}

		if((n == 1 && needLeftTicks < 30)||(n == -1 && needLeftTicks > -30)) {
			motor[left] = 0;
			motor[left2] = 0;
		}
		else {
			motor[left] = limiter(leftBasePower,lowerBound);
			motor[left2] = limiter(leftBasePower,lowerBound);
		}
	}
	motor[right] = 0;
	motor[right2] = 0;
	motor[left] = 0;
	motor[left2] = 0;
}

void autoMoveClaw(int degrees) {
	moveClaw(degrees);
	while(clawPower != 0) {
		wait1Msec(0001);
	}
}

void moveClaw(int degrees) { //Positive moves claw up
	clawState = clawState + 4 * degrees; //around 5 potentiometer ticks per degree (according to calculations)
	if(clawState > 0) {
		clawState = 0;
	}
	else if(clawState < -1080) {
		clawState = -1080;
	}
}

void cntrlMoveClaw() {
	if(vexRT[Btn5DXmtr2] == 1 && clawButton == 0) {
		moveClaw(-30);
		clawButton = 1;
	}
	else if(vexRT[Btn5UXmtr2] == 1 && clawButton == 0) {
		moveClaw(30);
		clawButton = 1;
	}
	else if(vexRT[Btn5DXmtr2] == 0 && vexRT[Btn5UXmtr2] == 0){
		clawButton = 0;
	}

	if(vexRT[Btn8UXmtr2] == 1) {
		SensorValue[clawEnc] = -120;
	}

	clawPower = limiter((int)((clawState - SensorValue[clawEnc])/3),10);

	motor[claw] = clawPower;
}

void autoMoveArm(int degrees) { //Only goes up
	moveArm(degrees);
	while(armPower != 0) {
		wait1Msec(0001);
	}
}

void moveArm(int degrees) { //Positive moves arm up
	leftArmState = leftArmState + 10 * degrees; //around 10 potentiometer ticks per degree (according to calculations)
	rightArmState = rightArmState - 10 * degrees;
}

void cntrlMoveArm() {
	if(vexRT[Btn6DXmtr2] == 1) {
		motor[arm] = -60;
		armButton = 0;
	}
	else if(vexRT[Btn6UXmtr2] == 1) {
		motor[arm] = 127;
		armButton = 2;
	}
	else {

		if(armButton == 2) {
			leftArmState = SensorValue[leftArmPot];
			rightArmState = SensorValue[rightArmPot];
			armButton = 1;
		}

		if(armButton == 1) {
			leftArmPower = limiter((int)((leftArmState - SensorValue[leftArmPot])),15);
			rightArmPower = limiter((int)((SensorValue[rightArmPot] - rightArmState)),15);
			if(leftArmPower > rightArmPower) {
				armPower = leftArmPower;
			}
			else {
				armPower = rightArmPower;
			}

			if(armPower < 15) {
				armPower = 0;
			}

			motor[arm] = armPower;
		}
		else {
			leftArmState = SensorValue[leftArmPot];
			rightArmState = SensorValue[rightArmPot];
			motor[arm] = 0;
		}
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

void toggleIntakeOff() {
	intakeUpState = -1;
	intakeDownState = -1;
}

void cntrlMoveIntake(){
	if(vexRT[Btn6U] == 1 && ballButton == 0) {
		toggleIntakeUp();
		ballButton = 1;
		ball = 1;
	}
	else if(vexRT[Btn6D] == 1 && ballButton == 0) {
		toggleIntakeDown();
		ballButton = 1;
		ball = 1;
	}
	else if(vexRT[Btn6D] == 0 && vexRT[Btn6U] == 0){
		ballButton = 0;
	}

	if(SensorValue[intakeLimit] == 1 && ball == 0) {
		ball = 1;
		intakeUpState = -1;
		intakeDownState = -1;
		motor[intake] = 0;

	}
	else if(intakeUpState == 1){
		motor[intake] = 127;
		if(SensorValue[intakeLimit] == 0) {
			ball = 0;
		}
	}
	else if(intakeDownState == 1){
		motor[intake] = -127;
		if(SensorValue[intakeLimit] == 0) {
			ball = 0;
		}
	}
	else{
		motor[intake] = 0;
	}
}

task moveAuton() {
	while(true) {
		cntrlMoveIntake();
		cntrlMoveClaw();
		cntrlMoveArm();
	}
}

void oAutonomous() {
	moveInches(-12);
	autoMoveArm(10);
	autoMoveClaw(-130);
	wait1Msec(0500);
	moveInches(20);
	wait1Msec(0500);
	moveDegrees(13);
	wait1Msec(0500);
	while(SensorValue[limitSwitch] == 1) {
		motor[mangonel] = 127;
	}
	motor[mangonel] = 0;
	moveInches(36);
	wait1Msec(0500);
	moveDegrees(-13);
	wait1Msec(0250);
	SensorValue[rightEnc] = 0;
	SensorValue[leftEnc] = 0;
	moveTicks(-880,-500);
	moveTicks(-820,-600);
	/*
	toggleIntakeUp();
	while(SensorValue[intakeLimit] != 1) {
		//Do nothing
	}
	toggleIntakeOff(); //Turn intake off
	moveInches(36);
	*/
}

void nAutonomous() {
	//CURRENTLY, THE AUTONOMUS IS MEANT TO COMPLETE SKILLS AUTON FOR ONE SIDE
	//CAN BE BROKEN DOWN TO TO ITS COMPONENT PARTS (FLAG TILE AND CAP TILE) FOR SKILLS
	int r = 0;
	if(SensorValue[autonPot] < 1000) {
		r = -1;
  	}
  else if(SensorValue[autonPot] > 1000 && SensorValue[autonPot] < 3000) {
		//SKILLS AUTON
	}
  else if(SensorValue[autonPot] > 3000) {
		r = 1;
  }

  	//FLAG TILE
	startTask(moveAuton);
	toggleIntakeUp();						//Pick up ball
	moveDegrees(10); //Something weird happening, robot swings right initially, compensated for
	moveInches(-36);
	while(SensorValue[intakeLimit] != 1) {
		//Do nothing
	}
	toggleIntakeOff();
	moveInches(36);							//Move back to starting tile
	moveDegrees(90*r);						//Turn to face flags
	moveInches(-12);						//Move into expansion zone
	autoMoveArm(10);						//Move claw out of the way
	autoMoveClaw(-130);
	moveInches(20);
	moveDegrees(10*r);
	moveToDistance(160,160);				//Aim for high flag
	while(SensorValue[limitSwitch] == 1) { 	//Launch
		motor[mangonel] = 127;
	}
	motor[mangonel] = 0;
	while(SensorValue[limitSwitch] != 1) { 	//Pull back
		motor[mangonel] = 127;
	}
	motor[mangonel] = 0;
	toggleIntakeUp();						//Load
	wait1Msec(2000);
	moveInches(24);
	moveToDistance(70,70);					//Aim for low flag
	while(SensorValue[limitSwitch] == 1) { 	//Launch
		motor[mangonel] = 127;
	}
	motor[mangonel] = 0;
	while(SensorValue[limitSwitch] != 1) { 	//Pull back
		motor[mangonel] = 127;
	}
	motor[mangonel] = 0;
	stopTask(moveAuton);
	moveInches(-24);

	//TRANSITION TO OTHER TILE
	moveInches(-48);

	//CAP TILE
	toggleIntakeUp();						//Pick up ball
	moveInches(-36);
	while(SensorValue[intakeLimit] != 1) {
		//Do nothing
	}
	toggleIntakeOff();
	moveInches(36);							//Move back to starting tile
	moveDegrees(90*r);						//Turn to face flags
	moveInches(24);							//Move to align with alliance tile
	moveDegrees(-90*r);
	moveInches(-48);						//Move to alliance tile
}
