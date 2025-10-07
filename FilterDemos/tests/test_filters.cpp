// test_filters.cpp : offline testing program to plot the effects of several filters

#include <stdio.h>
#include <math.h>
#include <string.h> // for memcpy
#include <stdlib.h> // for qsort

// Directly include the filter source files from the Arduino sketch.
#include "../ring_buffer.ino"
#include "../ring_median.ino"
#include "../linear.ino"
#include "../median.ino"
#include "../smoothing.ino"
#include "../statistics.h"
#include "../lowpass.ino"
#include "../highpass.ino"
#include "../bandpass.ino"
#include "../bandstop.ino"
#include "../trajfit.ino"

CentralMeasures stats;

int main(int argc, char *argv[])
{
  const char *input_filename = (argc > 1) ? argv[1] : "data/raw_waving.dat";
  FILE *input = fopen(input_filename, "r");
  if (input == NULL) return 0;

  FILE *c_output = fopen("data/calibrated.dat", "w");
  if (c_output == NULL) return 0;

  FILE *m_output = fopen("data/median.dat", "w");
  if (m_output == NULL) return 0;

  FILE *nz_output = fopen("data/nozero.dat", "w");
  if (nz_output == NULL) return 0;
  
  FILE *s_output = fopen("data/smoothed.dat", "w");
  if (s_output == NULL) return 0;

  FILE *lp_output = fopen("data/lowpass.dat", "w");
  if (lp_output == NULL) return 0;

  FILE *hp_output = fopen("data/highpass.dat", "w");
  if (hp_output == NULL) return 0;

  FILE *bp_output = fopen("data/bandpass.dat", "w");
  if (bp_output == NULL) return 0;

  FILE *bs_output = fopen("data/bandstop.dat", "w");
  if (bs_output == NULL) return 0;

  FILE *tf_output = fopen("data/trajfit.dat", "w");
  if (tf_output == NULL) return 0;

  FILE *d_output = fopen("data/differenced.dat", "w");
  if (d_output == NULL) return 0;

  FILE *rm_output = fopen("data/ring-median.dat", "w");
  if (rm_output == NULL) return 0;

  int previous = 0;

  while (!feof(input)) {
    int value;
    // read the raw ping time in microseconds
    if (fscanf(input, "%d", &value) != 1) break;
    
    // apply sonar calibration to convert the raw data from microseconds to centimeters
    float cm = fmap(value, 0.0, 5900.0, 0.0, 100.0);
    fprintf(c_output, "%f\n", cm);
    
    // demonstrate the median-of-three filter, report a calibrated result
    int median = median_3_filter(value);
    float median_cm = fmap(median, 0.0, 5900.0, 0.0, 100.0);
    fprintf(m_output, "%f\n", median_cm);
    
    // the raw data includes zeros representing no-ping conditions; remove these
    // by re-using the previous non-zero sample.
    if (value == 0) value = previous;
    else previous = value;
    
    // report the calibrated zero-suppressed data
    float nozero_cm = fmap(value, 0.0, 5900.0, 0.0, 100.0);
    fprintf(nz_output, "%f\n", nozero_cm);

    // keep statistics on the zero-suppressed data
    stats.add(nozero_cm);
      
    // demonstrate the smoothing filter on the zero-suppressed data, report a calibrated result    
    float smooth_cm = smoothing(nozero_cm);
    fprintf(s_output, "%f\n", smooth_cm);

    // demonstrate the low-pass filter on the zero-suppressed data, report a calibrated result    
    float lowpass_cm = lowpass(nozero_cm);
    fprintf(lp_output, "%f\n", lowpass_cm);

    // demonstrate the high-pass filter on the zero-suppressed data, report a calibrated result    
    float highpass_cm = highpass(nozero_cm);
    fprintf(hp_output, "%f\n", highpass_cm);

    // demonstrate the band-pass filter on the zero-suppressed data, report a calibrated result    
    float bandpass_cm = bandpass(nozero_cm);
    fprintf(bp_output, "%f\n", bandpass_cm);

    // demonstrate the band-stop filter on the zero-suppressed data, report a calibrated result    
    float bandstop_cm = bandstop(nozero_cm);
    fprintf(bs_output, "%f\n", bandstop_cm);

    // demonstrate the trajectory fitting filter on the zero-suppressed data, report a calibrated result
    float traj[3];
    trajfit(nozero_cm, traj);
    fprintf(tf_output, "%f %f %f\n", traj[0], traj[1], traj[2]);
    
    // apply the range data to the ring buffer and filter
    ring_buffer_put(nozero_cm);

    // calculate the median of the ring buffer
    float ring_median = ring_median_filter();
    fprintf(rm_output, "%f\n", ring_median);
    
    // calculate the finite differencing derivative
    float velocity = ring_buffer_deriv(coeff_dt);
    fprintf(d_output, "%f\n", velocity);
  }
  fclose(input);
  fclose(c_output);
  fclose(m_output);
  fclose(nz_output);
  fclose(s_output);
  fclose(lp_output);
  fclose(hp_output);
  fclose(bp_output);
  fclose(bs_output);
  fclose(tf_output);
  fclose(d_output);
  fclose(rm_output);

  stats.compute_stats();
  printf("stats: samples: %ld min: %f max: %f average: %f variance: %f standard deviation: %f\n",
	 stats.samples, stats.min, stats.max, stats.average, stats.variance, sqrt(stats.variance));
    
}
