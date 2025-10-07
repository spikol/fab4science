/*
 * HC-SR04 with Hysteresis Filter + Built-in LED Indicator Example
 * 
 * Demonstrates how hysteresis prevents rapid on/off switching
 * when a measurement hovers near a threshold.
 * 
 * Built-in LED turns ON when object is in range (close)
 * 
 * USE CASE: Proximity detection with stable output
 */

const int trigPin = 7;
const int echoPin = 8;
const int ledPin = LED_BUILTIN;  // Built-in LED (usually pin 13)

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
  pinMode(ledPin, OUTPUT);  // Configure built-in LED as output
  Serial.begin(115200);
  
  // Print header for Serial Plotter
  Serial.println("Distance,State,Lower,Upper");
  
  // Ensure built-in LED starts OFF
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
  
  // Apply hysteresis filter
  bool object_detected = hysteresis_filter(distance);
  
  // Control built-in LED based on detection state
  if (object_detected) {
    digitalWrite(ledPin, HIGH);  // Turn built-in LED ON when object is close
  } else {
    digitalWrite(ledPin, LOW);   // Turn built-in LED OFF when object is far
  }
  
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
 * LED BEHAVIOR:
 * - Built-in LED OFF when distance > 40 cm (far)
 * - Built-in LED stays OFF until distance < 20 cm
 * - Built-in LED turns ON when distance < 20 cm (close)
 * - Built-in LED stays ON until distance > 40 cm
 * - NO flickering in the 20-40 cm zone!
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
 * - Automatic lighting control
 * - Parking sensors
 * 
 * HARDWARE CONNECTIONS:
 * - HC-SR04 TRIG → Arduino Pin 7
 * - HC-SR04 ECHO → Arduino Pin 8
 * - HC-SR04 VCC  → Arduino 5V
 * - HC-SR04 GND  → Arduino GND
 * - Built-in LED (pin 13) - no additional wiring needed!
 * 
 * NOTE: The built-in LED (LED_BUILTIN) is available on most Arduino boards
 * and requires no external circuit. It's typically on pin 13.
 * 
 * SERIAL PLOTTER VIEW:
 * - Blue line: Actual distance (wavy)
 * - Red line: State (flat at 0 or 50)
 * - Green line: Lower threshold (20cm)
 * - Yellow line: Upper threshold (40cm)
 * 
 * Notice how the state only changes when crossing the thresholds,
 * not when the distance jiggles in the middle zone!
 * 
 * EXPERIMENT:
 * 1. Place hand far from sensor (>40cm) → Built-in LED OFF
 * 2. Slowly move hand closer → LED stays OFF until <20cm
 * 3. Hand reaches 15cm → Built-in LED turns ON
 * 4. Move hand to 25cm (in dead zone) → LED stays ON
 * 5. Move hand to 35cm (still in dead zone) → LED stays ON
 * 6. Move hand beyond 40cm → Built-in LED turns OFF
 * 7. Try moving hand rapidly around 30cm → LED remains stable!
 * 
 * CUSTOMIZATION:
 * - Adjust LOWER_THRESHOLD for sensitivity
 * - Adjust UPPER_THRESHOLD for release distance
 * - Larger gap = more stable, but less responsive
 * - Smaller gap = more responsive, but may flicker
 * 
 * RECOMMENDED SETTINGS:
 * - For stable detection: 20cm gap (current setting)
 * - For responsive detection: 10cm gap (e.g., 20-30cm)
 * - For very stable (parking sensor): 30cm gap (e.g., 15-45cm)
 */