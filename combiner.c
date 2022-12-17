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
    char color[1];
    char color_read[256];
    int fd_color_sense = open(argv[1], O_RDONLY);
    int fd_led_blinker = open(argv[2], O_RDONLY);
    int color_int[3];

    while(1)
    {
        timeinfo();

        read(fd_color_sense, color_read, 256);
        printf("from color_sense: %s", color_read);
        fflush(stdout);

        read(fd_led_blinker, color, 2);
        printf("from led_blinker: %s", color);
        fflush(stdout);

        printf("\n");

        char* token = strtok(color_read, ",");
        sscanf(token, "%d", &color_int[0]);
        //printf("%d\n", color_int[0]);

        int count = 1;
        while(token != NULL)
        {
            token = strtok(NULL, ",");
            if(count < 3)
            {
                sscanf(token, "%d", &color_int[count]);
                //printf("%d\n", color_int[count]);
            }
            count++;
        }


        if((color[0] == 'R')  && (color_int[0] > color_int[1]) && (color_int[0] > color_int[2]))
        {
            printf("red\n");
        }
        else if((color[0] == 'G')  && (color_int[1] > color_int[0]) && (color_int[1] > color_int[2]))
        {
            printf("green\n");
        }
        else if((color[0] == 'Y')  && (color_int[2] > color_int[1]) && (color_int[2] > color_int[0]))
        {
            printf("yellow\n");
        }
        else
        {
            printf("error\n");
        }
        
        sleep(2);
    }

    close(fd_color_sense);
    close(fd_led_blinker);

    return 0;
}
