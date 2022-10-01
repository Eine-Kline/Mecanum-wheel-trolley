#include "pid.h"

static float med(float value,float mx,float mi) {
    if (value < mi)
        return mi;
    if (value > mx)
        return mx;
    return value;
}

float PID_calcDelta(struct PID *pid, float now){
	float delta,pout,iout,dout;
    pid->err = pid->target - now;
    delta = pid->err - pid->lastErr;

    pout = pid->kp * delta;
     iout = pid->ki * pid->err;
     dout = pid->kd * (pid->err - pid->lastErr * 2 + pid->preErr);

    pid->preErr = pid->lastErr;
	pid->lastErr = pid->err;
    pid->out = med(pout + iout + dout, pid->maxOut, pid->minOut);
    return pid->out;
}


void PID_setP(struct PID* pid,float kp) {
	pid->kp = kp;
}

void PID_setI(struct PID* pid,float ki) {
	pid->ki = ki;
}

void PID_setD(struct PID* pid,float kd) {
	pid->kd = kd;
}

void PID_setTarget(struct PID* pid,float target) {
	pid->target = target;
}


void initPID(struct PID* pid,float kp,float ki,float kd,float maxOut,float minOut,float maxSum,float minSum) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
	pid->target = 0;
    pid->err = 0;
    pid->lastErr = 0;
    pid->preErr = 0;
    pid->out = 0;
    pid->maxOut = maxOut;
    pid->minOut = minOut;

	pid->sumErr = 0;
    pid->maxSumErr = maxSum;
    pid->minSumErr = minSum;
}
