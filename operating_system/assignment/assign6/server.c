/* server.c - code for example server program that uses TCP */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

typedef struct employee_info
{
  char ID[7];   char name[10];    double salary; 
} employee_info;

// Create a static array of struct employee_info with 10 employee
static employee_info DB[10] = { 
  {"aaa111","name1", 100000.0},
  {"bbb222","name2", 200000.0},
  {"ccc333","name3", 300000.0},
  {"ddd444","name4", 400000.0},
  {"eee555","name5", 500000.0},
  {"fff666","name6", 600000.0},
  {"ggg777","name7", 700000.0},
  {"hhh888","name8", 800000.0},
  {"iii999","name9", 900000.0},
  {"jjj000","name10", 1000000.0} };

main(argc, argv)
      int     argc;
      char    *argv[];
{
  struct  sockaddr_in sad; /* structure to hold server's address  */
  int     port;            /* protocol port number                */
  
  struct  sockaddr_in cad; /* structure to hold client's address  */
  int     alen;            /* length of address                   */
  
  int     welcomeSocket, connectionSocket; /* socket descriptors  */
  
  char    clientSentence[128]; 
  char    capitalizedSentence[128]; 
  char    buff[128];
  int     i, n;
  
  /* Check command-line argument for protocol port and extract   */
  /* port number if one is specified. Otherwise, give error      */
  if (argc > 1) {                /* if argument specified        */
    port = atoi(argv[1]);        /* convert argument to binary   */
  } else { 
    fprintf(stderr,"Usage: %s port-number\n",argv[0]);
    exit(1);
  }
  
  /* Create a socket */

  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0); /* CREATE SOCKET */
  if (welcomeSocket < 0) {
    fprintf(stderr, "socket creation failed\n");
    exit(1);
  }
  
  /* Bind a local address to the socket */
  
  memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure   */
  sad.sin_family = AF_INET;           /* set family to Internet     */
  sad.sin_addr.s_addr = INADDR_ANY;   /* set the local IP address   */
  sad.sin_port = htons((u_short)port);/* set the port number        */ 
  
  if (bind(welcomeSocket, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
    fprintf(stderr,"bind failed\n");
    exit(1);
  }
  
  /* Specify the size of request queue */
  
  if (listen(welcomeSocket, 10) < 0) {
    fprintf(stderr,"listen failed\n");
    exit(1);
  }
  
  /* Main server loop - accept and handle requests */
  
  while (1) {
    alen = sizeof(cad);
    if ( (connectionSocket=accept(welcomeSocket, (struct sockaddr *)&cad, &alen)) < 0) {
      fprintf(stderr, "accept failed\n");
      exit(1);
    }

    int pid = fork();
    if (pid == 0)
    {
      while (1)
      {
        /* Get the sentence from the client */
        clientSentence[0]='\0';
        n=read(connectionSocket,buff,sizeof(buff));
        while(n > 0){
          strncat(clientSentence,buff,n);
          if (buff[n-1]=='\0') break;
          n=read(connectionSocket,buff,sizeof(buff));
        }
        
        /* search in the DB */
        /* If the server gets "GETSALARY ID" or "STOP"*/
          char command[10], ID[7];
          sscanf(clientSentence,"%s %s",command,ID);
          if (strcmp(command,"GETSALARY")==0)
          {
            printf("Server searching %s %s\n",command,ID);
            /* Find the employee with the specified ID */
            for (i=0; i<10; i++) {
              if (strcmp(DB[i].ID,ID)==0) {
                /* Send the salary to the client */
                sprintf(buff,"Founded %s with salary %.2f",DB[i].ID,DB[i].salary);
                write(connectionSocket,buff,strlen(buff));
                printf("Founded and send back to the client\n");
                break;
              }
            }
            /* if the employee is not found */
            if (i==10) {
              printf("Server not found\n");
              sprintf(buff,"Not found %s in database.",ID);
              write(connectionSocket,buff,strlen(buff));
            }
          }
          else if (strcmp(command,"STOP")==0)
          {
            /* Stop the server */
            printf("Server stoping...\n");
            sprintf(buff,"Server is stopped.");
            write(connectionSocket,buff,strlen(buff));
            close(connectionSocket);
            close(welcomeSocket);
            printf("Server stopped. Bye\n");
            break;
          }
          else
          {
            sprintf(buff,"Invalid command.");
            write(connectionSocket,buff,strlen(buff));
          }
        }

      /* Close the connection */
      close(connectionSocket);
      exit(0);
    }

    close(connectionSocket);
  }

  /* Close the socket */
  close(welcomeSocket);
}