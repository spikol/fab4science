// smoothing.ino : platform-independent first-order smoothing filter
// No copyright, 2020, Garth Zeglin.  This file is explicitly placed in the public domain.

// Smooth an input signal using a first-order filter.  One floating point state
// value is required.  The smaller the coefficient, the smoother the output.

float smoothing(float input, float coeff=0.1)
{
  // The previous state is kept in a static variable; this means this function
  // can only be applied to a single input stream.
  static float value = 0.0;          // filtered value of the input

  float difference = input - value;  // compute the error
  value += coeff * difference;       // apply a constant coefficient to move the smoothed value toward the input

  return value;
}
