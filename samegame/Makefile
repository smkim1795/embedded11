
CC = arm-linux-gnueabi-gcc
CFLAGS = -lpthread

main: main.o button.o gyro.o libBitmap.o colorled.o fnd.o
        $(CC) main.o button.o gyro.o libBitmap.o colorled.o fnd.o -o main $(CFLAGS)

main.o: main.c button.h gyro.h libBitmap.h colorled.h fnd.h
        $(CC) -c main.c -o main.o

button.o: button.c button.h
        $(CC) -c button.c -o button.o

gyro.o: gyro.c gyro.h
        $(CC) -c gyro.c -o gyro.o

libBitmap.o: libBitmap.c libBitmap.h bitmapFileHeader.h
        $(CC) -c libBitmap.c -o libBitmap.o

colorled.o: colorled.c colorled.h
        $(CC) -c colorled.c -o colorled.o

fnd.o: fnd.c fnd.h
        $(CC) -c fnd.c -o fnd.o

clean:
        rm -f *.o main
:wq!: