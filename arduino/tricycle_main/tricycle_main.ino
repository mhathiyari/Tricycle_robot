/**************************************************
 * Main function for the mega on the the Tricycle_bot
 */
#include <Tasks.h> // https://github.com/chrispbarlow/arduino-tasks
#include<ros_subscribe_module>
#include<ros_publish_module>
#include<navigation_module>

/* Tasks are added to the schedule here in the form addTask(task_function_name, task_period, task_offset) */
void setup() {
  String addedTasks;
  /* Configure the serial port to 9600 baud - used for reportAddedTask and the statusOut task */
  Serial.begin(9600);
  ros_subscribe_setup();

  /* Create a schedule with 3 tasks */
  Schedule.begin(3);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Schedule.addTask("ROS Subscribe", ros_subscribe, 0, 100);
  Serial.print(Schedule.lastAddedTask());
  Schedule.addTask("Navigation", navigation, 0, 100);
  Serial.print(Schedule.lastAddedTask());
  Schedule.addTask("ROS Publish", ros_publish, 0, 100);
  Serial.print(Schedule.lastAddedTask());

  /* Starting the scheduler with a tick length of 1 millisecond */
  Schedule.startTicks(1);

  /* Some error checks */
  if(Schedule.checkTooManyTasks() == true){
    Serial.println("Too many tasks");
  }

  if(Schedule.checkTicksTooLong() == true){
    Serial.println("Ticks too long");
  }
}

/* It's best not to do anything in loop() except runTasks() - doing anything else here will affect timing */
void loop() {
  Schedule.runTasks();
}

