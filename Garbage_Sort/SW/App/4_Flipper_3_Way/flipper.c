
#include <stdint.h> // uint16_t and family
#include <stdio.h> // printf and family
#include <unistd.h> // file ops
#include <fcntl.h> // open() flags
#include <string.h> // strerror()
#include <errno.h> // errno


#define DEV_STREAM_FN "/dev/gpio_stream"

int main()
{
	int fd,r;
	fd = open(DEV_STREAM_FN, O_RDWR);
	if(fd < 0){
		fprintf(stderr, "ERROR: \"%s\" not opened!\n", DEV_STREAM_FN);
		fprintf(stderr, "fd = %d %s\n", fd, strerror(-fd));
		return 4;
	}
	//22 je ravnotezni
	uint8_t first_command[3] = {'w', 4, 1}; 
	uint8_t second_command[3] = {'w', 3, 0}; 
	uint8_t third_command[3] = {'w', 2, 1};
	uint8_t fourth_command[2] = {'r', 23};
	uint8_t fifth_command[3] = {'w', 2, 0};
	uint8_t sixth_command[2] = {'r',24};			//drugi prekidac
	uint8_t middle_switch[2] = {'r', 22};			//ravnotezni
	
	r = write(fd, first_command, sizeof(first_command));
	if(r != sizeof(first_command)){
		fprintf(stderr, "ERROR: write went wrong!\n");
		return 4;
	}
	usleep(100000);

	printf("Stigli smo do ovde : 1\n");

	r = write(fd, second_command, sizeof(second_command));
	if(r != sizeof(second_command)){
		fprintf(stderr, "ERROR: write went wrong!\n");
		return 4;
	}
	usleep(100000);

	printf("Stigli smo do ovde: 2\n");

	r = write(fd, third_command, sizeof(third_command));
	if(r != sizeof(third_command)){
		fprintf(stderr, "ERROR: write went wrong!\n");
		return 4;
	}
	usleep(100000); 


	printf("Stigli smo do ovde : 3\n");

	uint8_t rd_val=0;			// levi
	uint8_t rd_val2 = 0;		//desni
	uint8_t rd_val3 = 0;		//srednji

	while(1)
	{	
		
		r = write(fd, fourth_command, sizeof(fourth_command));
		if(r != sizeof(fourth_command)){
			fprintf(stderr, "ERROR: write went wrong!\n");
			return 4;
		}
		usleep(100); 

		r = read(fd, (char*)&rd_val, sizeof(rd_val));
		if(r != sizeof(rd_val)){
			fprintf(stderr, "ERROR: read went wrong!\n");
			return 5;
		}
		
		//drugi prekidac
		r = write(fd, sixth_command, sizeof(sixth_command));
		if(r != sizeof(sixth_command)){
			fprintf(stderr, "ERROR: write went wrong!\n");
			return 4;
		}
		usleep(100); 

		r = read(fd, (char*)&rd_val2, sizeof(rd_val2));
		if(r != sizeof(rd_val2)){
			fprintf(stderr, "ERROR: read went wrong!\n");
			return 5;
		}
		
		
		//middle
		r = write(fd, middle_switch, sizeof(middle_switch));
		if(r != sizeof(middle_switch)){
			fprintf(stderr, "ERROR: write went wrong!\n");
			return 4;
		}
		usleep(100); 

		r = read(fd, (char*)&rd_val3, sizeof(rd_val3));
		if(r != sizeof(rd_val3)){
			fprintf(stderr, "ERROR: read went wrong!\n");
			return 5;
		}
		
	//provjera
	if(rd_val || rd_val2 || rd_val3){
		write(fd, fifth_command, sizeof(fifth_command));
		if(rd_val){
			printf("Levi prekidac aktivan!\n");
			//break;
		}
		if(rd_val2){
			printf("Desni prekidac aktivan!\n");
			//break;
			}
		if(rd_val3){
			printf("Srednji prekidac aktivan!\n");
			//break;
			}
			
		printf("\nCekam 2 sekunde...\n");
		sleep(2);
		
		//ponovo pokretanje motora
		printf("\n Proslo dve sekunde motor ponovo rece.\n");
		write(fd, third_command, sizeof(third_command));
		usleep(500000);
		}
	

	

	}
	close(fd);

	return 0;
}

