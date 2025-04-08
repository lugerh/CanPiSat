import subprocess

def scan_i2c_bus(bus=1):
    # Comando para detectar dispositivos en el bus I2C
    command = ['i2cdetect', '-y', str(bus)]
    try:
        output = subprocess.check_output(command).decode('utf-8')
        print("Dispositivos encontrados en el bus I2C {}:".format(bus))
        print(output)
    except subprocess.CalledProcessError as e:
        print("Error al escanear el bus I2C:", e)

if __name__ == "__main__":
    # Puedes cambiar el número de bus dependiendo de tu configuración
    scan_i2c_bus(1)
