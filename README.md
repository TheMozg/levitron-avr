# About
Digital AVR brain for my levitron powered by custom [PID controller](https://en.wikipedia.org/wiki/PID_controller).

http://geektimes.ru/post/252110/

# Hardware
I used Arduino Uno r3 for prototyping. Connections:
* Vin - power
* GND - ground
* A0 pin - upper hall sensor
* A1 pin - lower hall sensor
* 9~ pwm pin - pwm output

# Getting code to AVR

## Build
Install the following packages:
* `avr-gcc`
* `avr-libc`
* `avrdude`

Then run `make` to build `.hex` file.

## Upload
You can use any avr isp programmer supported by `avrdude`.
By default it is 'avrispmkII'. To use other programmer simply modify the makefile.

Connect the programmer and run `make flash` to upload `.hex` file to AVR.
