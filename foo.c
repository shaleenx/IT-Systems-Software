#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#define MSGSIZE 16


void closeotherends1(int p[4][2], int x, int y){ 
  int i, j;
  for(i = 0; i<4; ++i)
    for(j = 0; j<2; ++j)
      if( i!=x || j!=y )
	close(p[i][j]);
}

void closeotherends(int p[4][2], int x1, int y1, int x2, int y2){
	int i, j;
	for(i =0; i<4; ++i)
		for(j = 0; j<2; ++j)
			if( (i!=x1 || j!=y1) && (i!=x2 || j!=y2))
				close(p[i][j]);
}


int main(){

  char inbuf[MSGSIZE];
  int p[4][2], i, j, nbytes, mbytes;

  for(i=0; i<4; ++i)
    if(pipe(p[i])<0)   /* Creating 4 pipes, as required */
       exit(1);

  if(fork() == 0){ /* Child Process 1 */
    closeotherends1(p, 0, 0);
    while( (nbytes = read(p[0][0], inbuf, MSGSIZE)) > 0)
      printf("\nChild 1 read message: %s\n", inbuf);
    if(!nbytes)
      exit(2);
    close(p[0][0]);
  }
  else if(fork() == 0){ /* Child Process 2 */
    closeotherends(p, 1, 0, 3, 0);
    while( ((nbytes = read(p[1][0], inbuf, MSGSIZE)) > 0) || ((mbytes = read(p[3][0], inbuf, MSGSIZE)) > 0)){
      if(inbuf[strlen(inbuf)-1] == '#'){
	inbuf[strlen(inbuf)-1] = '\n';
	printf("\nChild 2 read message: %s", inbuf);
      }
      else{
	closeotherends1(p, 2, 1);
	inbuf[strlen(inbuf)-1] = '*';
	printf("Child 2 sending message %s to Child 3\n", inbuf);
	write(p[2][1], inbuf, MSGSIZE);
	close(p[2][1]);
      }
    }
    close(p[1][0]);
    close(p[3][0]);
  }
  else if(fork() == 0){ /* Child Process 3 */
    closeotherends(p, 1, 0, 2, 0);
    while( ((nbytes = read(p[1][0], inbuf, MSGSIZE)) >0) ||  ((mbytes = read(p[2][0], inbuf, MSGSIZE)) > 0)){
      if(inbuf[strlen(inbuf)-1] == '*'){
	inbuf[strlen(inbuf)-1] = '\n';
	printf("\nChild 3 read message: %s", inbuf);
      }
      else{
	closeotherends1(p, 3, 1);
	inbuf[strlen(inbuf)-1] = '#';
	printf("Child 3 sending message %s to Child 2\n", inbuf);
	write(p[3][1], inbuf, MSGSIZE);
	close(p[3][1]);
      }
    }
    close(p[1][0]);
    close(p[2][0]);
  }
  else{ /* Parent Process */
    int count = 0, cno; /* counter and child no. */
    char str[MSGSIZE];
    printf("Hello, I'm the Parent. How many messages would should I send? ");
    scanf("%d", &count);

    for(i = 0; i<count; ++i){
      printf("Message %d (<message string> <child to receive message>): ", i);
      scanf("%s %d", str, &cno);
      closeotherends(p, 0, 1, 1, 1);
      switch(cno){
      case 1:
	write(p[0][1], str, MSGSIZE);
	break;
      case 2:
	strcat(str, "#"); /* '#' will signify whether it is meant for C2 or not. # = C2. * = C3 */
	write(p[1][1], str, MSGSIZE);
	break;
     case 3:
	strcat(str, "*");
	write(p[1][1], str, MSGSIZE);
	break;
      default:
	printf("\nInvalid Child No...Terminating...\n");
	exit(1);
	break;
      }
//	wait(NULL);
    }
    for(i =0; i<4; ++i)
	for(j = 0; j<2; ++j)
		close(p[i][j]); 
  }
  return 0;
}
