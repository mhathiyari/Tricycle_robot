#include "navigation_module.h"
#include "ros_subcribe_module.h"
#include "math.h"

static TSPose prev_pose;
static prev_er_phi;
static int vel;
void navigation_setup()
{
prev_pose.x = 0;
prev_pose.y = 0;
prev_pose.phi = 0;
prev_er_phi = 0;
vel = 5;
}
void navigation()
{
    int dt = prev_millis-millis();

    float er_phi = prev_pose.phi - pose.phi;
    er_phi = atan2(sin(er_phi),cos(er_phi));
    float er_phi_d = (prev_er_phi- er_phi)/dt;
    float er_phi_i = prev_er_phi_i+er_phi;
    omega = Kp*er_phi+Kd*er_phi_d+Ki*er_phi_i;

    vr = (2*vel+omega*Len)/(2*Rad);
    vl = (2*vel-omega*Len)/(2*Rad);

    pwm_r = PWM_CONST*vr;
    pwm_l = PWM_CONST*vl;

    analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);


}