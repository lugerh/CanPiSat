import subprocess
import time

EXECUTABLE_PATH = "RadioHead/examples/raspi/rf69/rf69_client"
mensaje_base = "TESTPYTHON"

contador = 0

try:
    while True:
        mensaje = f"{mensaje_base} {contador}"
        print(f"Enviando: {mensaje}")

        subprocess.run([EXECUTABLE_PATH, mensaje])

        contador += 1
        time.sleep(1)

except KeyboardInterrupt:
    print("\nPrograma interrumpido. Fin del test.")





