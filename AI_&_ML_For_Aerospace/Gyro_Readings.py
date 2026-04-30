import csv
import random as rm
import serial

arduino = serial.Serial('/dev/ttyUSB0', 115200)  # Windows: COM3 | Linux: /dev/ttyUSB0

while True:
    line = arduino.readline().decode().strip()
    print(line)


#with open("Gyro_Readings", "w", newline="") as f:
#   writer = csv.writer(f)
    
 