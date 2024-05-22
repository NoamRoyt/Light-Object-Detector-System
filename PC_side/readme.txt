objects_detector(): Detects objects within a specified masking distance and displays their angles and distances.

telemeter(): Measures and displays the distance at a specified angle using a telemeter.

lights_detector(): Detects and displays distances of light sources at different angles.

Object_and_light_scan(): Performs a combined scan for objects and light sources, displaying their angles and distances based on specified masking distances.

LDR_meas(): Measures distances using an LDR (Light Dependent Resistor) sensor at different intervals and sends a signal for measurement.

LDR_Calib_arr_calc(): Reads and collects calibration measurements from a sensor and returns an interpolated array of values.
interpolate(array, distance=5): Interpolates values in the input array to increase the data density based on the specified distance.

find_nearest(meas): Finds the nearest value to a given measurement within a predefined LDR_array.

init_uart(): Initializes a UART connection on 'COM5' with specified communication settings and resets input and output buffers.

send_char(char): Sends a single ASCII character over the UART connection after encoding it as bytes and introduces a small delay.

send_data(data): Sends a single byte of data over the UART connection after converting it to bytes using little-endian encoding and introduces a small delay.

send_angle(angle): Sends a 2-byte representation of the given angle to a device through a serial connection 's' and pauses execution for 0.05 seconds.

receive_data(): Continuously reads bytes from a serial connection 's' until it encounters a newline character, then converts the received bytes into an integer and returns it.

receive_dist(): Reads a single byte from a serial connection 's', converts it to an integer using little-endian byte order, and returns the resulting integer representing distance.

main(): prints the main menu, and by the user choice decide which function to use.
