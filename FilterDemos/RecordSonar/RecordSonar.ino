// RecordSonar.ino : sample a range sensor at precise intervals to collect reference data
// No copyright, 2020, Garth Zeglin.  This file is explicitly placed in the public domain.

//================================================================
// Hardware definitions. You will need to customize this for your specific hardware.
const int sonarTriggerPin = 7;    // Specify a pin for a sonar trigger output.
const int sonarEchoPin    = 8;    // Specify a pin for a sonar echo input.

//================================================================
// Set the serial port transmission rate. The baud rate is the number of bits
// per second.
const long BAUD_RATE = 115200;    

//================================================================
// This function is called once after reset to initialize the program.
void setup()
{
  // Initialize the Serial port for host communication.
  Serial.begin(BAUD_RATE);

  // Initialize the digital input/output pins.
  pinMode(sonarTriggerPin, OUTPUT);
  pinMode(sonarEchoPin, INPUT);
}

//================================================================
// This function is called repeatedly to handle all I/O and periodic processing.
// This loop should never be allowed to stall or block so that all tasks can be
// constantly serviced.
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

    // Generate a short trigger pulse.
    digitalWrite(sonarTriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(sonarTriggerPin, LOW);

    // Measure the echo pulse length.  The ~12 ms timeout is chosen for a maximum
    // range of 2 meters assuming sound travels at 340 meters/sec.  With a round
    // trip of 4 meters distance, the maximum ping time is 4/340 = 0.0118
    // seconds.  You may wish to customize this for your particular hardware.
    const unsigned long TIMEOUT = 11800;
    unsigned long ping_time = pulseIn(sonarEchoPin, HIGH, TIMEOUT);

    // The no-ping condition is returned as zero, and will appear so in the data.
    Serial.println(ping_time);
  }
}
