all: clean
         gcc led_blinker_Rabtsevich.c -o led_blinker_Rabtsevich
         gcc color_sense_Rabtsevich.c -o color_sense_Rabtsevich
clean:
        rm -f  *.o
