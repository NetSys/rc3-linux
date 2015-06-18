#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<netinet/tcp.h>
#include<sys/wait.h>
#include<time.h>
#include<sys/time.h>
#include<fcntl.h>

#define BACKLOG 1000  //no. of pending connections (passed as parameter to listen function)
#define MSGSZ 5120000



void main(int argc, char *argv[])
{
  int my_port;           //port at which the server binds
  int socket_fd, new_fd; // socket_fd is returned by the socket function (server listens on this), new_fd is returned by accept function, when a client connects
  struct sockaddr_in my_addr;   //to store my address information
  struct sockaddr_in client_addr; //to store client's address information
  int sin_size, numbytes;
  int size, totalbytes;
  fd_set rfds;
  struct timeval tv;
  int retval;
  int fdA, fdB;

  if(argc!=3)
  {
    fprintf(stderr, "usage: receiver port size\n");
    exit(1);
  }

  my_port = atoi(argv[1]);
  size = atoi(argv[2]);

  char filename[100];
 
  sprintf(filename, "regularA_%d.txt", size);
  if((fdA = open(filename, O_CREAT|O_WRONLY|O_APPEND)) == -1)
	perror("open");
  
  sprintf(filename, "regularB_%d.txt", size);
  if((fdB = open(filename, O_CREAT|O_WRONLY|O_APPEND)) == -1)
	perror("open");

  /*creating the socket with:
   address family = AF_INET (IPv4 addresses)
   type = SOCK_STREAM (TCP)
   protocol = 0 */
  if((socket_fd = socket(AF_INET, SOCK_STREAM, 0))==-1)
  {
    perror("socket");
    exit(1);
  }

  //updating the server's address information
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(my_port);   //htons converts the byte order from host machine's to networking standards
  my_addr.sin_addr.s_addr = INADDR_ANY; //auto filling with host IP address
  bzero(&(my_addr.sin_zero), 8);    //rest of the bytes are assigned 0 value

  //binding my address information to the socket
  if((bind(socket_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)))==-1)
  {
    perror("bind");
    exit(1);
  }

  //listening to the connections
  if(listen(socket_fd, BACKLOG)==-1)
  {
    perror("listen");
    exit(1);
  }


  //Part 2: Accepting connections serially
  int i=0;
  for(i=0; i<100; i++)
  {
 	struct timeval listime, acctime, recvtime;
  	sin_size = sizeof(struct sockaddr_in);
			
  	//accepting the client's connection
  	if((new_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &sin_size))==-1)
  	{
      		perror("accept");
      		exit(1);
  	}
	
	//if(!fork())
	//{
  		gettimeofday(&acctime,NULL);
  

  		//int priority = 5;
  		//setsockopt(new_fd, SOL_SOCKET, SO_PRIORITY, &priority, sizeof(int));

  		int optlen = sizeof(int);
    
  		int quickack = 1;
  		setsockopt(new_fd, IPPROTO_TCP, TCP_QUICKACK, &quickack, sizeof(int));

  		int nodelay = 1;
  		setsockopt(new_fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(int));

    
  		int rc3 = 0;
  		setsockopt(new_fd, SOL_SOCKET, 60, &rc3, sizeof(int));



  		int logme = 0;
  		setsockopt(new_fd, SOL_SOCKET, 61, &logme, sizeof(int));

  		int logtime = 0;
  		setsockopt(new_fd, SOL_SOCKET, 62, &logtime, sizeof(int));

  		char msg[MSGSZ];
  		int i, j, flag, n;

    		//Receiving data
  
    
  		tv.tv_sec = 5;
  		tv.tv_usec = 0;

  		totalbytes = 0;
      
  		while(totalbytes < size)
  		{
          		FD_ZERO(&rfds);
          		FD_SET(new_fd, &rfds);
          		retval = select(new_fd + 1, &rfds, NULL, NULL, &tv);
          		if(retval == -1)
            			perror("select");
          		if(retval && FD_ISSET(new_fd, &rfds))
          		{
              			if((numbytes = recv(new_fd, (char*)msg, MSGSZ, 0))==-1)
              			{
                  			perror("recv");
                  			exit(1);
              			}
              			totalbytes += numbytes;
          		}
  		}
      
      
  		gettimeofday(&recvtime, NULL);
  
  		double fctB = (double)((recvtime.tv_sec - acctime.tv_sec)*1000000 + recvtime.tv_usec - acctime.tv_usec)/1000;
		double fctA = fctB + 30.3;

  		printf("%lf\n", fctB/1000);
		dprintf(fdA, "%lf\n", fctA/1000);
		dprintf(fdB, "%lf\n", fctB/1000);
		
  		close(new_fd);
	}
   //}
}


