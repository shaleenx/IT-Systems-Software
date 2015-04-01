#include<stdio.h>
#include<string.h>
#include<fcntl.h>
int main(){


	char str[100];
	int fd = open("foolo.txt", O_RDONLY, 1);
	int nbytes = read(fd, str, 16);
	printf("nbytes = %d\n %s\n", nbytes, str);
	nbytes = read(fd, str, 10);
	printf("nbytes = %d\n %s\n", nbytes, str);




	return 0;
}
