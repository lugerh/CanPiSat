from picamera import PiCamera
from time import sleep

# Inicializar la c      mara
camera = PiCamera()

# Configuraci      n opcional
camera.resolution = (1024, 768)  # Establecer resoluci      n
camera.rotation = 180  # Rotar la imagen si es necesario

# Capturar la foto
def take_photo(filename="photo.jpg"):
    print("Preparando la c      mara...")
    sleep(2)  # Esperar para ajustar la exposici      n
    camera.capture(filename)
    print(f"Foto guardada como {filename}")

# Llamar a la funci      n principal
take_photo()

# Cerrar la c      mara
camera.close()
