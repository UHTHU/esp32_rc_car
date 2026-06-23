/*
  ESP32 RC Car: PID line following with ultrasonic obstacle stop

  Hardware:
  - ESP32-DevKitC-32E
  - TB6612FNG dual motor driver
  - Two TCRT5000 analog IR reflectance sensors facing the floor
  - HC-SR04P ultrasonic sensor facing forward

  The car follows a dark line on a light floor. If an obstacle is closer than
  obstacleStopCm, the motors stop until the path is clear.
*/

// Down-facing analog IR sensors. Use ADC1 pins on ESP32.
const int leftIrPin = 34;
const int rightIrPin = 35;

// Forward-facing HC-SR04P ultrasonic sensor.
const int trigPin = 5;
const int echoPin = 18;

// TB6612FNG motor driver pins.
const int leftPwmPin = 25;   // PWMA
const int leftIn1Pin = 26;   // AIN1
const int leftIn2Pin = 27;   // AIN2
const int rightPwmPin = 14;  // PWMB
const int rightIn1Pin = 12;  // BIN1
const int rightIn2Pin = 13;  // BIN2
const int standbyPin = 33;   // STBY

const int leftPwmChannel = 0;
const int rightPwmChannel = 1;
const int pwmFrequency = 20000;
const int pwmResolutionBits = 8;
const int maxMotorSpeed = 255;

// Start slow. Increase only after the car follows the line reliably.
int baseSpeed = 120;

// PID tuning values. These output motor PWM correction values.
// Start with I at 0 for line following.
float kp = 80.0;
float ki = 0.00;
float kd = 8.0;

const float obstacleStopCm = 20.0;

// Update these after reading your own sensors in the Serial Monitor.
// With many TCRT5000 modules, darker surfaces produce lower analog readings.
int leftWhite = 3000;
int leftBlack = 900;
int rightWhite = 3000;
int rightBlack = 900;

float integral = 0.0;
float previousError = 0.0;
unsigned long previousPidTime = 0;

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(leftIn1Pin, OUTPUT);
  pinMode(leftIn2Pin, OUTPUT);
  pinMode(rightIn1Pin, OUTPUT);
  pinMode(rightIn2Pin, OUTPUT);
  pinMode(standbyPin, OUTPUT);
  digitalWrite(standbyPin, HIGH);

  ledcSetup(leftPwmChannel, pwmFrequency, pwmResolutionBits);
  ledcSetup(rightPwmChannel, pwmFrequency, pwmResolutionBits);
  ledcAttachPin(leftPwmPin, leftPwmChannel);
  ledcAttachPin(rightPwmPin, rightPwmChannel);

  analogReadResolution(12);
  previousPidTime = millis();

  stopMotors();
  Serial.println("PID line follower ready.");
}

void loop() {
  float distanceCm = readDistanceCm();

  if (distanceCm > 0 && distanceCm < obstacleStopCm) {
    stopMotors();
    integral = 0.0;
    previousError = 0.0;
    Serial.print("Obstacle detected at ");
    Serial.print(distanceCm);
    Serial.println(" cm");
    delay(50);
    return;
  }

  int leftRaw = analogRead(leftIrPin);
  int rightRaw = analogRead(rightIrPin);

  float leftLine = normalizedLineReading(leftRaw, leftWhite, leftBlack);
  float rightLine = normalizedLineReading(rightRaw, rightWhite, rightBlack);

  // Positive error means the line is more under the right sensor.
  float error = rightLine - leftLine;

  unsigned long now = millis();
  float dt = (now - previousPidTime) / 1000.0;
  if (dt <= 0.0) {
    dt = 0.001;
  }

  integral += error * dt;
  integral = constrain(integral, -2.0, 2.0);

  float derivative = (error - previousError) / dt;
  float correction = (kp * error) + (ki * integral) + (kd * derivative);

  int leftSpeed = baseSpeed + correction;
  int rightSpeed = baseSpeed - correction;

  setMotorSpeeds(leftSpeed, rightSpeed);

  previousError = error;
  previousPidTime = now;

  printDebug(leftRaw, rightRaw, leftLine, rightLine, error, correction, distanceCm);
  delay(10);
}

float normalizedLineReading(int raw, int whiteValue, int blackValue) {
  if (whiteValue == blackValue) {
    return 0.0;
  }

  float normalized = (float)(raw - whiteValue) / (float)(blackValue - whiteValue);
  return constrain(normalized, 0.0, 1.0);
}

float readDistanceCm() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, 25000);
  if (duration == 0) {
    return -1.0;
  }

  return duration / 58.0;
}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  setOneMotor(leftIn1Pin, leftIn2Pin, leftPwmChannel, leftSpeed);
  setOneMotor(rightIn1Pin, rightIn2Pin, rightPwmChannel, rightSpeed);
}

void setOneMotor(int in1Pin, int in2Pin, int pwmChannel, int speed) {
  speed = constrain(speed, -maxMotorSpeed, maxMotorSpeed);

  if (speed > 0) {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    ledcWrite(pwmChannel, speed);
  } else if (speed < 0) {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
    ledcWrite(pwmChannel, -speed);
  } else {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
    ledcWrite(pwmChannel, 0);
  }
}

void stopMotors() {
  setMotorSpeeds(0, 0);
}

void printDebug(int leftRaw, int rightRaw, float leftLine, float rightLine,
                float error, float correction, float distanceCm) {
  static unsigned long previousPrintTime = 0;
  unsigned long now = millis();

  if (now - previousPrintTime < 250) {
    return;
  }

  previousPrintTime = now;

  Serial.print("leftRaw=");
  Serial.print(leftRaw);
  Serial.print(" rightRaw=");
  Serial.print(rightRaw);
  Serial.print(" leftLine=");
  Serial.print(leftLine, 2);
  Serial.print(" rightLine=");
  Serial.print(rightLine, 2);
  Serial.print(" error=");
  Serial.print(error, 2);
  Serial.print(" correction=");
  Serial.print(correction, 2);
  Serial.print(" distanceCm=");
  Serial.println(distanceCm, 1);
}
