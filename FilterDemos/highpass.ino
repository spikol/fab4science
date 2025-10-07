// High-Pass Butterworth IIR digital filter, generated using filter_gen.py.
// Sampling rate: 10 Hz, frequency: 1.0 Hz.
// Filter is order 4, implemented as second-order sections (biquads).
// Reference: https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.butter.html
float highpass(float input)
{
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - -1.04859958*z1 - 0.29614036*z2;
    output = 0.43284664*x + -0.86569329*z1 + 0.43284664*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.32091343*z1 - 0.63273879*z2;
    output = 1.00000000*x + -2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  return output;
}
