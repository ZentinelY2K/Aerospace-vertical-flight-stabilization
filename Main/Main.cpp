
#include <Wire.h>
#include "I2Cdev.h"
#include <MPU6050.h>
#include <Arduino.h>
#include <ESP32Servo.h>

MPU6050 mpu;

int16_t ax, ay, az;
int16_t gx, gy, gz;
//make lists to append values then sum then divide by total numbers for average across 600 trials, because then it exceeds available memory, probably should upgrade to
//Raspberry pi bruh like what the hell is this now i cant even make arrays? What is this, the 80's?

int countgx = 0;

int countgy = 0;

int countgz = 0; 

int countax = 0;

int countay = 0;

int countaz = 0;

int new_ax;
int new_ay;
int new_az;    
int new_gx;
int new_gy;
int new_gz;

float tilt_angle;
float pitch_angle;



//Servos for Fin steering 
Servo fin1;

void setup() { 
    Wire.begin();
    Serial.begin(115200);
    //attach Servos to pins
    fin1.attach(16);
   

    Serial.println("Initializing MPU6050...");
    mpu.initialize();

    if (mpu.testConnection()) {
        Serial.println("MPU6050 connected!");
    } else {
        Serial.println("MPU6050 connection failed");
    }

    //get data
    for (int i = 0; i<=5000; i++){ //increase i<=x for more trials
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      countgx += gx;
      countgy += gy;
      countgz += gz;
      countax += ax;
      countay += ay;
      countaz += az;    
    }
    //Decided to remove unecessary loop, since we will know the number of iterations ourselves, which is 1200.
    int size_of_everyone = 5000; 
    //We divide the total sum of the readings by the number of iterations to get an average and store it in a variable
    int new_ax = countax/size_of_everyone;
    int new_ay = countay/size_of_everyone;
    int new_az = countaz/size_of_everyone;
    int new_gx = countgx/size_of_everyone;
    int new_gy = countgy/size_of_everyone;
    int new_gz = countgz/size_of_everyone;
    
    
   
}

void loop() {
    
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    ax -= new_ax;
    ay -= new_ay;
    az -= new_az;
    gx -= new_gx;
    gy -= new_gy;
    gz -= new_gz;
    
    tilt_angle = atan2(ay,az) * 180/PI; //We give 2 arguments to atan2, y and z from accel, then multiply by 180 over PI since we got Radians
    pitch_angle = atan2(ax, az) * 180/PI; //We give 2 arguments to atan2, x and z from gyro, then multiply by 180 over PI since we got Radians
    
    Serial.println("Pitch angle: ");
    Serial.println(pitch_angle);    
    Serial.println("   ");
    Serial.println("Tilt_angle:");
    Serial.println(tilt_angle);
   
   
    delay(200);

}

void kalman_filter(){

}