#!/usr/bin/env python3
usage_guide = """\

This script generates C/C++ code for a trajectory estimation filter using the
SciPy signal processing library.  It uses the Savitzky-Golay technique to
generate coefficients which fit a quadratic to a time history of samples.  The
filter reports the current estimated position, velocity, and acceleration,
smoothed over the sampling window.

References:
  https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.savgol_coeffs.html
  https://en.wikipedia.org/wiki/Savitzky%E2%80%93Golay_filter
"""

import sys

# Standard Python libraries.
import sys, argparse

# Extension libraries.
import numpy as np
import scipy.signal

################################################################
def emit_filter_function(stream, name, coeffs):
    order = len(coeffs)
    width = len(coeffs[0])
    matrix = np.array2string(coeffs, max_line_width=55, precision=6, separator=", ", prefix="    ")
    matrix = matrix.replace('[', '{').replace(']', '}')

    stream.write(f"""\
void {name}(float input, float output[{order}])
{{
  const float coeff[{order}][{width}] = 
    {matrix};
  static float ring[{width}]; // buffer for recent time history
  static int oldest = 0;      // index of oldest sample

  // save the new sample by overwriting the oldest sample
  ring[oldest] = input;
  if (++oldest >= {width}) oldest = 0;

  // iterate over the coefficient rows
  int index = oldest;
  for (int i = 0; i < {order}; i++) {{
    output[i] = 0.0; // clear accumulator

    // Iterate over the samples and the coefficient rows.  The index cycles
    // around the circular buffer once per row.
    for (int j = 0; j < {width}; j++) {{
      output[i] += coeff[i][j] * ring[index];
      if (++index >= {width}) index = 0;
    }}
  }}
}}
""")

################################################################
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="""Generate C code for a trajectory estimation filter.""",
                                     formatter_class=argparse.RawDescriptionHelpFormatter,
                                     epilog=usage_guide)

    parser.add_argument('--rate',  default=10,  type=float, help = 'Sampling frequency in Hz (default 10).')
    parser.add_argument('--width', default=5,   type=int,   help = 'Window length, must be odd (default 5).')
    parser.add_argument('--name',  type=str, help='Name of C filter function.')
    parser.add_argument('--out',   type=str, help='Path of C output file for filter code.')
    args = parser.parse_args()

    # evaluate the polynomial at the end of the window to provide parameters representing the most recent sample
    position = args.width-1
    coeffs = list()
    order = 2
    
    for deriv in range(order+1):
        coeffs.append(scipy.signal.savgol_coeffs(args.width, order, deriv, pos=position, use='dot'))

    coeffs = np.stack(coeffs)

    # scale the coefficients so the output values have correct units
    dt = 1.0 / args.rate
    coeffs[1] *= args.rate
    coeffs[2] *= args.rate * args.rate

    # print out the coefficients
    # np.savetxt(sys.stdout, coeffs, fmt="%f")

    filename = 'trajfit.ino' if args.out is None else args.out
    stream = open(filename, "w")

    stream.write(f"// Trajectory estimation filter generated using trajfit_gen.py.\n")
    stream.write(f"// Based on Savitzky-Golay polynomial fitting filters.\n")
    stream.write(f"// Sampling rate: {args.rate} Hz.\n")
    stream.write("""\
// The output array will contain the trajectory parameters representing the signal
// at the current time: [position, velocity, acceleration], with units of [1, 1/sec, 1/sec/sec].
// Reference: https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.savgol_coeffs.html
""")
    funcname = 'trajfit' if args.name is None else args.name
    emit_filter_function(stream, funcname, coeffs)
    stream.close()
