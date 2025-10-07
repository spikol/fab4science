// Band-Stop Butterworth IIR digital filter, generated using filter_gen.py.
// Sampling rate: 10 Hz, frequency: [0.5, 1.5] Hz.
// Filter is order 4, implemented as second-order sections (biquads).
// Reference: https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.butter.html
float bandstop(float input)
{
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - -1.10547167*z1 - 0.46872661*z2;
    output = 0.43284664*x + -0.73640270*z1 + 0.43284664*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.48782202*z1 - 0.63179763*z2;
    output = 1.00000000*x + -1.70130162*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.04431445*z1 - 0.72062964*z2;
    output = 1.00000000*x + -1.70130162*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.78062325*z1 - 0.87803603*z2;
    output = 1.00000000*x + -1.70130162*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  return output;
}
