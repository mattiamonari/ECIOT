import serial

serial_speed = 115200
serial_port = '/dev/cu.usbmodem14301'
ser = serial.Serial(serial_port, serial_speed, timeout = 1)


while True:
     data = ser.readline()
     if data:
          print(data)