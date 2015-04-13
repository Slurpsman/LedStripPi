myLEDDriver: rgb_controll.c rgb_controll.h
	gcc -Wall -o myLEDDriver rgb_controll.c -lwiringPi -std=gnu99
clean:
	rm myLEDDriver
