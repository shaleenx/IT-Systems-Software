#include<stdio.h>
#include<string.h>

int main(){

	int i;
	char str[100];
	printf("Enter a string: ");
	scanf("%s", str);
	for(i=0; i<10; ++i)
		printf("char at %d is %c\n", i, str[i]);
	printf("strlen is %d", strlen(str));
	printf("str[strlen-1] is %c\n", str[strlen(str)-1]);
	printf("str[strlen-2] is %c\n", str[strlen(str)-2]);
	return 0;
}
