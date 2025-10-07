/*
 * HC-SR04 with High-Pass Butterworth IIR Filter Example
 * 
 * Demonstrates how a high-pass filter blocks low-frequency drift
 * while preserving fast changes in the signal.
 * 
 * Filter specs: 10 Hz sampling, 1.0 Hz cutoff, Order 4
 */

const int trigPin = 7;
const int echoPin = 8;

float duration, distance;

// High-Pass Butterworth IIR digital filter
// Generated using filter_gen.py from FilterDemos
// Sampling rate: 10 Hz, cutoff frequency: 1.0 Hz
// Filter is order 4, implemented as second-order sections (biquads)
float highpass(float input)
{
  float output = input;
  
  // First biquad section
  {
    static float z1, z2; // filter section state (memory)
    float x = output - (-1.04859958)*z1 - 0.29614036*z2;
    output = 0.43284664*x + (-0.86569329)*z1 + 0.43284664*z2;
    z2 = z1;
    z1 = x;
  }
  
  // Second biquad section
  {
    static float z1, z2; // filter section state (memory)
    float x = output - (-1.32091343)*z1 - 0.63273879*z2;
    output = 1.00000000*x + (-2.00000000)*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  
  return output;
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  
  // Print header for Serial Plotter
  Serial.println("Raw,HighPass,Zero");
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
  
  // Apply high-pass filter
  float filtered_distance = highpass(distance);
  
  // Output: Raw, Filtered, and Zero reference line
  Serial.print(distance);
  Serial.print(",");
  Serial.print(filtered_distance);
  Serial.print(",");
  Serial.println(0.0);  // Zero reference line for comparison
  
  delay(100);  // 10 Hz sampling rate (CRITICAL: matches filter design!)
}

/*
 * WHAT IS A HIGH-PASS FILTER?
 * 
 * A high-pass filter is the OPPOSITE of a low-pass filter:
 * - BLOCKS: Low frequencies (slow changes, DC offset, drift)
 * - PASSES: High frequencies (fast changes, motion, edges)
 * 
 * Think of it as a "change detector":
 * - Constant value → output goes to zero
 * - Slow drift → output is small
 * - Rapid change → output is large
 * 
 * 
 * VISUAL COMPARISON:
 * 
 * Input signal:    ___/‾‾‾‾‾‾\___    (ramp up, hold, ramp down)
 * Low-pass out:    ___/‾‾‾‾‾‾\___    (smooth version of input)
 * High-pass out:   ___/⌃_____⌄___    (only shows the changes!)
 *                      ↑       ↑
 *                   Rising   Falling
 * 
 * 
 * FREQUENCY DOMAIN:
 * 
 * Sampling rate: 10 Hz
 * Cutoff frequency: 1.0 Hz
 * 
 * This means:
 * ❌ BLOCK: Changes slower than 1 Hz (> 1 second period)
 * ✅ PASS: Changes faster than 1 Hz (< 1 second period)
 * 
 * If you hold the sensor still (0 Hz), output → 0
 * If you move it slowly (0.5 Hz), output is attenuated
 * If you move it quickly (2+ Hz), output tracks the motion
 * 
 * 
 * DC BLOCKING BEHAVIOR:
 * 
 * "DC" = Direct Current = constant value (0 Hz)
 * 
 * Example:
 * Input:  [30, 30, 30, 30, 30, 30] (constant at 30 cm)
 * Output: [30, 15, 7,  3,  1,  0]  (decays to zero!)
 * 
 * The filter removes the "DC offset" or baseline value.
 * Only CHANGES from the baseline are preserved.
 * 
 * 
 * PRACTICAL EXAMPLE - MOTION DETECTION:
 * 
 * Raw sensor reading: 25 cm, 25 cm, 25 cm, 26 cm, 30 cm, 35 cm, 38 cm, 38 cm
 * 
 * Low-pass output:    25,    25,    25,    26,    28,    32,    36,    37
 *                     ↑ Tracks the absolute distance smoothly
 * 
 * High-pass output:   0,     0,     0,     +1,    +4,    +5,    +3,    0
 *                     ↑ Shows only the motion/change!
 * 
 * When object stops moving → high-pass output returns to zero
 * 
 * 
 * HIGH-PASS vs LOW-PASS:
 * 
 * They are COMPLEMENTARY filters:
 * 
 * Low-Pass (smoothing):
 * - Removes noise, keeps trends
 * - Output = baseline + slow changes
 * - Use when: You want stable readings
 * 
 * High-Pass (differentiation):
 * - Removes baseline, keeps changes
 * - Output = rate of change (similar to derivative)
 * - Use when: You want to detect motion/events
 * 
 * MATHEMATICAL RELATIONSHIP:
 * Input = Low-pass output + High-pass output (approximately)
 * 
 * 
 * WHY THE OUTPUT CAN BE NEGATIVE:
 * 
 * High-pass filter output shows DIRECTION of change:
 * - Positive output = moving away (increasing distance)
 * - Zero output = stationary
 * - Negative output = moving closer (decreasing distance)
 * 
 * This is why we plot a zero reference line!
 * 
 * 
 * COEFFICIENT DIFFERENCES:
 * 
 * Compare to low-pass coefficients:
 * 
 * Low-pass feedforward:  [0.00482, 0.00965, 0.00482]  ← Small, all positive
 * High-pass feedforward: [0.43285, -0.86569, 0.43285] ← Larger, has negative!
 * 
 * The negative coefficient creates the "differentiating" effect.
 * This is how it emphasizes changes rather than levels.
 * 
 * 
 * PRACTICAL APPLICATIONS:
 * 
 * Motion detection (only respond to movement)
 * Edge detection (find sudden transitions)
 * Remove sensor drift (eliminate slow bias changes)
 * Audio processing (remove rumble, DC offset)
 * Seismic sensing (detect vibrations, ignore gravity)
 * Accelerometer processing (remove gravity, keep motion)
 * Event detection (trigger on changes, not levels)
 * 
 * 
 * REAL-WORLD EXAMPLE - DOOR SENSOR:
 * 
 * Raw distance:     100, 100, 100, 90, 70, 50, 30, 30, 30
 *                   ↑ Door closed    ↓ Opening     ↓ Open
 * 
 * Low-pass:         100, 100, 100, 95, 80, 60, 40, 32, 30
 *                   ↑ Smooth tracking of door position
 * 
 * High-pass:        0,   0,   0,   -10, -20, -20, -10, 0, 0
 *                   ↑ Quiet          ↓ Motion!     ↓ Quiet
 * 
 * The high-pass output ONLY triggers during the motion event!
 * Perfect for "door opening" alert without caring about the door's position.
 * 
 * 
 * COMBINED WITH LOW-PASS:
 * 
 * For complete signal analysis, use BOTH:
 * 
 * Low-pass → "Where is the object?" (position)
 * High-pass → "Is the object moving?" (velocity indicator)
 * 
 * Together they give you:
 * - Static position (low-pass)
 * - Dynamic motion (high-pass)
 * - Complete situational awareness!
 * 
 * 
 * SERIAL PLOTTER VIEW:
 * 
 * You'll see three lines:
 * - Blue (Raw): Original distance readings
 * - Red (High-pass): Change/motion signal (oscillates around zero)
 * - Green (Zero): Reference line
 * 
 * Notice:
 * - When you hold still → high-pass goes to zero
 * - When you move closer → high-pass goes negative
 * - When you move away → high-pass goes positive
 * - Fast movements create large spikes
 * - Slow drifts are suppressed
 * 
 * 
 * TRY THIS EXPERIMENT:
 * 
 * 1. Hold hand at 30 cm → high-pass output settles to ~0
 * 2. Slowly move to 40 cm → small positive bump
 * 3. Quickly move to 20 cm → large negative spike
 * 4. Hold still again → returns to ~0
 * 5. Tap sensor rapidly → large oscillations
 * 6. Remove hand entirely → big change detected!
 * 
 * This demonstrates pure "change detection" behavior!
 * 
 * 
 * FILTER DESIGN INSIGHT:
 * 
 * Same cutoff as low-pass (1.0 Hz) but opposite behavior:
 * - Below 1 Hz → attenuated (slow movements ignored)
 * - Above 1 Hz → passed through (fast movements detected)
 * 
 * The complementary nature means:
 * Low-pass(signal) + High-pass(signal) ≈ original signal
 * 
 * They "split" the frequency spectrum at the cutoff point!
 */