#include <WiFi.h>
#include <WiFiUdp.h>
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
float jaw_angle;

unsigned long previous_time;
//Servos for Fin steering 
Servo fin1;


const char* ssid = "ZentinelY2K";
const char* password = "M8!rTz#4qLpX92vB2i#x";

WiFiUDP udp;                           

unsigned int localPort = 4211;   // B listens here
unsigned int remotePort = 4210;  // A listens here

//21,22 for G
IPAddress ESP32_A_IP(10, 182, 195, 157); 

void setup() {
  Wire.begin();

  Serial.begin(115200);
  
  Serial.println("Initializing MPU6050...");
  
  mpu.initialize();
        
  if (mpu.testConnection()) {
        Serial.println("MPU6050 connected!");
  } 

  else {
        Serial.println("MPU6050 connection failed");
  }

 
  
  //wifi and wireless setup
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  udp.begin(localPort);

  Serial.println("\nESP32 B ready");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

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
    
    
    previous_time = millis();

}

float previous_angle_x;
float previous_angle_y;


void loop() {
   
    udp.beginPacket(ESP32_A_IP,remotePort);
    udp.print("Hello");
    udp.endPacket();

     unsigned long currentTime = millis(); //start counting
    float dt = (currentTime - previous_time) / 1000.0; //how much time has passed since last loop
    previous_time = currentTime; //update
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    ax -= new_ax;
    ay -= new_ay;
    az -= new_az;
    gx -= new_gx;
    gy -= new_gy;
    gz -= new_gz;
    
    gx = gx/131.0;
    gy = gy/131.0;
    gz = gz/131.0; //this since one raw value from the gyro measured in LSB is 1 degree per second every 131LSB

    
    previous_angle_x += gx * dt; //how many degrees per second times how much time it lasted
    previous_angle_y += gy * dt;
    
    tilt_angle = atan2(ay,az) * 180/PI; //We give 2 arguments to atan2, y and z from accel, then multiply by 180 over PI since we got Radians
    pitch_angle = atan2(ax, az) * 180/PI; //We give 2 arguments to atan2, x and z from gyro, then multiply by 180 over PI since we got Radians
    jaw_angle = atan2(az,ax) * 180/PI;

    float averaged_values_x = previous_angle_x - tilt_angle;
    float averaged_values_y = previous_angle_y - pitch_angle; //for example if previous angle y is 345 we substract the pitc from accel which is something like 34
    /*
    Dev Note:
    So basically we now have gx and gy for the gyroscope (yaw is kinda useless for now, and very unstable)
    what we're doing with the unsigned long variables is measuring since setup (hence the previous_time = millis() in setup)
    measuring time ever since our program started, then, we get the averaged gx and gy values and divide them by 131.0 to get degrees per second,
    then, we add the previous angle by the degrees per sedcond times time, which means we 'accumulate' values but also those
    values can decrease since they're relative, if dt is too small the value decreases, so we're measuring 'how much have I rolled since the program started?'
    this way, we can have some sort of telemetry, but of course, to get accurate readings, we need to use the precise accelometer values (kalman filter!)    */
    /*Serial.println("X:");
    Serial.println(tilt_angle);
    Serial.println("   ");
    Serial.println("Y:");
    Serial.println(pitch_angle);*/
    
    char user_input = Serial.read();
    Serial.println(user_input);
    if (user_input == 'a'){
          Serial.println("CONTINUE");
    }
    else{
          Serial.println("NO");
    }
    /*Serial.println("Gyro X:");
    Serial.println(previous_angle_x);
    Serial.println("Gyro Y:");
    Serial.println(previous_angle_y);*/
    
    /*Serial.println("Pitch angle: ");
    Serial.println(pitch_angle); 
    Serial.println("   ");
    Serial.println("Tilt_angle:");
    Serial.println(tilt_angle);*/
    delay(500);
      
}