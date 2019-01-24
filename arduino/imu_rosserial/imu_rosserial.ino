
 /*
 * Library: https://github.com/bolderflight/MPU9250
Basic_I2C.ino
Brian R Taylor
brian.taylor@bolderflight.com

Copyright (c) 2017 Bolder Flight Systems

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, 
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*
 * Updated by Ahmad Shamshiri on July 09, 2018 for Robojax.com
 * in Ajax, Ontario, Canada
 * watch instrucion video for this code: 
For this sketch you need to connect:
VCC to 5V and GND to GND of Arduino
SDA to A4 and SCL to A5

S20A is 3.3V voltage regulator MIC5205-3.3BM5
*/
#include <ros.h>
#include "sensor_msgs/Imu.h"
#include "sensor_msgs/MagneticField.h"
#include "MPU9250.h"
ros::NodeHandle  nh;

sensor_msgs::Imu imu_msg_;
sensor_msgs::MagneticField mag_msg_;

ros::Publisher imu_data("imu_data",&imu_msg_);
ros::Publisher mag_data("mag_data",&mag_msg_);

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire,0x68);
int status;

void setup() {
  // serial to display data
  ///Serial.begin(115200);
  //while(!Serial) {}
  MPU9250::setDlpfBandwidth(DLPF_BANDWIDTH_92HZ);
  // start communication with IMU 
  status = IMU.begin();

  /*if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }*/
  nh.initNode();
  nh.advertise(mag_data);
  nh.advertise(imu_data);
}

void loop() {
  // read the sensor
  IMU.readSensor();
  // display the data
//  ros::Time current_time = millis();//ros::Time::now();

  imu_msg_.header.stamp = nh.now();
  imu_msg_.linear_acceleration.x = IMU.getAccelX_mss();
  imu_msg_.linear_acceleration.y = IMU.getAccelY_mss();
  imu_msg_.linear_acceleration.z = IMU.getAccelZ_mss();
  imu_msg_.angular_velocity.x = IMU.getGyroX_rads();
  imu_msg_.angular_velocity.y = IMU.getGyroY_rads();
  imu_msg_.angular_velocity.z = IMU.getGyroZ_rads();
  imu_data.publish(&imu_msg_);

  mag_msg_.header.stamp = nh.now();
  mag_msg_.magnetic_field.x = IMU.getMagX_uT();
  mag_msg_.magnetic_field.y = IMU.getMagY_uT();
  mag_msg_.magnetic_field.z = IMU.getMagZ_uT();
  mag_data.publish(&mag_msg_);
  
  /*Serial.print("AccelX: ");
  Serial.print(IMU.getAccelX_mss(),6);
  Serial.print("  ");
  Serial.print("AccelY: ");  
  Serial.print(IMU.getAccelY_mss(),6);
  Serial.print("  ");
  Serial.print("AccelZ: ");  
  Serial.println(IMU.getAccelZ_mss(),6);
  
  Serial.print("GyroX: ");
  Serial.print(IMU.getGyroX_rads(),6);
  Serial.print("  ");
  Serial.print("GyroY: ");  
  Serial.print(IMU.getGyroY_rads(),6);
  Serial.print("  ");
  Serial.print("GyroZ: ");  
  Serial.println(IMU.getGyroZ_rads(),6);

  Serial.print("MagX: ");  
  Serial.print(IMU.getMagX_uT(),6);
  Serial.print("  ");  
  Serial.print("MagY: ");
  Serial.print(IMU.getMagY_uT(),6);
  Serial.print("  ");
  Serial.print("MagZ: ");  
  Serial.println(IMU.getMagZ_uT(),6);
  
  Serial.print("Temperature in C: ");
  Serial.println(IMU.getTemperature_C(),6);
  Serial.println();
  delay(200);*/
  nh.spinOnce();
  delay(10);
} 
