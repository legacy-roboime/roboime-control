#include "pid_controller.h"

#include <cmath>

namespace roboime
{
    pid_controller::pid_controller(float kp, float ki, float kd, float max_output, float max_integ) :
        kp(kp),
        ki(ki),
        kd(kd),
        max_output(max_output),
        max_integ(max_integ)
    {}

    void
    pid_controller::step(void)
    {
        auto error = input - feedback;
        int_err += error;
        if (ki > 0)
        {
            int_err = std::fmin(std::fmax(int_err, -std::abs(max_integ) / ki), std::abs(max_integ) / ki);
        }
        diff_err = error - prev_err;
        prev_err = error;

        output = kp * error + ki * int_err + kd * diff_err;
        output = std::fmin(std::fmax(output, -max_output), max_output);
    }
}

