#include <Wire.h>
#include "I2Cdev.h"
#include <MPU6050.h>
#include <Arduino.h>

MPU6050 mpu;

int16_t ax, ay, az;
int16_t gx, gy, gz;
//make lists to append values then sum then divide by total numbers for average across 600 trials, because then it exceeds available memory, probably should upgrade to
//Raspberry pi bruh like what the hell is this now i cant even make arrays? What is this, the 80's?
int gx_values[1200];
int countgx = 0;

int gy_values[1200];
int countgy = 0;

int gz_values[1200];
int countgz = 0;

int average_x;
int average_y;
int average_z;

int sum_x = 0;
int sum_y = 0;
int sum_z = 0;

void setup() {
    Wire.begin();
    Serial.begin(115200);
    
    Serial.println("Initializing MPU6050...");
    mpu.initialize();

    if (mpu.testConnection()) {
        Serial.println("MPU6050 connected!");
    } else {
        Serial.println("MPU6050 connection failed");
    }

    //get data
    for (int i = 0; i<=1200; i++){
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      gx_values[countgx++] = gx;
      gy_values[countgy++] = gy;
      gz_values[countgz++] = gz;
    }

    int size_of_everyone = 1200;

    for (int l = 0;l<=size_of_everyone;l++){ //all the lenghts should be same cuz they are SUPPOSED to have the same amount of trials (1200 instead of 600 for some reason)
        sum_x += gx_values[l];
        sum_y += gy_values[l];
        sum_z += gz_values[l];
    }

    average_x = sum_x/size_of_everyone;
    average_y = sum_y/size_of_everyone;
    average_z = sum_z/size_of_everyone;
   
}

void loop() {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    int new_gx = (gx-average_x/1000000);
    int new_gz = gz-average_z;
    int new_gy = (gy-average_y)-new_gx+new_gz*0.08;

    Serial.println("Accel: ");
    Serial.println(ax); 
    Serial.println(ay);
    Serial.println(az);
    Serial.print("Gyro: ");
    Serial.print(" Roll: "); Serial.print(new_gx);
    Serial.print(" Pitch: "); Serial.print(new_gy);
    Serial.print(" Yaw: "); Serial.println(new_gz);
    delay(200);

   /*if(new_gz <= -4000){
    Serial.println("Rocket State: Going East");
   }
   if(new_gz >= 4000){
    Serial.println("Rocket State: Going West");
   }*/

   /*if (gy >= 2000){
    Serial.println("Rocket State: Going Up!");
   }
   else if(gy <= -2000){
    Serial.println("Rocket State: Going Down!");
   }

   if(gx >= 0 || gx <= -500){
    Serial.println("Rocket State: Rolling");
   }*/



   
}