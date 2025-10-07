// ring_median.ino : platform-independent median filter on ring buffer
// No copyright, 2020, Garth Zeglin.  This file is explicitly placed in the public domain.

// Note: this assumes the data is held in a global ring buffer, see ring_buffer.ino.

float median_buffer[RING_LENGTH];    // working buffer of samples copied from ring_buffer

//================================================================
// Utility function for the sorting function.
int float_compare(const void *e1, const void *e2)
{
  float f1 = *((const float *) e1);
  float f2 = *((const float *) e2);  
  if (f1 < f2) return -1;
  else if (f1 == f2) return 0;
  else return 1;
}

//================================================================
// Reduce signal outliers using a median filter applied over a ring buffer.
// No additional state is required, but uses the global ring_buffer array.

float ring_median_filter(void)
{
  // copy and sort the ring buffer samples
  memcpy(median_buffer, ring_buffer, sizeof(median_buffer));
  qsort(median_buffer, RING_LENGTH, sizeof(float), float_compare);

  // return the median element
  return median_buffer[RING_LENGTH/2];
}
//================================================================
