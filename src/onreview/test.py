# Import required libraries
import time
import random
from rfm69 import RFM69

# Constants for RFM69
RFM69_FREQ = 915
RFM69_ENCRYPT_KEY = "sampleEncryptKey"

# Initialize RFM69 radio
radio = RFM69(freq=RFM69_FREQ, isRFM69HW=True, intPin=22, rstPin=24)
radio.setEncryptionKey(RFM69_ENCRYPT_KEY)

# Function to simulate sensor data
def get_sensor_data():
    # Simulate acceleration and height
    acceleration = round(random.uniform(-2, 2), 2)
    height = round(random.uniform(0, 100), 2)
    return {'acceleration': acceleration, 'height': height}

# Function to handle incoming messages
def handle_message(message):
    if message.lower() == "start":
        # Respond with ACK
        radio.send(aesEnabled=True, data="ACK")
        print("Received 'start'. Sent ACK.")

# Open a file to write the sensor data
with open("sensor_data.txt", "a") as file:
    try:
        while True:
            # Check for incoming messages
            if radio.available():
                incoming_message = radio.receive(aesEnabled=True)
                handle_message(incoming_message)

            # Get sensor data
            sensor_data = get_sensor_data()

            # Format data frame
            data_frame = f"Acceleration: {sensor_data['acceleration']} m/s², Height: {sensor_data['height']} m\n"

            # Send sensor data
            radio.send(aesEnabled=True, data=data_frame)

            # Write data to a file
            file.write(data_frame)

            # Wait for one second
            time.sleep(1)

    except KeyboardInterrupt:
        print("Program terminated.")