import os
from picamera import PiCamera
from time import sleep

def record_video(filename, duration=5):
    camera = PiCamera()
    camera.resolution = (1280, 720)
    camera.framerate = 30
    camera.shutter_speed = 10000  # Ajustar a la frecuencia de la luz
    camera.exposure_mode = "fixedfps"  # Evitar ajustes autom      ticos
    camera.awb_mode = "fluorescent"  # Mejorar color bajo luz fluorescente

    temp_filename = "temp_video.h264"
    print("Iniciando grabaci      n...")
    camera.start_recording(temp_filename)
    sleep(duration)
    camera.stop_recording()
    camera.close()

    print("Conversion a MP4...")
    os.system(f"ffmpeg -i {temp_filename} -movflags +faststart -c:v copy {filename}.mp4")
    os.remove(temp_filename)  # Borra el archivo temporal
    print(f"Video guardado como {filename}.mp4")

# Ejemplo de uso
if __name__ == "__main__":
    import sys
    if len(sys.argv) > 1:
        record_video(sys.argv[1])
    else:
        print("Uso: python script.py nombre_del_archivo")
