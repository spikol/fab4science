/*
 * HC-SR04 Basic Threshold Detection (NO FILTERING)
 * 
 * Demonstrates THE PROBLEM that filtering solves!
 * 
 * This sketch uses a single threshold with no hysteresis or filtering.
 * When an object is near the threshold distance, the LED will flicker
 * rapidly due to sensor noise and small movements.
 * 
 * PROBLEM DEMONSTRATION:
 * - Move your hand slowly around 30cm from the sensor
 * - Watch the LED flicker/flutter rapidly
 * - This happens because raw sensor readings are noisy
 * - Small variations around the threshold cause rapid on/off switching
 * 
 * SOLUTION: Use hysteresis or filtering (see other examples)
 * 
 * Compare this to sr_04_Hysteresis_LED.ino to see the improvement!
 */

const int trigPin = 7;
const int echoPin = 8;
const int ledPin = LED_BUILTIN;  // Built-in LED (usually pin 13)

float duration, distance;

// Single threshold - this is the problem!
const float THRESHOLD = 30.0;  // LED turns ON when closer than 30cm

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  
  // Print header for Serial Plotter
  Serial.println("Distance,Threshold,LED_State");
  
  // Start with LED OFF
  digitalWrite(ledPin, LOW);
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
  
  // Simple threshold detection - NO HYSTERESIS
  // This causes rapid switching when near the threshold!
  bool in_range;
  if (distance < THRESHOLD) {
    digitalWrite(ledPin, HIGH);  // Object is close - LED ON
    in_range = true;
  } else {
    digitalWrite(ledPin, LOW);   // Object is far - LED OFF
    in_range = false;
  }
  
  // Convert LED state to plotter value
  float led_state = in_range ? 40.0 : 0.0;
  
  // Output for Serial Plotter
  Serial.print(distance);
  Serial.print(",");
  Serial.print(THRESHOLD);
  Serial.print(",");
  Serial.println(led_state);
  
  delay(100);  // 10 Hz sampling rate
}

/*
 * THE PROBLEM THIS DEMONSTRATES:
 * 
 * Scenario: Hand positioned at exactly 30cm from sensor
 * 
 * Sample readings (typical sensor noise):
 * Reading 1: 29.8 cm → LED ON  ✓
 * Reading 2: 30.2 cm → LED OFF ✗
 * Reading 3: 29.9 cm → LED ON  ✓
 * Reading 4: 30.1 cm → LED OFF ✗
 * Reading 5: 29.7 cm → LED ON  ✓
 * Reading 6: 30.3 cm → LED OFF ✗
 * 
 * Result: LED flickers rapidly! 😵
 * 
 * WHY THIS HAPPENS:
 * 
 * 1. SENSOR NOISE:
 *    - Ultrasonic sensors have ~±1cm accuracy
 *    - Electrical noise adds random variations
 *    - Temperature/humidity affect readings
 * 
 * 2. ENVIRONMENTAL FACTORS:
 *    - Hand isn't perfectly still (micro-movements)
 *    - Air currents affect ultrasonic waves
 *    - Surface angle affects reflections
 * 
 * 3. DIGITAL QUANTIZATION:
 *    - pulseIn() has finite resolution
 *    - Timing variations at microsecond level
 *    - Rounding in distance calculation
 * 
 * REAL-WORLD ANALOGY:
 * 
 * Imagine a room thermostat with a single threshold at 20°C:
 * - 19.9°C → Heater turns ON
 * - 20.1°C → Heater turns OFF
 * - 19.9°C → Heater turns ON
 * - 20.1°C → Heater turns OFF
 * 
 * The heater would cycle on/off constantly, wearing it out!
 * 
 * SERIAL PLOTTER VIEW:
 * 
 * You'll see three lines:
 * - Blue: Raw distance (noisy, fluctuating)
 * - Red: Threshold line (constant at 30cm)
 * - Green: LED state (flickering between 0 and 40)
 * 
 * Notice how the LED state line rapidly jumps up and down
 * when the distance hovers near the threshold!
 * 
 * EXPERIMENT:
 * 
 * 1. Position hand at ~30cm from sensor
 * 2. Hold as still as possible
 * 3. Watch the LED flicker rapidly
 * 4. Watch Serial Plotter - LED state jumps up/down
 * 5. Now try the hysteresis example for comparison
 * 
 * With hysteresis (sr_04_Hysteresis_LED.ino):
 * - Two thresholds: 20cm and 40cm
 * - LED stays stable in the 20-40cm "dead zone"
 * - No flickering!
 * 
 * SOLUTIONS TO THIS PROBLEM:
 * 
 * 1. HYSTERESIS (best for binary state):
 *    - Use two thresholds (low and high)
 *    - Creates a "dead zone" between them
 *    - State only changes when crossing thresholds
 *    → See: sr_04_Hysteresis_LED.ino
 * 
 * 2. MEDIAN FILTER (removes spikes):
 *    - Takes middle value of recent samples
 *    - Eliminates single outlier readings
 *    → See: sr_04_median.ino
 * 
 * 3. MOVING AVERAGE (smooths signal):
 *    - Averages multiple recent samples
 *    - Reduces random noise
 *    → See: sr_04_moving_average.ino
 * 
 * 4. LOW-PASS FILTER (frequency-based):
 *    - Blocks high-frequency noise
 *    - Preserves slow movements
 *    → See: sr_04_low_pass.ino
 * 
 * 5. DEBOUNCING (count-based):
 *    - Require N consecutive readings before changing state
 *    - Simple software solution
 * 
 * KEY LESSON:
 * 
 * Raw sensor data is rarely suitable for direct control!
 * Always apply appropriate filtering or hysteresis to:
 * - Reduce noise
 * - Prevent rapid switching
 * - Improve system stability
 * - Extend actuator lifetime (relays, motors, etc.)
 * 
 * This sketch intentionally demonstrates BAD practice
 * to highlight why filtering is essential! 
 * 
 * NEXT STEPS:
 * 
 * 1. Run this sketch and observe the flickering
 * 2. Try sr_04_Hysteresis_LED.ino - see stable LED
 * 3. Compare Serial Plotter outputs
 * 4. Experiment with different filter types
 * 
 * Remember: Good signal processing is invisible!
 * When done right, the system "just works" smoothly.
 */