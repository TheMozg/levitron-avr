/* CPU frequency */
#define F_CPU 16000000UL

/* UART baud rate */
#define BAUD  9600

/*
 * hall sensors below and above the solenoid .
 * upper sensor captures only solenoid's own magnetic field.
 * lower sensor captures magnet's field + solenoid's field.
 * thus, difference in readings can be used to get distance to magnet.
 */
#define SENSOR_UP_PIN   DDC0
#define SENSOR_DOWN_PIN DDC1

/*
 * Preset #1: coke can
 */
#define SETPOINT  110
#define PWM_RES   800
#define KP        7
#define KI        0.1
#define KD        20
#define T0_TOP    80
#define NUM_READS 3

/*
 * Preset #2: large magnet
 *
#define SETPOINT  130
#define PWM_RES   800
#define KP        9
#define KI        0.1
#define KD        25
#define T0_TOP    75
#define NUM_READS 4
*/
