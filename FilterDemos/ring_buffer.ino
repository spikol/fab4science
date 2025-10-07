// ring_buffer.ino : fixed-length sample history buffer useful for finite filters

const unsigned int RING_LENGTH = 10;
float ring_buffer[RING_LENGTH];       // circular buffer of samples
unsigned int ring_position = 0;       // index of the oldest sample

// Put a new value into a circular sample buffer, overwriting the oldest sample.
void ring_buffer_put(float value)
{
  if (ring_position < RING_LENGTH) ring_buffer[ring_position] = value;
  if (++ring_position >= RING_LENGTH) ring_position = 0;
}

// Calculate the first derivative as the finite difference between the newest
// and oldest values.  The delta_t parameter is the sampling interval in
// seconds.
float ring_buffer_deriv(float delta_t)
{
  float oldest = ring_buffer[ring_position];
  float newest = (ring_position < RING_LENGTH-1) ? ring_buffer[ring_position+1] : ring_buffer[0];
  return (newest - oldest) / ((RING_LENGTH-1) * delta_t);
}

