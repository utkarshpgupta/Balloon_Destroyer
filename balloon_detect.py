import cv2
import numpy as np
import serial
import time

# Initialize serial connection
arduino = serial.Serial('/dev/cu.SLAB_USBtoUART', 9600)
time.sleep(2)  # Wait for the connection to establish

def send_command(command):
    arduino.write(command.encode())

cap = cv2.VideoCapture(0)
object_tracking = False  # Flag to check if an object is currently being tracked

while True:
    ret, frame = cap.read()
    if not ret:
        break

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Define the range of red color in HSV
    lower_red = np.array([0, 120, 70])
    upper_red = np.array([10, 255, 255])
    mask1 = cv2.inRange(hsv, lower_red, upper_red)
    lower_red = np.array([170, 120, 70])
    upper_red = np.array([180, 255, 255])
    mask2 = cv2.inRange(hsv, lower_red, upper_red)
    mask = mask1 + mask2

    contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    if contours:
        c = max(contours, key=cv2.contourArea)
        area = cv2.contourArea(c)

        if area > 6000:
            # Start tracking if the object is large enough
            object_tracking = True

            x, y, w, h = cv2.boundingRect(c)
            cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

            cX = x + w // 2
            cY = y + h // 2
            cv2.circle(frame, (cX, cY), 5, (255, 255, 255), -1)

            height, width, _ = frame.shape
            centerX, centerY = width // 2, height // 2
            dX, dY = centerX - cX, centerY - cY

            tolerance = 100

            if abs(dX) <= tolerance and abs(dY) <= tolerance:
                send_command('S')  # Stop if the object is approximately centered
                send_command('X')
                time.sleep(1)
            else:
                if abs(dX) > tolerance:
                    send_command('L' if dX > 0 else 'R')
                if abs(dY) > tolerance:
                    send_command('U' if dY > 0 else 'D')
        else:
            # Ignore small objects and continue scanning
            if not object_tracking:
                # Scanning mode: Move left and right
                send_command('L')  # Replace with your scanning logic
    else:
        if object_tracking:
            # Reset tracking if the object is lost
            object_tracking = False
            send_command('S')
        else:
            # Scanning mode: Move left and right
            send_command('L')  # Replace with your scanning logic

    cv2.imshow('frame', frame)
    cv2.imshow('mask', mask)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
arduino.close()
