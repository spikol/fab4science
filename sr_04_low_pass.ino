/*
 * HC-SR04 with Low-Pass Butterworth IIR Filter Example
 * 
 * Demonstrates how a low-pass filter blocks high-frequency noise
 * while preserving slow trends in the signal.
 * 
 * Filter specs: 10 Hz sampling, 1.0 Hz cutoff, Order 4
 */

const int trigPin = 7;
const int echoPin = 8;

float duration, distance;

// Low-Pass Butterworth IIR digital filter
// Generated using filter_gen.py from FilterDemos
// Sampling rate: 10 Hz, cutoff frequency: 1.0 Hz
// Filter is order 4, implemented as second-order sections (biquads)
float lowpass(float input)
{
  float output = input;
  
  // First biquad section
  {
    static float z1, z2; // filter section state (memory)
    float x = output - (-1.04859958)*z1 - 0.29614036*z2;
    output = 0.00482434*x + 0.00964869*z1 + 0.00482434*z2;
    z2 = z1;
    z1 = x;
  }
  
  // Second biquad section
  {
    static float z1, z2; // filter section state (memory)
    float x = output - (-1.32091343)*z1 - 0.63273879*z2;
    output = 1.00000000*x + 2.00000000*z1 + 1.00000000*z2;
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
  
  // Apply low-pass filter
  float filtered_distance = lowpass(distance);
  
  // Output both raw and filtered for comparison
  Serial.print(distance);
  Serial.print(",");
  Serial.println(filtered_distance);
  
  delay(100);  // 10 Hz sampling rate (CRITICAL: matches filter design!)
}

/*
 * WHAT IS A LOW-PASS FILTER?
 * 
 * A low-pass filter allows LOW frequencies to pass through while
 * blocking HIGH frequencies.
 * 
 * Frequency = how fast something changes:
 * - LOW frequency  = slow changes (smooth trends)
 * - HIGH frequency = fast changes (noise, jitter)
 * 
 * Example:
 * Raw signal:     ~~~∿∿∿~~~∿∿∿~~~   (wavy with noise)
 * Low-pass out:   ————————————————   (smooth trend)
 *                 ↑ Keeps slow drift
 *                         ∿∿∿ ← Blocks fast jitter
 * 
 * 
 * FREQUENCY DOMAIN THINKING:
 * 
 * Sampling rate: 10 Hz (one sample every 0.1 seconds)
 * Cutoff frequency: 1.0 Hz
 * 
 * This means:
 * PASS: Changes slower than 1 Hz (> 1 second period)
 * BLOCK: Changes faster than 1 Hz (< 1 second period)
 * 
 * If your hand moves slowly (< 1 Hz), the filter tracks it.
 * If your hand vibrates rapidly (> 1 Hz), the filter ignores it.
 * 
 * 
 * IIR vs FIR FILTERS:
 * 
 * This is an IIR (Infinite Impulse Response) filter:
 * - Uses feedback (previous outputs affect current output)
 * - Very efficient (minimal memory: just z1, z2 per section)
 * - Based on analog filter designs (Butterworth)
 * - "Infinite" because output theoretically never fully settles
 * 
 * Compare to Moving Average (which is an FIR filter):
 * - No feedback (only uses recent inputs)
 * - Needs more memory (stores all samples in window)
 * - Simpler to understand
 * 
 * 
 * BUTTERWORTH DESIGN:
 * 
 * Butterworth filters are designed for "maximally flat" response:
 * - Smooth frequency response in passband
 * - No ripples or oscillations
 * - Gentle rolloff into stopband
 * 
 * Order 4 means:
 * - 4 poles in the frequency response
 * - Steeper cutoff than order 2
 * - Implemented as 2 cascaded biquads (2nd order sections)
 * 
 * 
 * HOW THE BIQUAD WORKS:
 * 
 * Each biquad section is a "Direct Form II" implementation:
 * 
 *   input → [feedback] → [feedforward] → output
 *              ↓              ↓
 *            z1, z2        coefficients
 * 
 * The z1, z2 variables are "state" - they remember past values.
 * The static keyword means they persist between function calls.
 * 
 * The coefficients (0.00482434, -1.04859958, etc.) were calculated
 * by the filter_gen.py script using scipy.signal.butter()
 * 
 * 
 * LOW-PASS vs MOVING AVERAGE:
 * 
 * Both smooth signals, but differently:
 * 
 * Moving Average:
 * - Equal weight to all samples in window
 * - Sharp cutoff in time domain
 * - Poor frequency selectivity
 * - Easy to understand
 * 
 * Low-Pass IIR:
 * - Weighted by feedback coefficients
 * - Smooth exponential decay
 * - Excellent frequency selectivity
 * - More complex design
 * 
 * 
 * PRACTICAL APPLICATIONS:
 * 
 * Sensor smoothing (remove electrical noise)
 * Audio processing (remove high-frequency hiss)
 * Control systems (smooth control signals)
 * Communication (remove out-of-band interference)
 * Biomedical (ECG, EEG filtering)
 * 
 * 
 * DESIGN CONSIDERATIONS:
 * 
 * Cutoff frequency selection:
 * - Too high: doesn't filter enough noise
 * - Too low: loses important signal information
 * - Rule: Set to ~1/5 to 1/2 of sampling rate
 * 
 * For 10 Hz sampling, 1.0 Hz cutoff is reasonable because:
 * - Distance measurements don't change faster than 1 Hz in most cases
 * - Sensor noise is typically > 1 Hz
 * - Keeps system responsive while removing jitter
 * 
 * 
 * SERIAL PLOTTER VIEW:
 * 
 * You'll see two lines:
 * - Blue (Raw): Jumpy, noisy readings
 * - Red (Filtered): Smooth, delayed slightly
 * 
 * Notice:
 * - The filtered line is MUCH smoother
 * - There's a small delay (phase lag) in the response
 * - Rapid movements are attenuated more than slow movements
 * 
 * 
 * TRY THIS:
 * 1. Move your hand SLOWLY → filter tracks well
 * 2. Move your hand QUICKLY → filter lags behind
 * 3. Hold hand still → both lines converge (noise removed)
 * 4. Tap the sensor → raw spikes, filtered stays calm
 * 
 * This demonstrates the frequency-selective nature of the filter!
 */