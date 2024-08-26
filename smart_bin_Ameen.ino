#include <Servo.h>   // servo library

Servo servo;
const int trigPin = 5;
const int echoPin = 6;
const int servoPin = 7;
const int ledPin = 10;      // LED for blinking when no object is detected
const int statusLedPin = 9; // LED for indicating cap open status
const int buzzerPin = 8;    // Buzzer pin

long duration, dist;
long aver[3];   // array for average
bool capOpen = false;  // Flag to track cap state

void setup() {
    Serial.begin(9600);
    servo.attach(servoPin);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(statusLedPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    servo.write(150);         // close cap on power on
    delay(100);
    digitalWrite(statusLedPin, LOW); // Turn off status LED initially
    digitalWrite(buzzerPin, LOW);    // Turn off buzzer initially
    Serial.println("Cap is closed");
}

void measure() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(15);
    digitalWrite(trigPin, LOW);
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
    dist = (duration / 2) / 29.1;    // obtain distance in cm
}

void loop() {
    for (int i = 0; i < 3; i++) {   // average distance
        measure();
        aver[i] = dist;
        delay(10);              // delay between measurements
    }
    dist = (aver[0] + aver[1] + aver[2]) / 3;

    if (dist < 15) {  // Object detected, change distance as per your need
        servo.write(0);
        digitalWrite(ledPin, LOW);        // Turn off blinking LED
        digitalWrite(statusLedPin, HIGH); // Turn on status LED
        digitalWrite(buzzerPin, HIGH);    // Turn on buzzer
        if (!capOpen) {
            Serial.println("Cap is open");
            capOpen = true;
        }
    } else {          // Object not detected
        servo.write(150);
        digitalWrite(statusLedPin, LOW);  // Turn off status LED
        digitalWrite(buzzerPin, LOW);     // Turn off buzzer
        if (capOpen) {
            Serial.println("Cap is closed");
            capOpen = false;
        }
        // Start blinking LED
        for (int i = 0; i < 5; i++) { // Adjust the number of blinks as needed
            digitalWrite(ledPin, HIGH);
            delay(100);
            digitalWrite(ledPin, LOW);
            delay(100);
        }
    }

    Serial.print("Distance: ");
    Serial.print(dist);
    Serial.println(" cm");

    delay(100);  // Optional delay to prevent serial flooding
}
