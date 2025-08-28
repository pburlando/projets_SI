#include "Arduino.h"

const int pwm_pin = 6;
const int bp_pin = 2;
const int pot_pin = A0;
const int led_pin = 4;
const int buzzer = 5;
int enable = 0;

void beep(int nb);

void setup() {
  // put your setup code here, to run once:

  TCCR0B = TCCR0B & B11111000 | B00000010; // for PWM frequency of 7812.50 Hz
  //TCCR0B = TCCR0B & B11111000 | B00000001; // for PWM frequency of 62500.00 Hz
  pinMode(pwm_pin, OUTPUT);
  pinMode(bp_pin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pot_pin, INPUT);
  digitalWrite(led_pin, HIGH);
  Serial.begin(9600);
  beep(5);  // 2s
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led_pin, !enable);  // etat de la commande du moteur
  int etat_BP = digitalRead(bp_pin);
  if (etat_BP == 0) {
    enable = !enable;
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    while (etat_BP == 0) etat_BP = digitalRead(bp_pin);
  }
  if (enable) {
    int consigne = analogRead(pot_pin);
    int pwm = map(consigne, 0, 1023, 150, 0);
    analogWrite(pwm_pin, pwm);
    analogWrite(LED_BUILTIN, pwm);
    Serial.println(pwm);
  }
  else {
    analogWrite(pwm_pin, 0);
    analogWrite(LED_BUILTIN, 0);
  }
  
}

void beep(int nb) {
  for(int i=0; i < nb; i++) {
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
  }
}
