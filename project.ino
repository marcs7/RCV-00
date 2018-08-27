#include <XBOXRECV.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
XBOXRECV Xbox(&Usb);

// motor pin

int m1 = 43; // ChA front motor | pin 12 shield moved on pin 43 arduino mega
int m2 = 42; // ChB rear motor | pin 13 shield moved on 42 arduino mega

// declaration function for motion control


int left;
int right;

// controller button
int LeftPadX;
int triggerR;
int triggerL;
int ButtonA;
int ButtonB;

// function motor control

// rear motor

void forward(int motor_speed) {
  if (motor_speed < 0) {}
  else {
    digitalWrite(m2, HIGH);
    analogWrite(45, motor_speed); // pin 11 PWM shield moved on pin 45 PWM (mega)
    Serial.print("speedA ");
    Serial.println(motor_speed);
  }

}

void backward(int motor_speed) {
  if (motor_speed < 0) {}
  else {
    digitalWrite(m2, LOW);
    analogWrite(45, motor_speed); // pin 11 PWM shield moved on pin 45 PWM (mega)
    Serial.print("speedI ");
    Serial.println(motor_speed);
  }

}

// front motor
void LT(int motor_speed) {
  if (motor_speed < 0) {}
  else {
    digitalWrite(m1, HIGH);
    analogWrite(44, motor_speed); // pin 3 PWM shield move on 44 PWM (mega)
    Serial.print("speedL ");
    Serial.println(motor_speed);
  }

}

void RH(int motor_speed) {
  if (motor_speed < 0) {}
  else {
    digitalWrite(m1, LOW);
    analogWrite(44, motor_speed); // pin 3 PWM shield moved on 44 PWM (mega)
    Serial.print("speedR ");
    Serial.println(motor_speed);
  }

}

void setup() {

  // initialize motor A
  pinMode(m1, OUTPUT);

  // initialize motor B
  pinMode(m2, OUTPUT);

  Serial.begin(115200);

#if !defined(__MIPSEL__)
  while (!Serial);
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));

}

void loop() {

  Usb.Task();

  // start controller communication
  if (Xbox.XboxReceiverConnected) {
    for (uint8_t i = 0; i < 4; i++) {
      if (Xbox.Xbox360Connected[i]) {

        // control left and right motion with lefthat

        LeftPadX = Xbox.getAnalogHat(LeftHatX, i);

        if (LeftPadX < -7500) {
          Serial.println(LeftPadX);
          right = map(LeftPadX, -7500, -32768, 0, 255);
          RH(right);
        }

        if (LeftPadX > 7500) {
          Serial.println(LeftPadX);
          left = map (LeftPadX, 7500, 32768, 0, 255);
          LT(left);
        }

        // control forward and backward with trigger of controller

        triggerL = Xbox.getButtonPress(L2, i);
        if (triggerL > 0) {
          backward(triggerL);
        }
        triggerR = Xbox.getButtonPress(R2, i);
        if (triggerR > 0) {
          forward(triggerR);
        }

        // brake motor

        ButtonA = Xbox.getButtonClick(A);
        ButtonB = Xbox.getButtonClick(B);

        if (ButtonA) {
          analogWrite(45, 0);
          Serial.println("breakA");
        }

        if (ButtonB) {
          analogWrite(44, 0);
          Serial.println("breakB");
        }
      }
    }
  }
}




