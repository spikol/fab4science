// moving_average.ino : windowed moving average filter with integer data (platform-independent)
// No copyright, 2020, Garth Zeglin.  This file is explicitly placed in the public domain.

// Smooth a signal by averaging over multiple samples.  The recent time history
// (the 'moving window') is kept in an array along with a running total.  The
// state memory includes one long, an integer, and an array of integers the
// length of the window.

int moving_average(int input)
{
  // The window size determines how many samples are held in memory and averaged together.
  const int WINDOW_SIZE = 5;
  
  static int ring[WINDOW_SIZE];   // ring buffer for recent time history
  static int oldest = 0;          // index of oldest sample
  static long total = 0;          // sum of all values in the buffer

  // subtract the oldest sample from the running total before overwriting
  total = total - ring[oldest];
  
  // save the new sample by overwriting the oldest sample
  ring[oldest] = input;

  // advance to the next position, wrapping around as needed
  oldest = oldest + 1;
  if (oldest >= WINDOW_SIZE) oldest = 0;

  // add the new input value to the running total
  total = total + input;

  // calculate the average; this is integer arithmetic so fractional parts will be truncated
  return total / WINDOW_SIZE;
}
