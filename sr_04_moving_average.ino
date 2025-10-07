/*
 * HC-SR04 with Moving Average Filter Example
 * 
 * Demonstrates how averaging multiple samples smooths noisy data
 * Trade-off: Smoother output but slower response to changes
 */

const int trigPin = 7;
const int echoPin = 8;

float duration, distance;

// Moving average parameters
const int WINDOW_SIZE = 5;  // Number of samples to average
float readings[WINDOW_SIZE]; // Circular buffer to store recent samples
int index = 0;               // Current position in the buffer
float total = 0.0;           // Running sum of all values in buffer
bool buffer_filled = false;  // Track if we've filled the buffer once

// Function: Apply moving average filter
float moving_average(float new_sample) {
  // Subtract the oldest value from the running total
  total = total - readings[index];
  
  // Store the new sample at the current position
  readings[index] = new_sample;
  
  // Add the new sample to the running total
  total = total + new_sample;
  
  // Move to the next position (circular buffer)
  index = index + 1;
  if (index >= WINDOW_SIZE) {
    index = 0;
    buffer_filled = true;
  }
  
  // Calculate and return the average
  if (buffer_filled) {
    return total / WINDOW_SIZE;
  } else {
    // During startup, only average the samples we have so far
    return total / (index + 1);
  }
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  
  // Initialize the readings array to zero
  for (int i = 0; i < WINDOW_SIZE; i++) {
    readings[i] = 0.0;
  }
  
  // Print header for Serial Plotter
  Serial.println("Raw,Filtered");
}

void loop() {
  // Trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo duration
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2;
  
  // Apply moving average filter
  float filtered_distance = moving_average(distance);
  
  // Output both raw and filtered for comparison
  Serial.print(distance);
  Serial.print(",");
  Serial.println(filtered_distance);
  
  delay(100);  // 10 Hz sampling rate
}

/*
 * HOW MOVING AVERAGE WORKS:
 * 
 * Window size = 5 samples
 * 
 * Sample readings: 10, 12, 11, 50, 13, 12, 11
 *                              ↑ spike
 * 
 * Step by step (showing the 5-sample window):
 * 
 * Sample 1: [10, 0,  0,  0,  0 ] → avg = 10/1  = 10.0
 * Sample 2: [10, 12, 0,  0,  0 ] → avg = 22/2  = 11.0
 * Sample 3: [10, 12, 11, 0,  0 ] → avg = 33/3  = 11.0
 * Sample 4: [10, 12, 11, 50, 0 ] → avg = 83/4  = 20.8
 * Sample 5: [10, 12, 11, 50, 13] → avg = 96/5  = 19.2  ← Buffer full
 * Sample 6: [12, 12, 11, 50, 13] → avg = 98/5  = 19.6  ← Oldest (10) removed
 * Sample 7: [12, 11, 11, 50, 13] → avg = 97/5  = 19.4
 * Sample 8: [12, 11, 50, 50, 13] → avg = 136/5 = 27.2
 * Sample 9: [12, 11, 50, 13, 13] → avg = 99/5  = 19.8  ← Spike removed
 * Sample 10:[12, 11, 50, 13, 12] → avg = 98/5  = 19.6
 * 
 * Notice: The spike (50) affects multiple outputs as it slides through
 * the window. This is the "lag" or "delay" effect.
 * 
 * COMPARISON WITH MEDIAN FILTER:
 * 
 * Moving Average (5 samples):
 * - Smooths all noise and variations
 * - Spikes affect multiple outputs (blur effect)
 * - Good for: Gradual, continuous changes
 * - Linear filter
 * 
 * Median Filter (3 samples):
 * - Removes single-sample spikes instantly
 * - Preserves sharp edges and steps
 * - Good for: Removing outliers
 * - Non-linear filter
 * 
 * TRADE-OFFS:
 * 
 * Larger window (e.g., 10 samples):
 * Smoother output
 * Better noise reduction
 * Slower response to real changes
 * More memory usage
 * 
 * Smaller window (e.g., 3 samples):
 * Faster response
 * Less memory
 * Noisier output
 * Less smoothing
 * 
 * REAL-WORLD USES:
 * - Stock price charts (moving average trends)
 * - Sensor smoothing (temperature, light levels)
 * - Audio processing (low-pass filtering)
 * - Data visualization (smoothing noisy plots)
 * 
 * SERIAL PLOTTER TIP:
 * Try changing WINDOW_SIZE to 3, 5, 10, or 20 and see how it affects
 * the trade-off between smoothness and response time!
 */