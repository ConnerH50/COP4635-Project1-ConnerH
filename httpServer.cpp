/*
 * Conner Hendricks, COP4635 Project 1
 */


/* 
 * C++ includes
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable> //May not need

/*
 * C includes
 */

#include <string.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

/*
 * Namespaces
 */

using namespace std;

/*
 * DEFINES and other declarations
 */

#define PORT 60069
char httpHeader[25] = "HTTP/1.1 200 Ok\r\n"; //needed for sending stuff



/*
 * Methods
 */
 
//Parse the request type
char* getRequestType(char request[], const char parseSym[]){

	char *copyOfRequest, *token, *requestType;

	copyOfRequest = (char *)malloc(strlen(request) + 1);
    strcpy(copyOfRequest, request);
    
    //Tokenize the request copy
    token = strtok(copyOfRequest, parseSym);
        
    requestType = token;
    int i = 0;

    do{
    	if(i == 0){
        	requestType = token;
          	if(requestType == NULL){
              	strcpy(requestType, "");
          	}
          	return requestType;
      	}
      	i++;
   }while(token != NULL);
   
   //free everything allocated
   free(token);
   free(copyOfRequest);
   return requestType;
}

char* getFileNeeded(char request[], const char parseSym[]){

	char *copyOfRequest, *token, *fileNeeded;

    copyOfRequest = (char *)malloc(strlen(request) + 1);
    strcpy(copyOfRequest, request);
    
    //tokenize the request copy
    token = strtok(copyOfRequest, parseSym);
    
    fileNeeded = token;
    int i = 0;

    do{
      token = strtok(NULL, " ");
      if(i == 0){
          fileNeeded = token;
          if(fileNeeded == NULL){
              strcpy(fileNeeded, "");
          }
          return fileNeeded;
      }
      i++;
   }while(token != NULL); 
   
   //free everything
   free(token);
   free(copyOfRequest);
   return fileNeeded;
}

char* getFileExtension(char request[], const char parseSym[]){

	char *copyOfFileNeeded, *token, *fileExt;

	copyOfFileNeeded = (char *)malloc(strlen(request) + 1);
    strcpy(copyOfFileNeeded, request);
    
    token = strtok(copyOfFileNeeded, parseSym); //tokenize by "."
    fileExt = token;
    
    int i = 0;
    
    /*while(token != NULL){
    	token = strtok(NULL, " ");
    	cout << "Token: " << token << endl;
    	
    	//token = strtok(NULL, " ");
      	
    }*/
    
	//free stuff
	free(copyOfFileNeeded);
	return fileExt;
}

int sendClientMessage(int socket, char httpPath[], char copyOfHeader[]){
	cout << "http path: " << httpPath << endl;
	cout << "copy of header: " << copyOfHeader << endl;
	
	//	NEED TO DELETE WHEN I FIGURE OUT HOW IT WORKS
	
	struct stat inputFile;  /* hold information about input file */

    write(socket, copyOfHeader, strlen(copyOfHeader));

    int serverFDTransfer = open(httpPath, O_RDONLY); //open for byte transfer
    
    if(serverFDTransfer < 0){
        perror("Cannot open file path!");
    }
     
    fstat(serverFDTransfer, &inputFile);
    int totalSize = inputFile.st_size;
    int blockSize = inputFile.st_blksize;
    
    
    if(serverFDTransfer >= 0){
        ssize_t sentSize;

        while(totalSize > 0){
    
              int sendingBytes = ((totalSize < blockSize) ? totalSize : blockSize); // need to turn this into an if statement
              int sentBytes = sendfile(socket, serverFDTransfer, NULL, blockSize);
              totalSize -= sentBytes;
        }
        if(sentSize >= 0){
            printf("send file: %s \n" , httpPath);
        }
        close(serverFDTransfer);
    }
	
	return 0;
}


/*
 * Main method
 */

int main(int argc, char **argv){

	int newSocket, serverFD, pid, hostName, valueRead;
	char hostBuffer[1024];
	char buffer[1024];
	char *IPBuffer, *requestType, *fileNeeded, *copyOfFileNeeded, *fileExtension;
	struct hostent *hostEntry;
	struct sockaddr_in sockAddress;
	int addressLength = sizeof(sockAddress);
	
	//get host things
	hostName = gethostname(hostBuffer, sizeof(hostBuffer)); //get host name
	hostEntry = gethostbyname(hostBuffer); //get host information. Needed to get IP address
	IPBuffer = inet_ntoa(*((struct in_addr*)hostEntry->h_addr_list[0])); //convert network into string

	// set address variables
	sockAddress.sin_family = AF_INET;
	sockAddress.sin_addr.s_addr = INADDR_ANY; //set ip of address
	sockAddress.sin_port = htons(PORT); //set port of address
	memset(sockAddress.sin_zero, '\0', sizeof sockAddress.sin_zero); // set last struct variable


	// create serverFD w. error checking
	if((serverFD = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Error cannot create socket");
		return -1;
	}

	//Bind the serverFD
	if(bind(serverFD, (struct sockaddr *)&sockAddress, sizeof(sockAddress)) < 0){
		perror("Error in bind");
		return -1;
	}

	//listen on socket
	if(listen(serverFD, 10) < 0){
		perror("error in listen");
		return -1;
	}

	cout << "Port is: " << PORT << endl;
	cout << "Hostname is: " << hostBuffer << endl;
	cout << "Host IP is: " << IPBuffer << endl;

	while(1){ // Infinite loop for the server
	
		printf("\n<<<<<<<<< Waiting for connection!! >>>>>>>>>\n\n");
	
		
		//Accept testing
		if((newSocket = accept(serverFD, (struct sockaddr *)&sockAddress, (socklen_t*)&addressLength)) < 0){
			cout << "Error in accept testing" << endl;
			exit(EXIT_FAILURE);
		}
		
		//fork process
		pid = fork();
		if(pid < 0){
			cout << "Error in fork" << endl;
			exit(EXIT_FAILURE);
		}
		
		if(pid == 0){ //child 
			//send to server
			valueRead = read(newSocket , buffer, 1024); 
	    	cout << buffer << endl;
	    	
	    	requestType = getRequestType(buffer, " ");  //get request type
            cout << "Request Type: "<< requestType << endl;


            fileNeeded = getFileNeeded(buffer, " ");  //get needed file
            cout << "File Path: " << fileNeeded << endl;
            
            fileExtension = getFileExtension(fileNeeded, ".");
            cout << "File Extension: " << fileExtension << endl;

   			char *copyOfHeader = (char *)malloc(strlen(httpHeader) +200);
            strcpy(copyOfHeader, httpHeader);
	    	
	    	if(strcmp(requestType, "GET") == 0){
	    		//cout << "GET" << endl;
	    		if((strcmp(fileNeeded, "/") == 0) || (strcmp(fileNeeded, "") == 0)){
	    			//cout << "Need /index.html" << endl;
	    			char httpPath[1024] = ".";
	    			strcat(httpPath, "/index.html");
                    strcat(copyOfHeader, "Content-Type: text/html\r\n\r\n");
                    sendClientMessage(newSocket, httpPath, copyOfHeader);
	    		}
	    	
	    	}
	    	
	    	send(newSocket , "Welcome to server!", strlen("Welcome to Server!") , 0); 
	    	
	    	free(copyOfHeader);
	    	cout << "closing socket" << endl;	
	    	close(newSocket);
	    		
	    		
	    		
	    	}else{
	    		close(newSocket);
	    	}

	}	

	cout << "Port is: " << PORT << endl;
	cout << "Hostname is: " << hostBuffer << endl;
	cout << "Host IP is: " << IPBuffer << endl;

	close(serverFD);
	return 0;
}
