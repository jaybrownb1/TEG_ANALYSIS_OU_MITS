import serial
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Set the COM port and baud rate
COM_PORT = 'COM8'  # Use 'COM8' for Windows
BAUD_RATE = 115200

# Open the serial port
ser = serial.Serial(COM_PORT, BAUD_RATE)

# Allow time for the connection to establish
time.sleep(2)

# Initialize lists to hold temperature data
temperatures = []
x_data = []

# Initialize the plot
plt.figure(figsize=(10, 5))
plt.title('Temperature Readings from MAX31865')
plt.xlabel('Reading Number')
plt.ylabel('Temperature (°C)')
plt.grid()

# Function to update the plot
def update(frame):
    global temperatures, x_data
    
    # Read a line from the serial
    line = ser.readline().decode('utf-8').strip()
    
    if line:  # If the line is not empty
        temperature = float(line)
        temperatures.append(temperature)
        x_data.append(len(temperatures))  # Append the reading number

        # Limit the x-axis to the last 50 readings
        if len(temperatures) > 50:
            temperatures.pop(0)
            x_data.pop(0)

        plt.clf()  # Clear the current figure
        plt.plot(x_data, temperatures, marker='o')
        plt.title('Temperature Readings from MAX31865')
        plt.xlabel('Reading Number')
        plt.ylabel('Temperature (°C)')
        plt.grid()

# Use FuncAnimation to update the plot in real-time
ani = FuncAnimation(plt.gcf(), update, interval=1000)  # Update every second

try:
    plt.show()  # Show the plot
except KeyboardInterrupt:
    print("Interrupted by user.")
finally:
    ser.close()  # Close the serial port
