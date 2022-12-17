#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

void timeinfo()
{
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("local time is: %s", asctime(timeinfo));
}

int main(int argc, char* argv[])
{
    char color[128];
    char color_read[256];
    int fd_color_sense = open(argv[1], O_RDONLY);
    int fd_led_blinker = open(argv[2], O_RDONLY);

    while(1)
    {
        timeinfo();

        read(fd_color_sense, color_read, 256);
        printf("from color_sense: %s", color_read);
        fflush(stdout);

        read(fd_led_blinker, color, 256);
        printf("from led_blinker: %s", color);
        fflush(stdout);

        printf("\n");

        sleep(2);
    }

    close(fd_color_sense);
    close(fd_led_blinker);

    return 0;
}
