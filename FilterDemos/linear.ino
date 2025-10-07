// linear.ino : platform-independent linear transforms
// No copyright, 2020, Garth Zeglin.  This file is explicitly placed in the public domain.

//================================================================
// Floating-point version of map().  The standard Arduino map() function only
// operates using integers; this extends the idea to floating point.  The
// Arduino function can be found in the WMath.cpp file within the Arduino IDE
// distribution.  Note that constrain() is defined as a preprocessor macro and
// so doesn't have data type limitations.

float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  float divisor = in_max - in_min;
  if (divisor == 0.0) {
    return out_min;
  } else {
    return (x - in_min) * (out_max - out_min) / divisor + out_min;
  }
}

//================================================================

