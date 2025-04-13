from mpu6050 import mpu6050
import time

sensor = mpu6050(0x68)

while True:
    aceleracion = sensor.get_accel_data()
    giroscopio = sensor.get_gyro_data()
    print("Aceleraci      n: {}".format(aceleracion))
    print("Giroscopio: {}".format(giroscopio))
    time.sleep(1)

