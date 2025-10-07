#!/usr/bin/env python3
# record_Arduino_data.py : customizable script to capture serial output of Arduino using the pySerial library

# This requires a pySerial installation.
#  Package details: https://pypi.python.org/pypi/pyserial
#  Documentation:   http://pythonhosted.org/pyserial/
#  Typical installation procedure: pip3 install pyserial

import serial

#================================================================
# The following section is run when this is loaded as a script.
if __name__ == "__main__":

    # The following line should be customized for your particular Arduino serial
    # device and baud rate.  The Arduino will reset and the program restart when
    # the USB port connects.
    try:
        port = serial.Serial("/dev/cu.usbmodem232101", 115200 )

    except serial.serialutil.SerialException as err:
        print(err)
        exit()
        
    try:
        # The following can customized to specify the output file name.
        with open("arduino.dat", "w") as output:

            count = 0

            print("Starting to record.")
            while True:
                # Read a single line of text.  The pyserial library returns a
                # bytestring, this converts it to a Python string.
                line = port.readline().decode()

                # Copy it to the output file.
                output.write(line)

                # Keep track of line count, this can be used to terminate collection.
                count += 1
                if count % 20 == 0:
                    print(f"{count} lines recorded.")

                # The following end condition can be customized.
                if count > 400:
                    print("Recording complete.")
                    break

    except KeyboardInterrupt:
        print("User interrupted collection, stopping.")
            
    port.close()
