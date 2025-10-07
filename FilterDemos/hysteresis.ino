// hysteresis.ino : platform-independent non-linear filters
// No copyright, 2020, Garth Zeglin.  This file is explicitly placed in the public domain.

//================================================================
// Quantize an input stream into a binary state.  Dual thresholds are needed to
// implement hysteresis: the input needs to rise above the upper threshold to
// trigger a high output, then drop below the input threshold to return to the
// low output.  One bit of state is required.

bool hysteresis(int input, int lower=300, int upper=700)
{
  // The previous state is kept in a static variable; this means this function
  // can only be applied to a single input stream.
  static bool output = false;  // previous binary output

  if (output) {
    if (input < lower) output = false;
  } else {
    if (input > upper) output = true;
  }
  return output;
}

//================================================================
// Suppress a specific value in an input stream.  One integer of state is required.
int suppress_value(int input, int value)
{
  static int previous = 0;
  if (input != value) previous = input;
  return previous;
}

//================================================================
// Debounce an integer stream by suppressing changes from the previous value
// until a specific new value has been observed a minimum number of times. Three
// integers of state are required.

int debounce(int input, int samples)
{
  static int current_value = 0;
  static int new_value = 0;
  static int count = 0;

  if (input == current_value) {
    count = 0;
  } else {
    if (count == 0) {
      new_value = input;
      count = 1;
    } else {
      if (input == new_value) {
	count += 1;
	if (count >= samples) {
	  current_value = new_value;
	  count = 0;
	}
      } else {
	new_value = input;
	count = 1;
      }
    }
  }
  return current_value;
}
//================================================================
