// Trajectory estimation filter generated using trajfit_gen.py.
// Based on Savitzky-Golay polynomial fitting filters.
// Sampling rate: 10 Hz.
// The output array will contain the trajectory parameters representing the signal
// at the current time: [position, velocity, acceleration], with units of [1, 1/sec, 1/sec/sec].
// Reference: https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.savgol_coeffs.html
void trajfit(float input, float output[3])
{
  const float coeff[3][5] = 
    {{  0.085714,  -0.142857,  -0.085714,   0.257143,
        0.885714},
     {  3.714286,  -3.857143,  -5.714286,  -1.857143,
        7.714286},
     { 28.571429, -14.285714, -28.571429, -14.285714,
       28.571429}};
  static float ring[5]; // buffer for recent time history
  static int oldest = 0;      // index of oldest sample

  // save the new sample by overwriting the oldest sample
  ring[oldest] = input;
  if (++oldest >= 5) oldest = 0;

  // iterate over the coefficient rows
  int index = oldest;
  for (int i = 0; i < 3; i++) {
    output[i] = 0.0; // clear accumulator

    // Iterate over the samples and the coefficient rows.  The index cycles
    // around the circular buffer once per row.
    for (int j = 0; j < 5; j++) {
      output[i] += coeff[i][j] * ring[index];
      if (++index >= 5) index = 0;
    }
  }
}
