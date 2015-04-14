#include "arduino.h"
#include "Servo.h"

#define ARM_A                   148    // upper arm
#define ARM_B                   160    // lower arm
#define ARM_2AB                 47360  // 2*A*B
#define ARM_A2                  21904  // A^2
#define ARM_B2                  25600  // B^2
#define ARM_A2B2                47504  // A^2 + B^2
#define ARM_STRETCH_MIN         0
#define ARM_STRETCH_MAX         195
#define ARM_HEIGHT_MIN          -150
#define ARM_HEIGHT_MAX          160
#define ARM_ROTATION_MIN        -90
#define ARM_ROTATION_MAX        90
#define HAND_ROTATION_MIN       -90
#define HAND_ROTATION_MAX       90
#define RAD_TO_DEG				57.2957786
#define FIXED_OFFSET_L          25
#define FIXED_OFFSET_R          42
#define D090M_SERVO_MIN_PUL     500
#define D090M_SERVO_MAX_PUL     2500
#define D009A_SERVO_MIN_PUL     600
#define D009A_SERVO_MAX_PUL     2550
#define CATCH                   0x01
#define RELEASE                 0x02
#define RESET                   0x80


#define VALVE_EN                5     //
#define PUMP_EN                 6     //
#define LIMIT_SW                2     // Limit Switch
//#define SERVO_HAND              9     //
//#define SERVO_HAND_ROT          10    //
#define SERVO_ROT               9    //
#define SERVO_R                 10    //
#define SERVO_L                 11    //



class UF_uArm
{
public:
	void setPosition(double _stretch, double _height, int _armRot, int _handRot);
	void init();
	UF_uArm();
	void gripperCatch();    //
	void gripperRelease(); 
	void gripperDetach();   //

private:
	/*******************  Servo offset  *******************/
	char offsetL;
	char offsetR;
	/*****************  Define variables  *****************/
	int heightLst;
	int height;
	int stretch;
	int rotation;
	int handRot;
	boolean gripperRst;
	unsigned long delay_loop;
	   

	Servo servoR;
	Servo servoL;
	Servo servoRot;
	//Servo servoHand;
	//Servo servoHandRot;
};