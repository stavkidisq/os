#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define IN 0
#define OUT 1

#define LOW 0
#define HIGH 1

//***************************//
#define LEDR 24
#define LEDY 10
#define LEDG 9
//***************************//
#define VALUE_MAX 30

void Exiting(int);

void timeinfo()
{
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("local time is: %s\n", asctime(timeinfo));
}

static int GPIOExport(int pin)
{
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		Exiting(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return (0);
}

static int GPIOUnexport(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		Exiting(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return (0);
}

static int GPIODirection(int pin, int dir)
{
	static const char s_directions_str[] = "in\0out";

#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		Exiting(-1);
	}

	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3],
			IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		Exiting(-1);
	}

	close(fd);
	return (0);
}



static int GPIOWrite(int pin, int value)
{
	static const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		Exiting(-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		Exiting(-1);
	}

	close(fd);
	return (0);
}

void Exiting(int parameter)
{
	GPIOUnexport(LEDR);
	GPIOUnexport(LEDY);
	GPIOUnexport(LEDG);
	exit(parameter);
}

void Exiting_sig()
{
	GPIOUnexport(LEDR);
	GPIOUnexport(LEDY);
	GPIOUnexport(LEDG);
	exit(0);
}

void help()
{
	printf("    Use this application for blinking\n");
	printf("    execute format: ./led_blinking [-h][-q] TIME \n");
	printf("    return: color light\n");
	printf("    TIME - pause between blinks in sec\n");
	printf("    -h - help\n");
	printf("    -q - quiet\n");
}

int GPIOReader(int current_value, int delay)
{
    FILE* fd = fopen("/sys/class/gpio/gpio22/value", "r");
    int a = 1;
    fscanf(fd, "%d", &a);
    printf("A is: %d\n", a);

    if(a == 0)
    {
        timeinfo();
        if(current_value == 0)
        {
            GPIOWrite(LEDR, 0);
            GPIOWrite(LEDY, 1);
            GPIOWrite(LEDG, 0);
            printf("[button]: Light:R\n");
            fflush(stdout);
            usleep(delay/5);

            return 0;
        }
        else if(current_value == 1)
        {
            GPIOWrite(LEDR, 0);
            GPIOWrite(LEDY, 0);
            GPIOWrite(LEDG, 1);
            printf("[button]: Light:G\n");
            fflush(stdout);
            usleep(delay/5);

            return 0;
        }
        else if(current_value == 2)
        {
            GPIOWrite(LEDR, 0);
            GPIOWrite(LEDY, 1);
            GPIOWrite(LEDG, 0);
            printf("[button]: Light:Y\n");
            fflush(stdout);
            usleep(delay/5);

            return 0;
        }
    }

    return 1;
}

int main(int argc, char *argv[])
{
	int quiet = 0;
	if (argc > 1) {
		if ((strcmp(argv[1], "-h") == 0)) {
			help();
			return 0;
		} else {
			if ((strcmp(argv[1], "-q") == 0)) {
				quiet = 1;
			}
		}
	}

	if ((quiet && argc != 3) || (!quiet && argc != 2)) {
		help();
		return 0;
	}

	if (!quiet)
		printf("\nThe led blinker application was started\n\n");
	int argument = 1;
	if (quiet)
		argument++;
	double delay = atof(argv[argument]) * 1000000;
	signal(SIGINT, Exiting_sig);
	GPIOExport(LEDR);
	GPIOExport(LEDY);
	GPIOExport(LEDG);
	GPIODirection(LEDR, OUT);
	GPIODirection(LEDY, OUT);
	GPIODirection(LEDG, OUT);

    int current_value = 0;
    sleep(0.5);
	while (1) {
        current_value = 0;
        if(GPIOReader(current_value, delay) == 1)
        {
            GPIOWrite(LEDR, 1);
		    GPIOWrite(LEDY, 0);
		    GPIOWrite(LEDG, 0);
		    printf("Light:R\n");
		    fflush(stdout);
            if(GPIOReader(current_value, delay) == 1)
            {
		        usleep(delay);
            }
        }

        current_value = 1;
        if(GPIOReader(current_value, delay) == 1)
        {
            GPIOWrite(LEDR, 0);
		    GPIOWrite(LEDY, 1);
		    GPIOWrite(LEDG, 0);
		    printf("Light:Y\n");
		    fflush(stdout);
		    if(GPIOReader(current_value, delay) == 1)
            {
		        usleep(delay);
            }
        }

        current_value = 2;
        if(GPIOReader(current_value, delay) == 1)
        {
            GPIOWrite(LEDR, 0);
		    GPIOWrite(LEDY, 0);
		    GPIOWrite(LEDG, 1);
		    printf("Light:G\n");
		    fflush(stdout);
		    if(GPIOReader(current_value, delay) == 1)
            {
		        usleep(delay);
            }
        }
	}
	return 0;
}
