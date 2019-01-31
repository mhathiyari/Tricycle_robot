#ifndef NAVIGATION_MODULE
#define NAVIGATION_MODULE
//PID Tunning params
#define Kd (1)
#define Kp (1)
#define Ki (1)

//Robot params
#define Len (5)
#define Rad (1)

void navigation_setup();
void navigation();

#endif