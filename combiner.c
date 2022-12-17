#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char* argv[])
{
    char color[128];
    char color_read[256];
    int fd_color_sense = open(argv[1], O_RDONLY);
    int fd_led_blinker = open(argv[2], O_RDONLY);

    while(1)
    {
        read(fd_color_sense, color_read, 256);
        printf("from color_sense: %s\n", color_read);
        fflush(stdout);

        read(fd_led_blinker, color, 256);
        printf("from led_blinker: %s\n", color);
        fflush(stdout);

        sleep(2);
    }

    close(fd_color_sense);
    close(fd_led_blinker);

    return 0;
}
