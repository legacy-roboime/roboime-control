#ifndef __ROBOIME_PID_CONTROLLER_H
#define __ROBOIME_PID_CONTROLLER_H

namespace roboime
{
    class pid_controller
    {
        private:
            float kp;           // proportional gain
            float ki;           // integral gain
            float kd;           // derivative gain
            // state variables
            float err;          // error (input-feedback)
            float prev_err;     // previous error
            float int_err;      // integral of error
            float diff_err;     // derivative of error
            // limits
            float max_integ;    // integral wind-up limit
            float max_output;   // motor output clamp (limit)
            // input and output
            bool radians;

        public:
            float output;       // motor output
            float input;        // user input
            float feedback;     // motor feedback

            pid_controller(float kp, float ki, float kd, float max_output, float max_integ, bool radians=false);
            void step(void);
    };
}
#endif
