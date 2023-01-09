#include <Wire.h>
#include <EasyUltrasonic.h>

// variables for ultrasonic
EasyUltrasonic ultrasonic;
int ultrasonicDistance;

int slowSpeed = 120;
int slowSpeedneg = -120;
int highSpeed = 255;
int highSpeedneg = -255;

#define SWA 85

// arduino address
#define I2C_SLAVE_ADDR 0x04 // 4 in hexadecimal

void setup()
{
    // allows communication with computer
    Serial.begin(9600);
    // allows communication with mpu250 and arduino
    Wire.begin();

    // setup ultrasonic sesor
    ultrasonic.attach(14, 12);
}

void loop()
{
    // put your main code here, to run repeatedly:
    reverseToWall()
        goParalell();
    reverseToWall()
        // allows the code to repeat every 5 seconds so that testing is easier
        delay(5000);
}

void reverseToWall()
{
    int stop = 0;
    while (stop == 0)
    {
        ultrasonicDistance = ultrasonic.getDistanceCM();
        // Serial.println(ultrasonicDistance);
        if (ultrasonicDistance > 100)
        {
            Serial.println("high");
            sendDataToArduino(highSpeedneg, highSpeedneg, SWA);
        }
        else if (ultrasonicDistance > 10)
        {
            Serial.println("Slow");
            sendDataToArduino(slowSpeedneg, slowSpeedneg, SWA);
        }
        else
        {
            Serial.println("stop");
            stop = 1;
            sendDataToArduino(0, 0, SWA);
        }
    }
}

void goParalell()
{
    sendDataToArduino(slowSpeed, slowSpeed, SWA);
    delay(1000);
    sendDataToArduino(0, 0, SWA);
    turnUsingtime();
}
void turnUsingtime()
{
    sendDataToArduino(slowSpeedneg, slowSpeedneg, 155);
    delay(2000);
    sendDataToArduino(0, 0, SWA);
}

void sendDataToArduino(int leftMotor, int rightMotor, int steeringAngle)
{
    if (leftMotor > 255)
        leftMotor = 255;
    if (rightMotor > 255)
        rightMotor = 255;
    Wire.beginTransmission(I2C_SLAVE_ADDR); // transmit to device #4
    Wire.write((byte)((leftMotor & 0x0000FF00) >> 8));
    Wire.write((byte)(leftMotor & 0x000000FF));
    Wire.write((byte)((rightMotor & 0x0000FF00) >> 8));
    Wire.write((byte)(rightMotor & 0x000000FF));
    Wire.write((byte)((steeringAngle & 0x0000FF00) >> 8));
    Wire.write((byte)(steeringAngle & 0x000000FF));
    Wire.endTransmission(); // stop transmitting
    delay(100);
}