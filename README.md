# Signal Processing for HC-SR04 Ultrasonic Sensor

A comprehensive collection of Arduino examples demonstrating various signal processing and filtering techniques for the HC-SR04 ultrasonic distance sensor.

## 📚 Overview

This repository contains educational materials for learning signal processing concepts through hands-on Arduino programming. Each example demonstrates a different filtering technique to handle real-world sensor noise and extract meaningful information from raw data.

## 🎯 Learning Objectives

By working through these examples, you will learn:
- How different filter types affect sensor data
- When to use each filtering technique
- How to implement digital filters on microcontrollers
- Practical signal processing for embedded systems
- Trade-offs between smoothness, responsiveness, and computational cost

## 🛠️ Hardware Requirements

- Arduino board (Uno, Nano, Mega, etc.)
- HC-SR04 Ultrasonic Distance Sensor
- Breadboard and jumper wires
- (Optional) LED for hysteresis examples

### Wiring Diagram

```
HC-SR04 → Arduino
VCC     → 5V
GND     → GND
TRIG    → Pin 7
ECHO    → Pin 8
```

For LED examples:
```
LED (+) → Pin 2 → [220Ω Resistor] → GND
```

## 📁 Repository Contents

### Arduino Examples (`sr_04_*.ino`)

| File | Filter Type | Purpose |
|------|-------------|---------|
| `sr_04_simple_plot.ino` | None (baseline) | Raw sensor readings |
| `sr_04_low_pass.ino` | Low-Pass IIR | Smooth noisy data, remove high-frequency jitter |
| `sr_04_high_pass.ino` | High-Pass IIR | Detect motion/changes, remove DC offset |
| `sr_04_band_pass.ino` | Band-Pass IIR | Extract periodic signals (0.5-1.5 Hz) |
| `sr_04_median.ino` | 3-Sample Median | Remove outliers and spikes |
| `sr_04_moving_average.ino` | Moving Average | Simple smoothing by averaging |
| `sr_04_Hysteresis.ino` | Hysteresis | Stable threshold detection |
| `sr_04_Hysteresis_LED.ino` | Hysteresis + LED | Visual proximity indicator |
| `sr_04_basic_threshold.ino` | None (anti-pattern) | Demonstrates the problem filters solve |

### Complete Demonstration

- **`FilterDemos.ino`** - Comprehensive example applying multiple filters simultaneously with advanced features like trajectory fitting and statistical analysis

### Documentation

- **`signal_processing_sr04.md`** - Complete slide deck presentation covering all concepts (Marp format)

### Filter Response Plots

Visual representations of how each filter responds to different frequencies:
- `bandpass.png` - Band-pass filter frequency response
- `lowpass.png` - Low-pass filter frequency response  
- `highpass.png` - High-pass filter frequency response
- `bandstop.png` - Band-stop filter frequency response

## 🚀 Getting Started

### Quick Start

1. **Clone this repository**
   ```bash
   git clone https://github.com/yourusername/signal-processing-sr04.git
   ```

2. **Open Arduino IDE**
   - File → Open → Select any `sr_04_*.ino` example

3. **Upload to Arduino**
   - Connect your Arduino via USB
   - Select correct board and port
   - Click Upload

4. **View Results**
   - Tools → Serial Plotter (or Serial Monitor)
   - Watch the real-time filter output!

### Recommended Learning Path

1. **Start with `sr_04_simple_plot.ino`**
   - Understand raw sensor behavior
   - Observe noise and variability

2. **Try `sr_04_basic_threshold.ino`**
   - See the problem: LED flickering!
   - Understand why filtering is needed

3. **Explore individual filters**
   - Low-pass: Smoothing
   - Median: Spike removal
   - Moving average: Simple noise reduction
   - Hysteresis: Stable switching

4. **Advanced examples**
   - High-pass: Motion detection
   - Band-pass: Periodic signal extraction
   - `FilterDemos.ino`: Complete system

## 📊 Understanding the Output

### Serial Plotter Format

All examples output data in Serial Plotter compatible format:
```
Value1,Value2,Value3
```

This creates multiple traces for easy comparison between raw and filtered signals.

### Typical Plots

- **Raw signal**: Noisy, fluctuating line
- **Filtered signal**: Smooth, stable line
- **Reference lines**: Thresholds or zero crossings

## 🎓 Filter Comparison Guide

### Which Filter Should I Use?

| Goal | Recommended Filter | Example |
|------|-------------------|---------|
| Smooth noisy readings | Low-pass or Moving Average | `sr_04_low_pass.ino` |
| Detect motion/changes | High-pass | `sr_04_high_pass.ino` |
| Remove occasional spikes | Median | `sr_04_median.ino` |
| Extract rhythmic motion | Band-pass | `sr_04_band_pass.ino` |
| Stable on/off control | Hysteresis | `sr_04_Hysteresis_LED.ino` |

### Trade-offs

| Filter | Smoothness | Responsiveness | Complexity | Memory |
|--------|-----------|----------------|------------|---------|
| Moving Average | ⭐⭐⭐ | ⭐⭐ | ⭐ | ⭐⭐ |
| Median | ⭐⭐ | ⭐⭐⭐ | ⭐⭐ | ⭐ |
| Low-pass IIR | ⭐⭐⭐⭐ | ⭐⭐ | ⭐⭐⭐ | ⭐ |
| High-pass IIR | N/A | ⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐ |
| Hysteresis | ⭐⭐⭐⭐ | ⭐ | ⭐ | ⭐ |

## 🔬 Experiments to Try

### 1. Comparing Filter Behavior
- Run different examples with the same hand motion
- Compare response time and smoothness
- Which filter best suits your application?

### 2. Parameter Tuning
- Modify `WINDOW_SIZE` in moving average
- Adjust thresholds in hysteresis example
- Change cutoff frequencies (requires filter_gen.py)

### 3. Real-World Applications
- **Parking sensor**: Use hysteresis for stable distance warning
- **Motion detector**: Use high-pass to trigger on movement
- **Gesture recognition**: Use band-pass to detect waving
- **Presence detection**: Use low-pass for stable occupancy sensing

### 4. Noise Analysis
- Compare raw vs filtered signal variance
- Count false triggers with and without filtering
- Measure response delay for different filters

## 📖 Key Concepts

### Frequency Domain Thinking

- **Low frequency** = Slow changes (trends, actual distance)
- **High frequency** = Fast changes (noise, vibration)

### Sampling Rate

All examples use **10 Hz** sampling (one reading every 100ms). This is important because:
- Filter designs are matched to this rate
- Nyquist frequency is 5 Hz (can detect up to this frequency)
- Changing the delay affects filter performance

### IIR vs FIR Filters

- **IIR** (Infinite Impulse Response): Low-pass, high-pass, band-pass examples
  - Uses feedback (previous outputs)
  - Very efficient (minimal memory)
  - Based on analog filter designs
  
- **FIR** (Finite Impulse Response): Moving average example
  - No feedback (only past inputs)
  - More memory required
  - Simpler to understand

## 🐛 Troubleshooting

### No Serial Output
- Check baud rate: Should be 115200
- Verify USB connection
- Check board and port selection

### Erratic Readings
- Ensure sensor is powered properly (5V)
- Check wiring connections
- Avoid reflective or angled surfaces
- Keep sensor steady during testing

### Filter Not Working
- Verify sampling rate matches design (100ms delay)
- Check that static variables aren't being reset
- Ensure proper function call placement in loop()

## 📚 Further Reading

### Digital Signal Processing
- [SciPy Signal Processing Tutorial](https://docs.scipy.org/doc/scipy/reference/tutorial/signal.html)
- [Butterworth Filter (Wikipedia)](https://en.wikipedia.org/wiki/Butterworth_filter)
- [Digital Biquad Filters](https://en.wikipedia.org/wiki/Digital_biquad_filter)

### Arduino Resources
- [HC-SR04 Datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf)
- [Arduino Serial Plotter](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-serial-plotter)

## 🤝 Contributing

This is an educational resource. If you:
- Find bugs or issues
- Have suggestions for improvements
- Want to add new examples
- Have questions about the code

Please open an issue or submit a pull request!

## 📝 License

This code is placed in the public domain and is free to use for educational purposes.

## 🙏 Acknowledgments

**See this course from CMU at:** https://courses.ideate.cmu.edu/16-223/f2025/text/code/FilterDemos.html#filterdemos-dev-tools
Filter generation based on SciPy signal processing library and Butterworth filter design principles.

---

**Happy filtering! 🎛️**

*Remember: The best filter is the one that solves your specific problem. Experiment and learn!*
