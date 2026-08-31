#include <Wire.h>
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

const int fsrPin = A0;     // FSR analog input
const int motorPin = 9;    // Vibration motor output
const int ledPin = 12;     // LED output
const int buzzerPin = 10;

int gripValue = 0;
float tiltAngle = 0;

const int gripThreshold = 800;   // Lower value = weak grip
const int tiltThreshold = 100;    // Degrees from vertical

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(fsrPin, INPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin,OUTPUT);

  if(!accel.begin()) {
    Serial.println("ADXL345 not detected!");
    while(1);
  }

  accel.setRange(ADXL345_RANGE_2_G);
  Serial.println("CaneSentinel Combined Test Ready");
}

void loop() {
  sensors_event_t event;
  accel.getEvent(&event);

  // Calculate tilt angle relative to vertical
  float x = event.acceleration.x;
  float y = event.acceleration.y;
  float z = event.acceleration.z;
  tiltAngle = atan2(sqrt(x * x + y * y), z) * 180.0 / PI;

  // Read grip pressure
  gripValue = analogRead(fsrPin);

  // Show live readings
  Serial.print("Tilt: ");
  Serial.print(tiltAngle);
  Serial.print("° | Grip: ");
  Serial.println(gripValue);

  if(tiltAngle < 50 ){
    Serial.println("⚠️  WARNING: Tilt");
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(500);
    
  }else{
    digitalWrite(buzzerPin,LOW);
    digitalWrite(ledPin, LOW);
  }

  // Main logic
  if (gripValue < gripThreshold || (tiltAngle > 100 || tiltAngle < 80) ) {
    Serial.println("⚠️ Weak Grip Detected!");
    digitalWrite(motorPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(1000); // vibrate for 1.5s
  } else {
    digitalWrite(motorPin, LOW);
    digitalWrite(ledPin, LOW);
  }

  delay(200);
}
