// FilterDemos.ino : Arduino program to demonstrate a variety of single-channel signal filters.
// No copyright, 2020, Garth Zeglin.  This file is explicitly placed in the public domain.

// The actual filter functions are kept in a separate .ino files which will
// automatically be compiled with this one by the Arduino IDE.  The filters are
// purely numerical and can compiled for testing on a normal desktop computer.
#include <stdlib.h>

// The baud rate is the number of bits per second transmitted over the serial port.
const long BAUD_RATE = 115200;

//================================================================
// Hardware definitions. You will need to customize this for your specific hardware.
const int sonarTriggerPin = 7;    // Specify a pin for a sonar trigger output.
const int sonarEchoPin    = 8;    // Specify a pin for a sonar echo input.

//================================================================
// Global variables.
#include "statistics.h"
CentralMeasures stats; // class defined in statistics.h


//================================================================
// Standard Arduino initialization function to configure the system.
void setup()
{
  // initialize the Serial port
  Serial.begin( BAUD_RATE );

  // Initialize the digital input/output pins.
  pinMode(sonarTriggerPin, OUTPUT);
  pinMode(sonarEchoPin, INPUT);
}

//================================================================
// Standard Arduino polling function. This function is called repeatedly to
// handle all I/O and periodic processing.  This loop should never be allowed to
// stall or block so that all tasks can be constantly serviced.

void loop()
{
  // Calculate the interval in microseconds since the last polling cycle.
  static unsigned long last_time = 0;
  unsigned long now = micros();
  unsigned long interval = now - last_time;
  last_time = now;

  // Poll the sonar at regular intervals.
  static long sonar_timer = 0;
  sonar_timer -= interval;
  if (sonar_timer < 0) {
    sonar_timer += 100000; // 10 Hz sampling rate

    // read the sonar; zeros represent a no-ping condition
    int raw_ping = ping_sonar();

    // suppress zeros in the input, just repeating the last input
    int nz_ping = suppress_value(raw_ping, 0);

    // apply a median filter to suppress individual outliers
    int median = median_3_filter(nz_ping);
    
    // convert the value from microseconds to centimeters
    float cm = fmap(median, 0.0, 5900.0, 0.0, 100.0);

    // track central measures such as average and variance
    stats.add(cm);
    stats.compute_stats();

    // apply the low-pass, high-pass, band-pass, and band-stop filters
    float lowpass_cm = lowpass(cm);
    float highpass_cm = highpass(cm);
    float bandpass_cm = bandpass(cm);
    float bandstop_cm = bandstop(cm);
    
    // apply the range data to the ring buffer and filter
    ring_buffer_put(cm);

    // calculate the finite differencing derivative
    float velocity = ring_buffer_deriv(0.1);

    // calculate the median filter over the ring buffer
    float ring_median = ring_median_filter();
    
    // fit a trajectory curve to recent sample history
    float traj[3];
    trajfit(cm, traj);

    // emit some data to plot
    Serial.print(raw_ping); Serial.print(" ");         // ping time in microseconds
    Serial.print(median); Serial.print(" ");           // median-filtered ping time
    Serial.print(cm); Serial.print("  ");              // centimeter-scaled of median-filtered
    Serial.print(velocity); Serial.print("  ");        // velocity using finite differencing
    Serial.print(ring_median); Serial.print(" ");         // median-filtered distance using ring buffer
    Serial.print(traj[0]); Serial.print(" ");             // quadratic position
    Serial.print(traj[1]); Serial.print(" ");             // quadratic velocity
    Serial.print(stats.average); Serial.print("  ");   // position average over all data
    Serial.println();
  }
}

//================================================================
// Run a measurement cycle on the sonar range sensor. Returns the round-trip
// time in microseconds.

int ping_sonar(void)
{
  // Generate a short trigger pulse.
  digitalWrite(sonarTriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonarTriggerPin, LOW);

  // Measure the echo pulse length.  The ~6 ms timeout is chosen for a maximum
  // range of 100 cm assuming sound travels at 340 meters/sec.  With a round
  // trip of 2 meters distance, the maximum ping time is 2/340 = 0.0059
  // seconds.  You may wish to customize this for your particular hardware.
  const unsigned long TIMEOUT = 5900;
  unsigned long ping_time = pulseIn(sonarEchoPin, HIGH, TIMEOUT);
	
  return ping_time;
}
//================================================================
