#include "pid.h"

void pid_init(pid_t* pid,
              double setpoint,
              double kp,
              double ki,
              double kd,
              double min,
              double max) {

  pid->iterm = min;
  pid->prev_input = 0;

  pid_set_setpoint(pid, setpoint);
  pid_set_limits(pid, min, max);
  pid_set_tunings(pid, kp, ki, kd);
}

static double constrain(double value, double min, double max){
  if(value > max)
    value = max;
  else if(value < min)
    value = min;
  return value;
}

double pid_compute(pid_t* pid, double input) {
  double error = pid->setpoint - input;

  pid->iterm = constrain(pid->iterm + pid->ki * error, pid->min, pid->max);

  double diff_input = input - pid->prev_input;
  pid->prev_input = input;

  double output = pid->kp * error + pid->iterm - pid->kd * diff_input;
  output = constrain(output, pid->min, pid->max);
  return output;
}

void pid_set_setpoint(pid_t* pid, double setpoint){
  pid->setpoint = setpoint;
}

void pid_set_tunings(pid_t* pid, double kp, double ki, double kd){
  if (kp < 0 || ki < 0 || kd < 0)
    return;

  pid->kp = kp;
  pid->ki = ki;
  pid->kd = kd;
}

void pid_set_limits(pid_t* pid, double min, double max) {
  if (min >= max)
    return;

  pid->min = min;
  pid->max = max;

  pid->iterm = constrain(pid->iterm, pid->min, pid->max);
}
