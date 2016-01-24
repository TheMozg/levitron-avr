#ifndef PID_H
#define PID_H

typedef struct {
  double setpoint;
  double kp;
  double ki;
  double kd;
  double iterm;
  double prev_input;
  double min;
  double max;
} pid_t;

/*
 * performs first time initialization.
 */
void pid_init(pid_t* pid,
              double setpoint,
              double kp,
              double ki,
              double kd,
              double min,
              double max);

/*
 * does all the magic.
 * should be called at regular intervals.
 */
double pid_compute(pid_t* pid, double input);

/*
 * adjusts output limits.
 * can be called while pid is running.
 */
void pid_set_limits(pid_t* pid, double min, double max);

/*
 * sets pid parameters.
 * can be called while pid is running.
 */
void pid_set_tunings(pid_t* pid, double kp, double ki, double kd);

/*
 * sets desired input (setpoint).
 * can be called while pid is running.
 */
void pid_set_setpoint(pid_t* pid, double setpoint);

#endif
