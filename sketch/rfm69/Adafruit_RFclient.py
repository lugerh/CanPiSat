import board
import digitalio
import adafruit_rfm69

# Configuracio de pines
CS = digitalio.DigitalInOut(board.CE0)
RESET = digitalio.DigitalInOut(board.D25)

# Crear instancia de RFM69
rfm69 = adafruit_rfm69.RFM69(board.SPI(), CS, RESET, 433.0, baudrate=1000000)

# Enviar mensaje
rfm69.send(bytes("Hola desde el satelite", "utf-8"))

# Recibir mensaje
mensaje = rfm69.receive()
if mensaje is not None:
    print("Mensaje recibido: ", mensaje.decode())

