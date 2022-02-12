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
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

/*
 * Namespaces
 */

using namespace std;

/*
 * DEFINES and other stuff
 */

#define PORT 60069
char http_header[25] = "HTTP/1.1 200 Ok\r\n";



/*
 * Methods
 */
 
/*char *parseRequestType(char firstRequest[], const char sym[]){
	//char *copy = (char *)malloc(strlen(firstRequest) + 1);
    	//strcpy(copy, firstRequest);
        
    	char *requestType;
    	char *token = strtok(firstRequest, sym);
	int current = 0;

    	while( token != NULL ) {
    	
      		if(current == 0){
        		requestType = token;
          		if(requestType == NULL){
              			strcpy(requestType, "");
          		}
          		//free(copy);
          		return requestType;
      		}
      		current = current + 1;
   	}
   	//free(copy);
   	free(token);
   	return requestType; 
 
}
 
char *parseFileNeeded(char request[], const char sym[]){
	
	char *copy = (char *)malloc(strlen(request) + 1);
    	strcpy(copy, request);
    
    	char *message;
    	char * token = strtok(copy, sym);
    	int current = 0;

    	while( token != NULL ) {
      
      		token = strtok(NULL, " ");
      		if(current == 0){
          		message = token;
          		if(message == NULL){
              			message = "";
          		}
          	return message;
      		}
      		current = current + 1;
   	}
   free(token);
   free(copy);
   return message;
}*/


char* parseRequestType(char request[], const char sym[]){

	char *copyOfRequest, *token, *requestType;

	copyOfRequest = (char *)malloc(strlen(request) + 1);
    strcpy(copyOfRequest, request);
    token = strtok(copyOfRequest, sym);
        
    requestType = token;
    int i = 0;

    while(token != NULL) {
    	if(i == 0){
        	requestType = token;
          	if(requestType == NULL){
              	strcpy(requestType, "");
          	}
          	return requestType;
      	}
      	i++;
   }
   free(token);
   free(copyOfRequest);
   return requestType;
}

char* parseFileNeeded(char request[], const char sym[]){

	char *copyOfRequest, *token, *fileNeeded;

    //copyOfRequest = (char *)malloc(strlen(request) + 1);
    //strcpy(copyOfRequest, request);
    
    //tokenize the request copy
    //token = strtok(copyOfRequest, sym);
    
    token = strtok(request, sym);
    
    fileNeeded = token;
    int i = 0;

    while(token != NULL) {
      token = strtok(NULL, " ");
      if(i == 0){
          fileNeeded = token;
          if(fileNeeded == NULL){
              strcpy(fileNeeded, "");
          }
          return fileNeeded;
      }
      i++;
   }
   free(token);
   //free(copyOfRequest);
   return fileNeeded;
}


/*
 * Main method
 */

int main(int argc, char **argv){

	int newSocket, serverFD, pid, hostName, valueRead;
	char hostBuffer[1024];
	char buffer[1024];
	char *IPBuffer;
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
	    	
	    	char *requestType = parseRequestType(buffer, " ");  //Try to get the request type
            cout << "Request Type: "<< requestType << endl;


            char *fileNeeded = parseFileNeeded(buffer, " ");  //Try to get the path which the client ask for
            cout << "File Path: " << fileNeeded << endl;

	    		
	    			/*//parse request type
	    		char *parsedRequestType = parseRequestType(buffer, " ");  //Try to get the path which the client ask for
            	cout << "Request type: " << parsedRequestType << endl;
            		
            		//parse file needed
            	char *parsedFileNeeded = parseFileNeeded(buffer, " ");  //Try to get the path which the client ask for
            	cout << "File needed: " << parsedFileNeeded << endl;*/
            		
	    		
	    	send(newSocket , "Welcome to server!" , strlen("Welcome to Server!") , 0); 
	    		
	    	close(newSocket);
	    		
	    		
	    		
	    	}else{
	    		close(newSocket);
	    	}

	}	

	cout << "Port is: " << PORT << endl;
	cout << "Hostname is: " << hostBuffer << endl;
	cout << "Host IP is: " << IPBuffer << endl;

	return 0;
}
