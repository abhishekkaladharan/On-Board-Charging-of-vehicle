/*
  INTERLEAVED BOOST CONVERTER
  Arduino UNO
*/

#define DUTY_PERCENT 80

void setup() {

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  // Fast PWM Mode 14
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13);

  // D9 normal
  TCCR1A |= (1 << COM1A1);

  // D10 inverted
  TCCR1A |= (1 << COM1B1) | (1 << COM1B0);

  // 20kHz
  ICR1 = 799;

  uint16_t duty = (ICR1 * DUTY_PERCENT) / 100;

  OCR1A = duty;
  OCR1B = duty;

  TCCR1B |= (1 << CS10);
}

void loop() {}