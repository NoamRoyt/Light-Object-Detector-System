import serial as ser
import time

# --------------------------- mission\State Funcs ----------------------
def objects_detector():
    array = []
    masking_distance = int(input("Enter Masking Distance:"))
    send_char('1')

    for i in range(63):
        distance = (2 * receive_dist())
        if distance < int(masking_distance):
            array.append(distance)
        else:
            array.append(0)

    for i in range(3, 64):
        if array[i] > 0:
            print("Angle: ", 3*(i-3), " | Distance: ", array[i], "\n")
        else:
            print("Angle: ", 3*(i-3), " | Distance: Masked", "\n")


def telemeter():
    send_char('2')
    angle = int(input("Insert Angle:"))
    send_angle(angle)
    time.sleep(0.05)
    distance = int(receive_dist())
    print("Angle: ", angle, " | Distance: ", distance, "\n")


def lights_detector():
    array = []
    send_char('3')

    for i in range(63):
        trash = receive_dist()
        print(trash)
        meas = receive_dist()
        distance = find_nearest(meas)

        if distance > 0:
            array.append(distance)
        else:
            array.append(0)

    for i in range(3, 64):
        if array[i] > 0:
            print("Angle: ", 3*(i-3), " | Distance: ", array[i], "\n")
        else:
            print("Angle: ", 3*(i-3), " | Distance: Masked", "\n")

def Object_and_light_scan():
    rows = 63
    light_array = [[i * 3, 0] for i in range(rows)]
    object_array = [[i * 3, 0] for i in range(rows)]
    masking_distance = int(input("Enter Objects Masking Distance:"))

    send_char('5')

    for i in range(123):
        if i < 64:
            object_distance = (2 * receive_dist())
            if object_distance < int(masking_distance):
                object_array[i][1] = object_distance
        else:
            meas = receive_dist()
            LDR_distance = find_nearest(meas)
            if LDR_distance > 0:
                light_array[i - 64][1] = LDR_distance

    for i in range(0, 60):
        if light_array[i][1] > 0:
            print("Angle: ", light_array[i][0], " | LDR Distance: ", light_array[i][1])
        else:
            print("Angle: ", light_array[i][0], " | LDR Distance: Masked")

        if object_array[i + 3][1] > 0:
            print(" | Distance: ", object_array[i + 3][1], "\n")

        else:
            print(" | Distance: Masked\n")

# --------------------------- LDR funcs -----------------------------


def LDR_meas():
    count = 0
    while True:
        if count < 10:
            cm = 5 * count
            print("Press Enter To Measure at ", cm, " cm")
            input()
            send_char('4')
            count += 1

        elif count == 10:
            break


def LDR_Calib_arr_calc():
    meas_array = []

    for i in range(10):
        temp = s.read(1)
        meas_array.append(int.from_bytes(temp, "big"))
    print(meas_array)
    interpolated_arr = interpolate(meas_array, 5)
    return interpolated_arr



def interpolate(array, distance=5):
    result = [array[0]]

    for i in range(len(array) - 1):
        start_val = array[i]
        end_val = array[i + 1]

        # Calculate the difference between start and end values
        diff = end_val - start_val

        # Calculate the step size for interpolation
        step = diff / (distance)

        # Perform linear interpolation and append the values to the result array
        for j in range(1, distance + 1):
            interpolated_val = int(start_val + step * j)
            result.append(interpolated_val)

        if i < len(array) - 2:
            result.append(end_val)  # Add the next measurement to the result array

    result.append(array[-1])
    print(len(result))
    print(result)
    return result


def find_nearest(meas):
    closest = None
    min_distance = float('inf')  # Initialize with positive infinity

    for i in range(len(LDR_array)-5):
        distance = abs(meas - LDR_array[i])
        if distance < min_distance:
            min_distance = distance
            closest = LDR_array[i]
    return closest


# --------------------- UART useful funcs -------------------------------
def init_uart():
    global s
    s = ser.Serial('COM5', baudrate=9600, bytesize=ser.EIGHTBITS,
                   parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE, timeout=None, write_timeout=1)
    s.reset_input_buffer()
    s.reset_output_buffer()


def send_char(char):
    global s
    s.write(bytes(char, 'ascii'))
    time.sleep(0.05)


def send_data(data):
    global s
    byte = data.to_bytes(1, 'little')
    s.write(byte)
    time.sleep(0.05)


def send_angle(angle):
    global s
    bytes_angle = angle.to_bytes(2, 'little')
    s.write(bytes_angle)
    time.sleep(0.05)


def receive_data():
    data = b''
    temp = b''
    while True:
        temp = s.read(1)
        if temp != b'\n':
            data += temp
        else:
            break

    return int.from_bytes(data, "little")


def receive_dist():
    temp = b''

    temp = s.read(1)

    int_dist = int.from_bytes(temp, "little")

    return int_dist


if __name__ == '__main__':
    global s

    menu = """-------------------Menu---------------------
1. Object Detector Scan
2. Telemeter
3. Light Sources Detector Scan
4. LDR Calibration
5. Light Sources and Objects Detector Scan
6. Show menu
"""

    print(menu)
    init_uart()
    ldr_detect_en = 0
    while True:
        menuNum = int(input("Enter menu number:"))
        if menuNum == 1:
            objects_detector()
        elif menuNum == 2:
            telemeter()
        elif menuNum == 3:
            if ldr_detect_en == 1:
                lights_detector()
            else:
                print("You must hit LDR Calibration Before light detector", "\n")
        elif menuNum == 4:
            LDR_meas()
            LDR_array = LDR_Calib_arr_calc()
            ldr_detect_en = 1
        elif menuNum == 5:
            Object_and_light_scan()
        elif menuNum == 6:
            print(menu)
