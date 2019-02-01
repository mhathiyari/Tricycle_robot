#include "ros_publish_module.h"
#include "ros.h"
#include "sensor_msgs/Imu.h"
#include "sensor_msgs/MagneticField.h"
#include "MPU9250.h"

#include <std_msgs/String.h>

ros::NodeHandle  nh;
std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

char hello[13] = "hello world!";

void ros_publish_setup()
{
  nh.initNode();
  nh.advertise(chatter);
  pinMode(LED_BUILTIN, OUTPUT);

}

void ros_publish()
{
  str_msg.data = hello;
  chatter.publish( &str_msg );
  nh.spinOnce();
  digitalWrite(LED_BUILTIN, HIGH);

}


/*sensor_msgs::Imu imu_msg_;
sensor_msgs::MagneticField mag_msg_;

ros::Publisher imu_data("imu_data",&imu_msg_);
ros::Publisher mag_data("mag_data",&mag_msg_);

void setup() {
  MPU9250::setDlpfBandwidth(DLPF_BANDWIDTH_92HZ);
  // start communication with IMU
  status = IMU.begin();

  if (status < 0)
  {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  nh.initNode();
  nh.advertise(mag_data);
  nh.advertise(imu_data);
}

void loop() {
  // read the sensor
  IMU.readSensor();
  // display the data

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

  nh.spinOnce();
}
*/