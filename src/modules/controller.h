#ifndef controller_h
#define controller_h

#include "mbed.h"

#include "src/config/parameters.h"

// Speed estimator class
class Controller
{
  public:
    // Constructor
    Controller();
    // Control step
    void control(float theta_s, float omega_s, float theta_w, float omega_w);
    // Torque
    float tau;
  private:
    // State regulator step
    void state_regulator(float theta_s, float omega_s, float theta_w, float omega_w);
    // Feedback linearization step
    void feedback_linearization(float theta_s, float omega_w);
    // Linearized input
    float u;

};

#endif