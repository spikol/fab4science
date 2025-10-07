/*
 * HC-SR04 with Band-Pass Butterworth IIR Filter Example
 * 
 * Demonstrates how a band-pass filter isolates a specific frequency range
 * while blocking both low-frequency drift and high-frequency noise.
 * 
 * Filter specs: 10 Hz sampling, 0.5-1.5 Hz passband, Order 4
 * Center frequency: 1.0 Hz, Bandwidth: 1.0 Hz
 */

const int trigPin = 7;
const int echoPin = 8;

float duration, distance;

// Band-Pass Butterworth IIR digital filter
// Generated using filter_gen.py from FilterDemos
// Sampling rate: 10 Hz, frequency: [0.5, 1.5] Hz
// Filter is order 4, implemented as second-order sections (biquads)
float bandpass(float input)
{
  float output = input;
  
  // First biquad section
  {
    static float z1, z2; // filter section state
    float x = output - (-1.10547167)*z1 - 0.46872661*z2;
    output = 0.00482434*x + 0.00964869*z1 + 0.00482434*z2;
    z2 = z1;
    z1 = x;
  }
  
  // Second biquad section
  {
    static float z1, z2; // filter section state
    float x = output - (-1.48782202)*z1 - 0.63179763*z2;
    output = 1.00000000*x + 2.00000000*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  
  // Third biquad section
  {
    static float z1, z2; // filter section state
    float x = output - (-1.04431445)*z1 - 0.72062964*z2;
    output = 1.00000000*x + (-2.00000000)*z1 + 1.00000000*z2;
    z2 = z1;
    z1 = x;
  }
  
  // Fourth biquad section
  {
    static float z1, z2; // filter section state
    float x = output - (-1.78062325)*z1 - 0.87803603*z2;
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
  Serial.println("Raw,BandPass,Zero");
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
  
  // Apply band-pass filter
  float filtered_distance = bandpass(distance);
  
  // Output: Raw, Filtered, and Zero reference line
  Serial.print(distance);
  Serial.print(",");
  Serial.print(filtered_distance);
  Serial.print(",");
  Serial.println(0.0);  // Zero reference line
  
  delay(100);  // 10 Hz sampling rate (CRITICAL: matches filter design!)
}

/*
 * WHAT IS A BAND-PASS FILTER?
 * 
 * A band-pass filter allows ONLY frequencies within a specific "band" to pass:
 * - BLOCKS: Low frequencies (DC, slow drift)
 * -  PASSES: Middle frequencies (the "band of interest")
 * -  BLOCKS: High frequencies (noise, rapid changes)
 * 
 * Think of it as a "window" in the frequency spectrum:
 * 
 *   Frequency:  0 Hz -------- 1 Hz -------- 5 Hz
 *   Response:   BLOCK -------- PASS -------- BLOCK
 *                            ↑      ↑
 *                          0.5 Hz  1.5 Hz
 * 
 * 
 * OUR FILTER SPECIFICATIONS:
 * 
 * Passband: 0.5 Hz to 1.5 Hz
 * Center frequency: 1.0 Hz
 * Bandwidth: 1.0 Hz
 * Sampling rate: 10 Hz
 * 
 * This means:
 * BLOCK: < 0.5 Hz (slower than 2 seconds per cycle)
 * PASS: 0.5-1.5 Hz (oscillations of 0.67-2 seconds per cycle)
 * BLOCK: > 1.5 Hz (faster than 0.67 seconds per cycle)
 * 
 * 
 * BAND-PASS = HIGH-PASS + LOW-PASS:
 * 
 * Conceptually, a band-pass is like cascading two filters:
 * 
 * Input → [High-pass at 0.5 Hz] → [Low-pass at 1.5 Hz] → Output
 *         ↑ Removes DC/drift       ↑ Removes high noise
 * 
 * The result: Only the "band" between 0.5-1.5 Hz survives!
 * 
 * 
 * FREQUENCY RESPONSE VISUALIZATION:
 * 
 * Imagine the filter's "gain" (how much signal passes through):
 * 
 * 0.0 Hz: |                              | Gain = 0% (blocked)
 * 0.3 Hz: |▁▁                            | Gain = 10%
 * 0.5 Hz: |▁▁▁▁▁▃▃                       | Gain = 50% (band edge)
 * 1.0 Hz: |▁▁▁▁▁▃▃▃▃▃▅▅▅▅▅█████         | Gain = 100% (center!)
 * 1.5 Hz: |▁▁▁▁▁▃▃▃▃▃▅▅▅▅▅█████▅▅▅▅▅▃   | Gain = 50% (band edge)
 * 2.0 Hz: |▁▁▁▁▁▃▃▃▃▃▅▅▅▅▅█████▅▅▅▅▅▃▃▁ | Gain = 10%
 * 5.0 Hz: |▁▁▁▁▁▃▃▃▃▃▅▅▅▅▅█████▅▅▅▅▅▃▃▁▁| Gain = 0% (blocked)
 * 
 * 
 * WHY FOUR BIQUAD SECTIONS?
 * 
 * This is an Order 4 filter = 4 cascaded 2nd-order sections (biquads)
 * 
 * More sections = Steeper rolloff = Better frequency selectivity
 * 
 * Order 2: Gentle slopes, frequencies near edges leak through
 * Order 4: Sharp cutoffs, clean separation
 * Order 8: Very sharp, but more computation
 * 
 * Our Order 4 gives excellent selectivity without being too complex.
 * 
 * 
 * WHAT GETS THROUGH?
 * 
 * Example scenarios:
 * 
 * Scenario 1: Constant distance (0 Hz)
 * Input:  [30, 30, 30, 30, 30, 30]
 * Output: [30, 15,  7,  3,  1,  0]  ← Decays to zero (DC blocked!)
 * 
 * Scenario 2: Slow drift (0.1 Hz - too slow)
 * Input:  Gradually increases from 30 to 35 cm over 10 seconds
 * Output: Small values near zero (drift blocked!)
 * 
 * Scenario 3: Rhythmic motion at 1.0 Hz (PERFECT!)
 * Input:  30, 35, 40, 35, 30, 25, 20, 25, 30, 35...
 *         ↑ One complete cycle per second
 * Output: Clean oscillating signal (PASSED through!)
 * 
 * Scenario 4: Rapid jitter at 3 Hz (too fast)
 * Input:  Rapid small oscillations
 * Output: Heavily attenuated (high-freq blocked!)
 * 
 * 
 * PRACTICAL EXAMPLE - BREATHING DETECTION:
 * 
 * Imagine the sensor pointed at someone's chest:
 * 
 * Raw signal contains:
 * - DC offset (average distance to chest): 50 cm
 * - Breathing motion (0.3 Hz = 18 breaths/min): ±1 cm
 * - Heartbeat (1.2 Hz = 72 bpm): ±0.2 cm
 * - Random noise (5+ Hz): ±0.5 cm
 * 
 * Low-pass output: 50 cm (just the average, smooth)
 * High-pass output: All motion (breathing + heart + noise)
 * Band-pass output: Mostly heartbeat! (0.5-1.5 Hz band)
 * 
 * The band-pass isolates the heartbeat frequency range!
 * 
 * 
 * REAL-WORLD APPLICATIONS:
 * 
 * Heart rate monitoring (0.5-3 Hz for 30-180 bpm)
 * Respiration detection (0.1-0.5 Hz for breathing)
 *  Vibration analysis (isolate specific machine frequencies)
 *  Seismic sensing (earthquakes in specific frequency bands)
 *  Sonar/radar processing (target detection at known frequencies)
 * Music/audio (isolate specific instruments or vocals)
 * Communication (extract signal from specific frequency channel)
 * 
 * 
 * EXAMPLE - PERIODIC MOTION DETECTION:
 * 
 * You want to detect if someone is waving their hand rhythmically:
 * 
 * No motion: Output ≈ 0 (DC blocked)
 * Random motion: Output is small (not periodic)
 * Waving at 1 Hz: Output is LARGE (in passband!)
 * Rapid tapping: Output ≈ 0 (too high frequency)
 * 
 * The filter acts as a "rhythmic motion detector"!
 * 
 * 
 * BAND-PASS vs OTHER FILTERS:
 * 
 * | Filter Type | Passes | Blocks | Use Case |
 * |-------------|--------|--------|----------|
 * | Low-pass    | 0 → 1 Hz | > 1 Hz | Smoothing |
 * | High-pass   | > 1 Hz | 0 → 1 Hz | Change detection |
 * | Band-pass   | 0.5 → 1.5 Hz | <0.5, >1.5 Hz | Periodic signal extraction |
 * | Band-stop   | <0.5, >1.5 Hz | 0.5 → 1.5 Hz | Remove interference |
 * 
 * 
 * OUTPUT BEHAVIOR:
 * 
 * Like high-pass, band-pass output oscillates around ZERO:
 * - Not absolute distance (DC removed)
 * - Shows periodic component only
 * - Positive and negative values (oscillation)
 * - Magnitude indicates strength of periodic motion
 * 
 * 
 * FILTER INITIALIZATION:
 * 
 * At startup, the filter has no history (z1, z2 = 0).
 * It takes several samples to "charge up":
 * 
 * Samples 1-5: Transient behavior (settling)
 * Samples 6+: Steady-state behavior
 * 
 * This is normal for IIR filters!
 * 
 * 
 * SERIAL PLOTTER VIEW:
 * 
 * You'll see three lines:
 * - Blue (Raw): Original distance
 * - Red (Band-pass): Oscillating around zero
 * - Green (Zero): Reference line
 * 
 * The band-pass line will:
 * - Stay near zero for constant distance
 * - Stay near zero for very slow changes
 * - Stay near zero for very fast jitter
 * - OSCILLATE for periodic motion at ~1 Hz!
 * 
 * 
 * EXPERIMENT IDEAS:
 * 
 * 1. HOLD STILL
 *    → Band-pass output decays to zero
 * 
 * 2. MOVE HAND SLOWLY (0.2 Hz - every 5 seconds)
 *    → Small output (too slow, blocked)
 * 
 * 3. WAVE HAND RHYTHMICALLY (1 Hz - once per second)
 *    → LARGE oscillations (in passband!)
 * 
 * 4. TAP RAPIDLY (3+ Hz)
 *    → Small output (too fast, blocked)
 * 
 * 5. MOVE RANDOMLY
 *    → Chaotic small output (not periodic)
 * 
 * Try to match the ~1 Hz frequency - that's the "sweet spot"!
 * 
 * 
 * DESIGN CONSIDERATIONS:
 * 
 * Bandwidth selection:
 * - Narrow band (0.8-1.2 Hz): Very selective, rejects more noise
 * - Wide band (0.5-1.5 Hz): Catches more signal, less selective
 * 
 * Our 1.0 Hz bandwidth is a good compromise.
 * 
 * Center frequency:
 * - Set to expected signal frequency (e.g., 1 Hz for waving)
 * - Must be < Nyquist frequency (sampling_rate / 2 = 5 Hz)
 * 
 * 
 * MUSICAL ANALOGY:
 * 
 * Imagine an orchestra:
 * - Low-pass: Hear only the bass section
 * - High-pass: Hear only the piccolos
 * - Band-pass: Hear only the violins (mid-range)
 * - Band-stop: Hear everything EXCEPT the violins
 * 
 * Band-pass lets you "tune in" to specific instruments!
 * 
 * 
 * SIGNAL ISOLATION:
 * 
 * If you know your signal of interest is at a specific frequency,
 * band-pass filtering is THE tool for extracting it from noise.
 * 
 * Example: Detecting a 1 Hz vibration on a motor
 * - All other vibrations: filtered out
 * - The 1 Hz component: isolated and measurable
 * - Perfect for machinery diagnostics!
 */