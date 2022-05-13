/* client.c - code for example client program that uses TCP */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>

int received = 0;

void catch_ctrl_C(int sig)
{
  printf("\nFound CTRL-C\n");
  received = 1;
}

int main(int argc, char* argv[])
{ 
  struct  sockaddr_in sad; /* structure to hold an IP address     */
  int     clientSocket;    /* socket descriptor                   */
  struct  hostent  *ptrh;  /* pointer to a host table entry       */
  
  char    *host;           /* pointer to host name                */
  int     port;            /* protocol port number                */
  
  char    Sentence[128];
  char    modifiedSentence[128];
  char    buff[128];
  int     n;
  
  if (argc != 3) {
    fprintf(stderr,"Usage: %s server-name port-number\n",argv[0]);
    exit(1);
  }

  /* this is the signal part */
  // use signal to catch SIGINT
  struct sigaction act;
  sigset_t blockmask, sigmask;

  printf("client is waiting for SIGINT Ctrl-C\n");
  act.sa_flags = SA_SIGINFO;
  act.sa_sigaction = catch_ctrl_C;
  sigemptyset(&act.sa_mask);
  sigaction(SIGINT, &act, NULL);

  sigemptyset(&blockmask);
  sigemptyset(&sigmask);

  sigaddset(&blockmask, SIGINT);
  sigprocmask(SIG_BLOCK, &blockmask, &sigmask);

  while(received == 0)
  {
    sigsuspend(&sigmask);
  }

  sigprocmask(SIG_UNBLOCK, &blockmask, NULL);
  
  /* Extract host-name from command-line argument */
  host = argv[1];         /* if host argument specified   */
  
  /* Extract port number  from command-line argument */
  port = atoi(argv[2]);   /* convert to binary            */
  
  /* Create a socket. */
  
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    fprintf(stderr, "socket creation failed\n");
    exit(1);
  }

  /* Connect the socket to the specified server. */

  memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure */
  sad.sin_family = AF_INET;           /* set family to Internet     */
  sad.sin_port = htons((u_short)port);
  ptrh = gethostbyname(host); /* Convert host name to equivalent IP address and copy to sad. */
  if ( ((char *)ptrh) == NULL ) {
    fprintf(stderr,"invalid host: %s\n", host);
    exit(1);
  }
  memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);
  
  if (connect(clientSocket, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
    fprintf(stderr,"connect failed\n");
    exit(1);
  }
  
  /* Read a sentence from user */
  while (1){
    printf("Enter Command (GETSALARY or STOP): "); fgets(Sentence, 20, stdin);
    
    /* Send the command to the server  */
    
    char command[10], ID[7];
    /* if the command Sentence is "GETSALARY"*/
    sscanf(Sentence, "%s %s", command, ID);
    if (strcmp(command, "GETSALARY") == 0) {
      n = write(clientSocket, Sentence, strlen(Sentence)+1);

      /* wait for the server to respond */
      n = read(clientSocket, buff, sizeof(buff));
      if (n < 0) {
        fprintf(stderr, "read failed\n");
        exit(1);
      }
      buff[n] = '\0';
      printf("%s\n", buff);
    }
    
    /* if the command Sentence is "STOP" */
    else if (strcmp(command, "STOP") == 0) {
      n = write(clientSocket, Sentence, strlen(Sentence)+1);

      /* wait for the server to respond */
      n = read(clientSocket, buff, sizeof(buff));
      if (n < 0) {
        fprintf(stderr, "read failed\n");
        exit(1);
      }
      buff[n] = '\0';
      printf("%s\n", buff);

      /* close the socket */
      close(clientSocket);
      break;
    }

    else
    {
      close(clientSocket);
      fprintf(stderr, "Invalid Command\n");
      break;
    }
  } // end of while loop

  return 0;
}