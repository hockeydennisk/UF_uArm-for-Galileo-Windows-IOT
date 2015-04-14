#include "UF_uArm.h"


UF_uArm::UF_uArm()
{
	heightLst = 0;
	height = 0;
	stretch = 0;
	rotation = 0;
	handRot = 0;
}

void UF_uArm::setPosition(double _stretch, double _height, int _armRot, int _handRot)
{
	_armRot = -_armRot;
	//if (!digitalRead(LIMIT_SW) && _height < heightLst) //limit switch protection
	//	_height = heightLst;
	// input limit
	_stretch = constrain(_stretch, ARM_STRETCH_MIN, ARM_STRETCH_MAX) + 68;		// +68, set zero -stretch 
	_height = constrain(_height, ARM_HEIGHT_MIN, ARM_HEIGHT_MAX);
	_armRot = constrain(_armRot, ARM_ROTATION_MIN, ARM_ROTATION_MAX) + 90;		// +90, change -90~90 to 0~180
	_handRot = constrain(_handRot, HAND_ROTATION_MIN, HAND_ROTATION_MAX) + 90;	// +90, change -90~90 to 0~180
	// angle calculation
	double stretch2height2 = _stretch * _stretch + _height * _height;              // 
	double angleA = (acos((ARM_A2B2 - stretch2height2) / ARM_2AB)) * RAD_TO_DEG; // angle between the upper and the lower
	double angleB = (atan(_height / _stretch)) * RAD_TO_DEG;                         // 
	double angleC = (acos((ARM_A2 + stretch2height2 - ARM_B2) / (2 * ARM_A * sqrt(stretch2height2)))) * RAD_TO_DEG; // 
	int angleR = 180 - angleA - angleB - angleC + FIXED_OFFSET_R + offsetR;        // 
	int angleL = angleB + angleC + FIXED_OFFSET_L + offsetL;                       // 
	// angle limit
	angleL = constrain(angleL, 15 + offsetL, 180 + offsetL);
	// set servo position
	servoR.write(angleR);
	servoL.write(angleL);
	servoRot.write(_armRot);
	//servoHandRot.write(_handRot);
	heightLst = _height;
}

void UF_uArm::init()
{
	offsetL = 3;
	offsetR = 3;
	//pinMode(LIMIT_SW, INPUT);  digitalWrite(LIMIT_SW, HIGH);
	//servoL.attach(SERVO_L, D090M_SERVO_MIN_PUL, D090M_SERVO_MAX_PUL);
	//servoR.attach(SERVO_R, D090M_SERVO_MIN_PUL, D090M_SERVO_MAX_PUL);
	//servoRot.attach(SERVO_ROT, D090M_SERVO_MIN_PUL, D090M_SERVO_MAX_PUL);
	//servoHand.attach(SERVO_HAND, D009A_SERVO_MIN_PUL, D009A_SERVO_MAX_PUL);
	//servoHandRot.attach(SERVO_HAND_ROT, D009A_SERVO_MIN_PUL, D009A_SERVO_MAX_PUL);
	Log("attach Start");
	pinMode(PUMP_EN, OUTPUT); digitalWrite(PUMP_EN, LOW);
	pinMode(VALVE_EN, OUTPUT); digitalWrite(VALVE_EN, LOW);
	servoL.attach(SERVO_L);
	servoR.attach(SERVO_R);
	servoRot.attach(SERVO_ROT);
	//servoHand.attach(SERVO_HAND);
	//servoHandRot.attach(SERVO_HAND_ROT);
	Log("attach End");
	setPosition(stretch, height, rotation, handRot);
}

void UF_uArm::gripperCatch()
{
	//servoHand.attach(SERVO_HAND, D009A_SERVO_MIN_PUL, D009A_SERVO_MAX_PUL);
	//servoHand.write(HAND_ANGLE_CLOSE, 0, false);
	digitalWrite(VALVE_EN, LOW); // valve disnable
	digitalWrite(PUMP_EN, HIGH); // pump enable
	gripperRst = true;
}

void UF_uArm::gripperRelease()
{
	if (gripperRst)
	{
		//servoHand.attach(SERVO_HAND, D009A_SERVO_MIN_PUL, D009A_SERVO_MAX_PUL);
		//servoHand.write(HAND_ANGLE_OPEN, 0, false);
		digitalWrite(VALVE_EN, HIGH); // valve enable, decompression
		digitalWrite(PUMP_EN, LOW);   // pump disnable
		gripperRst = false;
		delay_loop = 0;
	}
}

void UF_uArm::gripperDetach()
{
	if (++delay_loop > 300000)        // delay release valve
	{
		//servoHand.detach();
		digitalWrite(VALVE_EN, LOW); // valve disnable
		delay_loop = 0;
	}
}