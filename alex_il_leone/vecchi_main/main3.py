import time
import PicoRobotics
import pimoroni_i2c
import breakout_vl53l5cx

# -------------------------------------------------------------------------
# Funzioni compatibili con ulab.numpy
# -------------------------------------------------------------------------

def reduce(func, iterable, initializer=None):
    it = iter(iterable)
    if initializer is None:
        value = next(it)
    else:
        value = initializer
    for element in it:
        value = func(value, element)
    return value

def reshape(arr, shape):
    total_elements = reduce(lambda x, y: x * y, shape)
    if len(arr) < total_elements:
        arr += [0] * (total_elements - len(arr))
    arr = arr[:total_elements]
    new_arr = []
    for i in range(shape[0]):
        start_index = i * shape[1]
        end_index = start_index + shape[1]
        new_arr.append(arr[start_index:end_index])
    return new_arr

def flip(arr, axis):
    if axis == 0:
        return arr[::-1]
    elif axis == 1:
        return [row[::-1] for row in arr]
    else:
        raise ValueError("Invalid axis: {}".format(axis))

def clamp(val, min_val, max_val):
    return max(min_val, min(max_val, val))

# -------------------------------------------------------------------------
# Costanti di configurazione
# -------------------------------------------------------------------------

DISTANCE_THRESHOLD = 3000
REFLECTANCE_THRESHOLD = 90
DRIVING_SPEED = 0.7
TURNING_SPEED = 1.8
GOAL_DISTANCE = 100.0
SPEED_RANGE = 80.0

# -------------------------------------------------------------------------
# Inizializzazione I2C
# -------------------------------------------------------------------------

PINS_BREAKOUT_GARDEN = {"sda": 6, "scl": 7}
i2c = pimoroni_i2c.PimoroniI2C(**PINS_BREAKOUT_GARDEN, baudrate=2_000_000)

# -------------------------------------------------------------------------
# Inizializzazione motori
# -------------------------------------------------------------------------

board = PicoRobotics.KitronikPicoRobotics(i2c)
board.motorOn(1, "r", 10)
board.motorOn(2, "f", 10)
time.sleep(1)
board.motorOff(1)
board.motorOff(2)

# -------------------------------------------------------------------------
# Inizializzazione sensore VL53L5CX
# -------------------------------------------------------------------------

print("Starting up sensor...")
t_sta = time.ticks_ms()
sensor = breakout_vl53l5cx.VL53L5CX(i2c)
t_end = time.ticks_ms()
print("Done in {}ms...".format(t_end - t_sta))

sensor.set_resolution(breakout_vl53l5cx.RESOLUTION_8X8)
sensor.set_ranging_frequency_hz(15)
sensor.start_ranging()

# -------------------------------------------------------------------------
# Ciclo principale
# -------------------------------------------------------------------------

while True:
    time.sleep(1.0 / 60)

    if sensor.data_ready():
        data = sensor.get_data()

        reflectance = reshape(list(data.reflectance), (8, 8))
        distance = reshape(list(data.distance), (8, 8))

        scalar = 0
        target_distance = 0
        n_distances = 0

        for ox in range(8):
            for oy in range(8):
                d = distance[ox][oy]
                r = reflectance[ox][oy]
                if d > DISTANCE_THRESHOLD or r < REFLECTANCE_THRESHOLD:
                    reflectance[ox][oy] = 0
                else:
                    scalar += r

        for ox in range(8):
            for oy in range(8):
                d = distance[ox][oy]
                r = reflectance[ox][oy]
                if r > 0:
                    target_distance += d
                    n_distances += 1

        if n_distances > 0:
            target_distance /= n_distances
        else:
            target_distance = 0

        # Ribaltamento verticale
        reflectance = flip(reflectance, axis=0)

        x = 0
        y = 0

        if scalar > 0:
            for ox in range(8):
                for oy in range(8):
                    y += reflectance[ox][oy] * ox
                    x += reflectance[ox][oy] * oy

            x = (x / scalar) / 3.5 - 1.0
            y = (y / scalar) / 3.5 - 1.0

            # Inverti il segno dell'asse X per compensare la rotazione della camera
            x = -x

            scale = (target_distance - GOAL_DISTANCE) / SPEED_RANGE
            spd = clamp(scale, -1.0, 1.0) * DRIVING_SPEED

            left_speed = int(clamp((spd - (x * TURNING_SPEED)) * 30, 0, 100))
            right_speed = int(clamp((spd + (x * TURNING_SPEED)) * 30, 0, 100))

            print("x={:.2f}, y={:.2f}, d={:.1f}, L={}, R={}".format(x, y, target_distance, left_speed, right_speed))

            board.motorOn(1, "f", left_speed)
            board.motorOn(2, "f", right_speed)
        else:
            board.motorOff(1)
            board.motorOff(2)




            











            import PicoRobotics
import utime
import time
import pimoroni_i2c
import breakout_vl53l5cx

# -------------------------------------------------------------------------
# Funzioni compatibili con ulab.numpy
# -------------------------------------------------------------------------

def reduce(func, iterable, initializer=None):
    it = iter(iterable)
    if initializer is None:
        value = next(it)
    else:
        value = initializer
    for element in it:
        value = func(value, element)
    return value

def reshape(arr, shape):
    """
    Reshape the given array to the given shape.

    :param arr: The input array.
    :param shape: The desired shape.
    :return: The reshaped array.
    """
    total_elements = reduce(lambda x, y: x * y, shape)
    if len(arr) < total_elements:
        arr += [0] * (total_elements - len(arr))
    arr = arr[:total_elements]
    new_arr = []
    for i in range(shape[0]):
        start_index = i * shape[1]
        end_index = start_index + shape[1]
        new_arr.append(arr[start_index:end_index])
    return new_arr

def flip(arr, axis):
    """
    Flip the given array along the given axis.

    :param arr: The input array.
    :param axis: The axis along which to flip the array.
    :return: The flipped array.
    """
    if axis == 0:
        return arr[::-1]
    elif axis == 1:
        return [row[::-1] for row in arr]
    else:
        raise ValueError("Invalid axis: {}".format(axis))

def clamp(val, min_val, max_val):
    return max(min_val, min(max_val, val))

# -------------------------------------------------------------------------
# Costanti di configurazione
# -------------------------------------------------------------------------
DISTANCE_THRESHOLD = 3000
REFLECTANCE_THRESHOLD = 90
DRIVING_SPEED = 0.7
TURNING_SPEED = 1.8
GOAL_DISTANCE = 500.0
SPEED_RANGE = 80.0

# -------------------------------------------------------------------------
# Inizializzazione I2C
# -------------------------------------------------------------------------
PINS_BREAKOUT_GARDEN = {"sda": 6, "scl": 7}
i2c = pimoroni_i2c.PimoroniI2C(**PINS_BREAKOUT_GARDEN, baudrate=2_000_000)

# -------------------------------------------------------------------------
# Inizializzazione motori (test iniziale)
# -------------------------------------------------------------------------
board = PicoRobotics.KitronikPicoRobotics(i2c)
board.motorOn(1, "r", 10)
board.motorOn(2, "f", 10)
time.sleep(1)
board.motorOff(1)
board.motorOff(2)

# -------------------------------------------------------------------------
# Inizializzazione sensore VL53L5CX
# -------------------------------------------------------------------------
print("Starting up sensor...")
t_sta = time.ticks_ms()
sensor = breakout_vl53l5cx.VL53L5CX(i2c)
t_end = time.ticks_ms()
print("Done in {}ms...".format(t_end - t_sta))

sensor.set_resolution(breakout_vl53l5cx.RESOLUTION_8X8)
sensor.set_ranging_frequency_hz(15)
sensor.start_ranging()

# -------------------------------------------------------------------------
# Ciclo principale di tracking e controllo motori
# -------------------------------------------------------------------------
while True:
    
    # Imposta i limiti dei servo
    # servo spalla destra
    servo1_min = 0   # posizione più bassa per il servo 1
    servo1_max = 40  # posizione più alta per il servo 1
    
    #servo spalla sinistra
    servo2_min = 100   # posizione più bassa per il servo 2
    servo2_max = 50  # posizione più alta per il servo 2

    # Costruisci la sequenza di andata e ritorno
    degrees_list = list(range(servo1_max, servo1_min - 1, -1)) + list(range(servo1_min, servo1_max + 1))

    for degrees in degrees_list:
        board.servoWrite(1, degrees)
        
        # Mappatura inversa proporzionale per il servo 2
        proportion = (degrees - servo1_min) / (servo1_max - servo1_min)  # tra 0 e 1
        servo2_pos = int(servo2_max - proportion * (servo2_max - servo2_min))
        
        board.servoWrite(2, servo2_pos)

        utime.sleep_ms(20)
    """
    for degrees in range(50, -1, -1):  # Servo 1 va da 50 a 0 gradi (grande, medio, piccolo)
        board.servoWrite(1, degrees)  # Servo 1 scende (braccio destro)
        board.servoWrite(2, 135 - (50 - degrees))  # Servo 2 va da 135 (per degrees=0) a 85 (per degrees=50)
        utime.sleep_ms(20)
    
    for degrees in range(0, 51):  # Servo 1 va da 0 a 50 gradi (piccolo, medio, grande)
        board.servoWrite(1, degrees)  # Servo 1 sale (braccio destro)
        board.servoWrite(2, 135 - (50 - degrees))  # Servo 2 va da 135 (per degrees=0) a 85 (per degrees=50)
        utime.sleep_ms(20)
    """
    utime.sleep_ms(100)
    time.sleep(1.0 / 60)

    if sensor.data_ready():
        data = sensor.get_data()
        reflectance = reshape(list(data.reflectance), (8, 8))
        distance = reshape(list(data.distance), (8, 8))

        scalar = 0
        target_distance = 0
        n_distances = 0

        for ox in range(8):
            for oy in range(8):
                d = distance[ox][oy]
                r = reflectance[ox][oy]
                if d > DISTANCE_THRESHOLD or r < REFLECTANCE_THRESHOLD:
                    reflectance[ox][oy] = 0
                else:
                    scalar += r

        for ox in range(8):
            for oy in range(8):
                d = distance[ox][oy]
                r = reflectance[ox][oy]
                if r > 0:
                    target_distance += d
                    n_distances += 1

        if n_distances > 0:
            target_distance /= n_distances
        else:
            target_distance = 0

        reflectance = flip(reflectance, axis=0)
        x = 0
        y = 0

        if scalar > 0:
            for ox in range(8):
                for oy in range(8):
                    y += reflectance[ox][oy] * ox
                    x += reflectance[ox][oy] * oy

            x = (x / scalar) / 3.5 - 1.0
            y = (y / scalar) / 3.5 - 1.0

            scale = (target_distance - GOAL_DISTANCE) / SPEED_RANGE
            spd = clamp(scale, -1.0, 1.0) * DRIVING_SPEED

            left_speed = int(clamp((spd - (x * TURNING_SPEED)) * 30, 0, 100))
            right_speed = int(clamp((spd + (x * TURNING_SPEED)) * 30, 0, 100))

            print("x={:.2f}, y={:.2f}, d={:.1f}, L={}, R={}".format(x, y, target_distance, left_speed, right_speed))

            board.motorOn(1, "f", left_speed)
            board.motorOn(2, "f", right_speed)
        else:
            board.motorOff(1)
            board.motorOff(2)


