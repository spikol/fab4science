/*
 * HC-SR04 with 3-Sample Median Filter Example
 * 
 * Demonstrates how a median filter removes outliers and spikes
 * from noisy sensor data
 */

const int trigPin = 7;
const int echoPin = 8;

float duration, distance;

// Median filter state - stores the last 2 readings
float sample_a = 0.0;  // oldest sample
float sample_b = 0.0;  // middle sample
// sample_c will be the newest (current) sample

// Function: Find the median (middle value) of three numbers
float median_of_three(float a, float b, float c) {
  // There are 6 possible orderings: ABC, ACB, BAC, BCA, CAB, CBA
  // We return the middle value in each case
  
  if (a < b) {
    if (b < c)      return b; // ABC order: a < b < c
    else if (a < c) return c; // ACB order: a < c < b
    else            return a; // CAB order: c < a < b
  } else {
    if (a < c)      return a; // BAC order: b < a < c
    else if (b < c) return c; // BCA order: b < c < a
    else            return b; // CBA order: c < b < a
  }
}

// Function: Apply 3-sample median filter
float median_filter(float new_sample) {
  // Find median of the last 3 samples
  float result = median_of_three(sample_a, sample_b, new_sample);
  
  // Shift samples: oldest is discarded, middle becomes oldest, new becomes middle
  sample_a = sample_b;
  sample_b = new_sample;
  
  return result;
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  
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
  
  // Apply median filter
  float filtered_distance = median_filter(distance);
  
  // Output both raw and filtered for comparison
  // Format: Raw,Filtered (comma-separated for plotter)
  Serial.print(distance);
  Serial.print(",");
  Serial.println(filtered_distance);
  
  delay(100);  // 10 Hz sampling rate
}

/*
 * HOW IT WORKS:
 * 
 * Example: Readings come in as [10, 100, 12, 11, 95, 13]
 *          (100 and 95 are outliers/spikes)
 * 
 * Sample 1: a=0,   b=0,   c=10  → median(0,0,10)   = 0
 * Sample 2: a=0,   b=10,  c=100 → median(0,10,100) = 10  ✓ (spike removed!)
 * Sample 3: a=10,  b=100, c=12  → median(10,100,12)= 12  ✓ (spike removed!)
 * Sample 4: a=100, b=12,  c=11  → median(100,12,11)= 12
 * Sample 5: a=12,  b=11,  c=95  → median(12,11,95) = 12  ✓ (spike removed!)
 * Sample 6: a=11,  b=95,  c=13  → median(11,95,13) = 13  ✓ (spike removed!)
 * 
 * The median filter removes single-sample spikes while preserving
 * the true signal. It's especially good for:
 * - Removing occasional bad readings
 * - Handling intermittent interference
 * - Non-linear filtering (doesn't blur sharp edges like averaging)
 */