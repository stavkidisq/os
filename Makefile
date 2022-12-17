all: clean
         gcc led_blinker.c -o led_blinker
         gcc color_sense.c -o color_sense
clean:
        rm -f  *.o
