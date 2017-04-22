
byte PWM_PIN1 = 5;
byte PWM_PIN2 = 6;

float pwm_value1;
float pwm_value2;
void setup() {
  pinMode(PWM_PIN1, INPUT);
  pinMode(PWM_PIN2, INPUT);
  Serial.begin(9600);
}

void loop() {
  int timeStart1 = micros();
  pwm_value1 = pulseIn(PWM_PIN1, HIGH);
  int timeEnd1 = micros();

  int timeStart2 = micros();
  pwm_value2 = pulseIn(PWM_PIN2, HIGH);
  int timeEnd2 = micros();

  pwm_value1 = pwm_value1/10;
  pwm_value2 = pwm_value2/10;
  Serial.print(pwm_value1);
  Serial.print(" ");
  Serial.println(pwm_value2);

  Serial.print("time1: ");
  Serial.println(timeEnd1-timeStart1);
  Serial.print("time2: ");
  Serial.println(timeEnd2-timeStart2);
  delay(1000);
}
