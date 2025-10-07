/*
 * HC-SR04 with Hysteresis Filter Example
 * 
 * Demonstrates how hysteresis prevents rapid on/off switching
 * when a measurement hovers near a threshold.
 * 
 * USE CASE: Proximity detection with stable output
 */

const int trigPin = 7;
const int echoPin = 8;

float duration, distance;

// Hysteresis thresholds (in cm)
const float LOWER_THRESHOLD = 20.0;  // Object must be closer than 20cm to trigger
const float UPPER_THRESHOLD = 40.0;  // Object must move beyond 40cm to release

// Hysteresis state
bool is_close = false;  // Tracks if object is considered "close"

// Function: Apply hysteresis to prevent rapid switching
bool hysteresis_filter(float input) {
  // Current state determines which threshold to check
  
  if (is_close) {
    // Object is currently "close"
    // Only switch to "far" if distance exceeds UPPER threshold
    if (input > UPPER_THRESHOLD) {
      is_close = false;
    }
  } else {
    // Object is currently "far"
    // Only switch to "close" if distance drops below LOWER threshold
    if (input < LOWER_THRESHOLD) {
      is_close = true;
    }
  }
  
  return is_close;
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  
  // Print header for Serial Plotter
  Serial.println("Distance,State,Lower,Upper");
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
  
  // Apply hysteresis filter
  bool object_detected = hysteresis_filter(distance);
  
  // Convert boolean to a value for plotting (0 or 50)
  float state_value = object_detected ? 50.0 : 0.0;
  
  // Output: Distance, State (as height), and threshold lines
  Serial.print(distance);
  Serial.print(",");
  Serial.print(state_value);
  Serial.print(",");
  Serial.print(LOWER_THRESHOLD);
  Serial.print(",");
  Serial.println(UPPER_THRESHOLD);
  
  delay(100);  // 10 Hz sampling rate
}

/*
 * HOW HYSTERESIS WORKS:
 * 
 * WITHOUT Hysteresis (simple threshold at 30cm):
 * Distance: 31 → 29 → 31 → 30 → 29 → 31
 * Output:   OFF  ON   OFF  OFF  ON   OFF  ← Rapid switching!
 * 
 * WITH Hysteresis (lower=20cm, upper=40cm):
 * Distance: 50 → 30 → 25 → 15 → 20 → 30 → 50
 * State:    FAR  FAR  FAR  CLOSE CLOSE CLOSE FAR
 *              ↑                              ↑
 *              Still FAR (>20)                Crosses 40, now FAR
 * 
 * The "dead zone" between 20-40cm prevents oscillation!
 * 
 * REAL-WORLD ANALOGY:
 * Think of a thermostat:
 * - Set temp: 20°C
 * - Lower threshold: 19°C (heater turns ON)
 * - Upper threshold: 21°C (heater turns OFF)
 * 
 * This prevents the heater from rapidly cycling on/off when
 * the temperature hovers around 20°C.
 * 
 * PRACTICAL USES:
 * - Obstacle detection for robots (avoid rapid direction changes)
 * - Proximity alarms (stable triggering)
 * - Door sensors (prevent false triggers)
 * - Level detection in tanks
 * 
 * SERIAL PLOTTER VIEW:
 * - Blue line: Actual distance (wavy)
 * - Red line: State (flat at 0 or 50)
 * - Green line: Lower threshold (20cm)
 * - Yellow line: Upper threshold (40cm)
 * 
 * Notice how the state only changes when crossing the thresholds,
 * not when the distance jiggles in the middle zone!
 */