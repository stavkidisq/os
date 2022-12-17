#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

char color[1];
char color_read[256];

int fd_led_blinker;
int fd_color_sense;


void* timeinfo()
{
    while(1)
    {
        time_t rawtime;
        struct tm* timeinfo;

        time(&rawtime);
        timeinfo = localtime(&rawtime);
        printf("local time is: %s", asctime(timeinfo));
    }
}

void* receive_led_blinker()
{
    fd_led_blinker = open(argv[2], O_RDONLY);

    while(1)
    {
        read(fd_led_blinker, color, 2);
        printf("from led_blinker: %s", color);
        fflush(stdout);
    }
}

void* receive_color_sense()
{
    fd_color_sense = open(argv[1], O_RDONLY);

    while(1)
    {
        read(fd_color_sense, color_read, 256);
        printf("from color_sense: %s", color_read);
        fflush(stdout);
    }
}

void* compare_colors()
{
    int color_int[3];

    while(1)
    {
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
}

int main(int argc, char* argv[])
{

    printf("Main() was started!");

    pthread_t receive_led_blinker_thread;
    pthread_t receive_color_sense_thread;
    pthread_t timeinfo_thread;
    pthread_t compare_colors_thread;

    void* thread_data = NULL;

    if (pthread_create(&receive_led_blinker_thread, NULL, receive_led_blinker, thread_data) != 0) {
		fprintf(stderr, "error: pthread_create was failed\n");
		exit(-1);
	}

    if (pthread_create(&receive_color_sense_thread, NULL, receive_color_sense, thread_data) != 0) {
		fprintf(stderr, "error: pthread_create was failed\n");
		exit(-1);
	}

    if (pthread_create(&timeinfo_thread, NULL, timeinfo, thread_data) != 0) {
		fprintf(stderr, "error: pthread_create was failed\n");
		exit(-1);
	}

    if (pthread_create(&compare_colors_thread, NULL, compare_colors, v) != 0) {
		fprintf(stderr, "error: pthread_create was failed\n");
		exit(-1);
	}

    return 0;
}
