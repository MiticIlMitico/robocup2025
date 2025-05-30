import sensor, image, time
from machine import UART
from machine import LED

# UART3 (TX=P4, RX=P5 di solito)
uart = UART(1, 9600)

# Inizializza sensore
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)  # 160x120 -> più veloce
sensor.set_auto_gain(False)         # disattiva auto regolazioni lente
sensor.set_auto_whitebal(False)
sensor.set_vflip(True)
sensor.set_hmirror(True)
sensor.skip_frames(time=2000)

clock = time.clock()

# Parametri di calibrazione lente (approssimati per lente standard OpenMV M7/MV4)
f_x = (2.8 / 3.984) * 160  # per QQVGA
f_y = (2.8 / 2.952) * 120
c_x = 160 // 2
c_y = 120 // 2

def calc_steering_and_speed(cx = None, z_mm = None):
    try:
        global c_x
        center_pos = cx if cx else c_x
        raw_turn_rate = round(center_pos - c_x)
        steering = max(-100, min(100, raw_turn_rate * 100 / 70))
        steering = int(steering)
        speed = 100
        speed = max(0, min(100, speed)) if abs(z_mm) > 3 else 0
        return steering, speed
    except:
        return 0, 0

led = LED("LED_BLUE")

while True:
    led.on()
    clock.tick()
    img = sensor.snapshot()
    tags = img.find_apriltags(fx=f_x, fy=f_y, cx=c_x, cy=c_y)

    cx = tags[0].cx if tags else None
    z_mm = tags[0].z_translation if tags else None
    try:
        steering, speed = calc_steering_and_speed(cx, z_mm) if cx else (0, 0)
        print(steering, int(speed))
        uart.write(f"{steering} {int(speed)}")
    except:
        pass