// median.ino : platform-independent three-sample median filter
// No copyright, 2020, Garth Zeglin.  This file is explicitly placed in the public domain.

// Return the median of three integers, i.e. the middle value of the three.
// There are six possible sorted orderings from which to choose: ABC, ACB, BAC,
// BCA, CAB, CBA.  Note that equality can make some of these cases equivalent.
int median_of_three(int a, int b, int c)
{
  if (a < b) {
    if (b < c)      return b; // ABC
    else if (a < c) return c; // ACB
    else            return a; // CAB
  } else {
    if (a < c)      return a; // BAC
    else if (b < c) return c; // BCA
    else            return b; // CBA
  }    
}  

//================================================================
// Reduce signal outliers using a non-linear median filter with a fixed width of
// three samples.  Two integer values of state are required.  The input signal
// is typically delayed by one sample period.

int median_3_filter(int c)
{
  // The previous state is kept in a static variable; this means this function
  // can only be applied to a single input stream.
  static int a = 0, b = 0;  // previous two inputs in sample order
  int median = median_of_three(a, b, c);
  a = b;
  b = c;
  return median;
}
//================================================================
