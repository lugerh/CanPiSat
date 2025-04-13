import smbus
import math
import time

# Configuraci      n del MPU6050
MPU6050_ADDR = 0x68  # Direcci      n I2C del MPU6050
bus = smbus.SMBus(1)  # Utilizamos el bus I2C 1 en Raspberry Pi

# Registros del aceler      metro y giroscopio
ACCEL_XOUT_H = 0x3B
ACCEL_XOUT_L = 0x3C
ACCEL_YOUT_H = 0x3D
ACCEL_YOUT_L = 0x3E
ACCEL_ZOUT_H = 0x3F
ACCEL_ZOUT_L = 0x40

# Funci      n para leer un valor de 16 bits desde el MPU6050
def read_word(reg):
    high = bus.read_byte_data(MPU6050_ADDR, reg)
    low = bus.read_byte_data(MPU6050_ADDR, reg + 1)
    value = (high << 8) + low
    if value >= 0x8000:  # Convertir a valor negativo si es necesario
        value -= 0x10000
    return value

# Funci      n para calcular la aceleraci      n absoluta
def calculate_acceleration():
    # Leer los valores del aceler      metro en 3 ejes (en unidades de aceleraci      n)
    ax = read_word(ACCEL_XOUT_H)
    ay = read_word(ACCEL_YOUT_H)
    az = read_word(ACCEL_ZOUT_H)
    
    # Convertir de los valores brutos a m/s       (dependiendo de la escala de sensibilidad)
    # El MPU6050 tiene una escala de 2g por defecto para el aceler      metro
    scale_factor = 16384.0  # Para una escala de 2g
    ax = ax / scale_factor
    ay = ay / scale_factor
    az = az / scale_factor
    
    # Calcular la aceleraci      n absoluta
    acceleration = math.sqrt(ax**2 + ay**2 + az**2)
    
    return acceleration

# Filtro de media m      vil para suavizar los datos
def moving_average(data, window_size=5):
    return sum(data[-window_size:]) / len(data[-window_size:])

# Iniciar la lectura y procesamiento de datos
accel_data = []

try:
    while True:
        acceleration = calculate_acceleration()
        accel_data.append(acceleration)

        # Aplicar filtro de media m      vil para suavizar los datos
        smoothed_accel = moving_average(accel_data)

        # Imprimir la aceleraci      n absoluta suavizada
        print(f"Aceleraci      n absoluta suavizada: {smoothed_accel:.2f} m/s      ")

        # Esperar un poco antes de la siguiente medici      n (ajustar seg      n sea necesario)
        time.sleep(0.1)
except KeyboardInterrupt:
    print("Detenido por el usuario.")
