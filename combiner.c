#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[])
{
int delay_micros;
int readed_value[3];
int writed_value[3];
char read_from_color_sense[3];
char write_from_led_blinker;
char color_read[10];                             //буфер для чтения из color_sense
char color_write[3];                                     //буфер для чтения из led_blinker

delay_micros = argc * 1000000;

while (1) {                                     
	int fd = open(argv[1], O_RDONLY);
	printf("%d", fd);
	read(fd, color_read, 10);
	close(fd);
	printf("from color_sense: %s\n", color_read);
	fflush(stdin); 
	fflush(stdout);
	int fdd = open(argv[2], O_RDONLY);
	read(fdd, color_write, 3);
	close(fdd);
	printf("from led_blinker: %s\n", color_write);
	fflush(stdin); 
	fflush(stdout);                                  //?
	int k = 0;
	for (int i = 0; i < 3; i ++ ){
		write_from_led_blinker = color_write[i];
		read_from_color_sense[i] = color_read[i+k];
		read_from_color_sense[i+1] = color_read[i+k+1];
		read_from_color_sense[i+2] = color_read[i+k+2];
	/*	if(i == 0){
			k = 3;
		}
		if(i == 1){
			k = 4;
		}*/
		readed_value[i] = atoi(read_from_color_sense);
		writed_value[i] = atoi(write_from_led_blinker);
	}
	if((writed_value[0] == 1)  && (readed_value[0] > readed_value[1]) && (readed_value[0] > readed_value[2])){
		printf("\nred");
	}  
	if((writed_value[1] == 1)  && (readed_value[1] > readed_value[0]) && (readed_value[1] > readed_value[0])){
		printf("\ngreen");
	}  
	if((writed_value[2] == 1)  && (readed_value[2] > readed_value[1]) && (readed_value[2] > readed_value[0])){
		printf("\nblue");
	}else{
		printf("\ncolor");	
	}
	usleep(delay_micros);
	}
return 0;
}
