#ifndef myMotor_H
#define myMotor_H

//typedef enum state {
//	FORWARD,
//	REVERSE,
//	RIGHT,
//	LEFT,
//	LEFTFW,
//	LEFTRV,
//	RIGHTFW,
//	RIGHTRV,
//	HALT
//} state_t;

void stop(void);
void forward(void);
void reverse(void);
void turnLeft(void);
void turnRight(void);
void rightForward(void);
void rightReverse(void);
void leftReverse(void);
void leftForward(void);

#endif
