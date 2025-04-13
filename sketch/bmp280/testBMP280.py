import asyncio
import board
import busio
import adafruit_bmp280

async def leer_altura_bmp280():
    # Inicializar I2C y sensor
    i2c = busio.I2C(board.SCL, board.SDA)
    bmp280 = adafruit_bmp280.Adafruit_BMP280_I2C(i2c, address=0x76)

    bmp280.sea_level_pressure = 1013.25  # hPa (puedes ajustar seg      n el d      a)

    while True:
        temperatura = bmp280.temperature
        presion = bmp280.pressure
        altitud = bmp280.altitude
        print(f'Temperatura: {temperatura:.2f}       C | Presi      n: {presion:.2f} hPa | Altitud: {altitud:.2f} m')

        # Espera no bloqueante 1 segundo
        await asyncio.sleep(1)

# Lanza la tarea asincrona
async def main():
    await leer_altura_bmp280()

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("Programa finalizado.")
