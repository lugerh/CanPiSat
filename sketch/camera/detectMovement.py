from picamera import PiCamera
import time
import numpy as np
import cv2

def detect_motion(duration=10):
    # Iniciar la c      mara Raspberry Pi
    camera = PiCamera()
    camera.resolution = (640, 480)
    camera.framerate = 24
    time.sleep(2)  # Dar tiempo para que la c      mara se estabilice

    # Capturar im      genes desde PiCamera
    prev_frame = np.empty((480, 640, 3), dtype=np.uint8)  # Imagen vac      a para la comparaci      n

    for _ in range(duration * 24):  # 24 FPS durante 10 segundos
        # Capturar una imagen
        camera.capture(prev_frame, 'bgr')

        # Convertir a escala de grises
        gray_frame = cv2.cvtColor(prev_frame, cv2.COLOR_BGR2GRAY)

        # Mostrar el frame (opcional)
        cv2.imshow("Gray Frame", gray_frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    camera.close()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    detect_motion(duration=10)
