import board
import busio
import digitalio

CS = digitalio.DigitalInOut(board.CE0)
RESET = digitalio.DigitalInOut(board.D25)

spi = busio.SPI(clock=board.SCK, MISO=board.MISO, MOSI=board.MOSI)

while not spi.try_lock():
    pass

spi.configure(baudrate=5000000, phase=0, polarity=0)

# Leer el registro 0x10 (Version Register)
result = bytearray(2)
result[0] = 0x10 & 0x7F  # bit 7 a 0 para leer
result[1] = 0x00

CS.direction = digitalio.Direction.OUTPUT
CS.value = False
spi.write_readinto(result, result)
CS.value = True

spi.unlock()

print(f"Respuesta del registro 0x10 (Version): {hex(result[1])}")

if result[1] == 0x24 or result[1] == 0x10 or result[1] == 0x13:
    print("Modulo RFM69 detectado correctamente.")
elif result[1] == 0xFF or result[1] == 0x00:
    print("Posible modulo muerto o mal conectado.")
else:
    print("Respuesta desconocida, revisar conexion modelo.")

