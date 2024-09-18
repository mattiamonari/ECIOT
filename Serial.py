import serial

string=""
ser = serial.Serial('/dev/cu.usbmodem14301', 115200) #Tried with and without the last 3 parameters, and also at 1Mbps, same happens.
ser.flushInput()
ser.flushOutput()

while True:
    if (ser.inWaiting() > 0):
        # Read data out of the buffer until a carraige return / new line is found
        serialString = ser.readline()
        # Print the contents of the serial data
        print("hello")
        print(serialString.decode('Ascii'))