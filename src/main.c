#include "defines.h"

#include <avr/io.h>
#include <util/delay.h>

#include "pid.h"
#include "uart.h"

/* enables/disables HIGH output on pwm pin */
#define ENABLE_PWM_PIN()  DDRB = _BV(DDB1)
#define DISABLE_PWM_PIN() DDRB &= ~(_BV(DDB1))

/* set up io streams used by uart, avoids malloc() calls */
FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

void pwm_init(){
  /* set up timer1 in pwm phase correct mode */
  TCCR1B |= _BV(WGM13);
  TCCR1A |= _BV(WGM11);
  /* connect OC1A pin */
  TCCR1A |= _BV(COM1A1);
  /* initialize top timer1 value */
  ICR1 = PWM_RES;
  /* start timer1 with no prescaling */
  TCCR1B |= _BV(CS10);
}

void adc_init() {
  /* set adc reference voltage to AVcc */
  ADMUX = _BV(REFS0);
  /* set 128 prescaling for max accuracy*/
  ADCSRA |= _BV(ADPS2)| _BV(ADPS1) | _BV(ADPS0);
  /* enable adc */
  ADCSRA |= _BV(ADEN);
}

int adc_read(uint8_t ch) {
  /* select channel */
  ch &= 0b00000111;
  ADMUX = (ADMUX & 0xF8)|ch;
  /* start conversion */
  ADCSRA |= _BV(ADSC);
  /* wait for conversion to complete */
  loop_until_bit_is_clear(ADCSRA, ADSC);
  return ADC;
}

void timer0_init() {
  /* set up timer0 with 1024 prescaling */
  TCCR0B = _BV(CS02) | _BV(CS00);
  /* initialize counter */
  TCNT0 = 0;
}

int main(void) {
  /* set pid controller parameters */
  pid_t pid;
  pid_init(&pid, SETPOINT, KP, KI, KD, 0, PWM_RES);

  /* enable serial communication via usb */
  uart_init();
  /* use uart streams for standard io functions such as printf */
  stdout = &uart_output;
  stdin  = &uart_input;

  /* enable reading sensor values */
  adc_init();

  /* start timer1 in pwm mode */
  pwm_init();
  /* enable HIGH output on pwm pin */
  ENABLE_PWM_PIN();

  /* start timer0 */
  timer0_init();

  while (1) {
    /* sample sensor values at regular intervals */
    if (TCNT0 >= T0_TOP){
      /* reset timer0 immediately - adc & pid don't affect sample time */
      TCNT0 = 0;

      int up_value = 0;
      int down_value = 0;
      /* temporary disable output to reduce adc noise, then read sensors */
      DISABLE_PWM_PIN();
      size_t i;
      for (i = 0; i < NUM_READS; i++) {
        up_value    += adc_read(SENSOR_UP_PIN);
      }
      for (i = 0; i < NUM_READS; i++) {
        down_value  += adc_read(SENSOR_DOWN_PIN);
      }
      ENABLE_PWM_PIN();

      /* get average of multiple readings to compensate for adc noise */
      up_value /= NUM_READS;
      down_value /= NUM_READS;

      /* compensate for solenoid's own magnetic field */
      int dist = down_value - up_value;
      /* set pwm duty cycle using pid controller */
      int output = pid_compute(&pid, dist);
      OCR1A = output;
    }
  }
}
