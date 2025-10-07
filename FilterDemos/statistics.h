// statistics.ino : compute some basic central measures in an accumulator
// No copyright, 2009-2020, Garth Zeglin.  This file is explicitly placed in the public domain.
#include <math.h>
#include <float.h>

#ifndef MAXFLOAT
#define MAXFLOAT FLT_MAX
#endif

class CentralMeasures {

public:
  long samples;        // running sum of value^0, i.e., the number of samples
  float total;         // running sum of value^1, i.e., the accumulated total
  float squared;       // running sum of value^2, i.e., the accumulated sum of squares
  float min;           // smallest input seen
  float max;           // largest input seen
  float last;          // most recent input

  // computed statistics
  float average;       // mean value
  float variance;      // square of the standard deviation

  // Constructor to initialize an instance of the class.
  CentralMeasures(void) {
    samples = 0;
    total = squared = 0.0;
    min = MAXFLOAT;
    max = -MAXFLOAT;
    last = 0.0;
    average = variance = 0.0;
  }

  // add a new sample to the accumulators; does not update the computed statistics
  void add(float value) {
    total += value;
    squared += value*value;
    if (value < min) min = value;
    if (value > max) max = value;
    samples += 1;
    last = value;
  }

  void compute_stats(void) {
    if ( samples > 0 ) {
      average = total / samples;
      if (samples > 1) {
	// The "standard deviation of the sample", which is only correct
	// if the population is normally distributed and a large sample
	// is available, otherwise tends to be too low:
	// sigma = sqrtf( samples * squared - total*total ) / ((float)samples);
      
	// Instead compute the "sample standard deviation", an unbiased
	// estimator for the variance.  The standard deviation is the
	// square root of the variance.
	variance = ( samples * squared - total*total) / ((float) samples * (float)(samples - 1)) ;
      }
    }
  }
};

