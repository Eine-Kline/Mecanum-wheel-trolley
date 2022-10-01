//
// Created by 周蜀杰 on 2020/12/9.
//

#ifndef __PID_H
#define __PID_H
#ifdef __cplusplus
extern "C" {
#endif

//#include "main.h"

struct PID {
    float kp,ki,kd;

    float err//0
    ,lastErr//1
    ,preErr//2
    ,sumErr;

    float maxOut,minOut;

    float maxSumErr,minSumErr;

    float out;

	float target;
};

float PID_calcDelta(struct PID*,float);
float PID_calcPosition(struct PID*,float);

void PID_setP(struct PID* pid,float);
void PID_setI(struct PID* pid,float);
void PID_setD(struct PID* pid,float);
void PID_setTarget(struct PID* pid,float);

void initPID(struct PID* pid,float _kp,float _ki,float _kd,float maxOut,float minOut,float maxSum,float minSum);

#ifdef __cplusplus
}
#endif
#endif //__PID_H
