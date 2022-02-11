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
 
char *parseRequestType(char firstRequest[], const char sym[]){
	char *copy = (char *)malloc(strlen(firstRequest) + 1);
    	strcpy(copy, firstRequest);
        
    	char *requestType;
    	char *token = strtok(copy, sym);
	int current = 0;

    	while( token != NULL ) {
    	
      		if(current == 0){
        		requestType = token;
          		if(requestType == NULL){
              			strcpy(requestType, "");
          		}
          		free(copy);
          		return requestType;
      		}
      		current = current + 1;
   	}
   	free(copy);
   	free(token);
   	return requestType; 
 
}
 
char *parseSecond(char request[], const char sym[]){
	//char *copyOfRequest = (char *)malloc(strlen(request) + 1);
	char *parsedRequest;
	char *token = strtok(request, sym);
	int currentPlace = 0;
	
	while(token != NULL){
		token = strtok(NULL, " ");
		if(currentPlace == 0){
			parsedRequest = token;
			return parsedRequest;
		}
		currentPlace++;
	}
	
	
	//free(copyOfRequest);
	return parsedRequest;
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
			///*
			valueRead = read(newSocket , buffer, 1024); 
	    		cout << buffer << endl;
	    		
	    		
	    		
	    		send(newSocket , "Welcome to server!" , strlen("Welcome to Server!") , 0); 
	    		
	    		close(newSocket);//*/
	    		
	    		/*valueRead = read(newSocket , buffer, 1024); // info from client
			
	    		//cout << buffer << endl; // prints what client wants?
	    		char *requestType = parseRequestType(buffer, " ");
	    		//cout << "Client request type is: " << requestType << endl;
	    		
	    		
	    		char *parsedRequest = parseSecond(buffer, " ");
	    		//cout << "Parsed Request is: " << parsedRequest << endl;
	    		
	    		//copy parsedRequest to get any file types
	    		char *copyOfParsedRequest;
	    		strcpy(copyOfParsedRequest, parsedRequest);
	    		
	    		if(strcmp(requestType, "GET") == 0){
	    			//cout << "requestType is GET" << endl;
	    			
	    		}
	    		
	    		send(newSocket , "Welcome to server!" , strlen("Welcome to Server!") , 0 ); 
	    		printf("Hello message sent");
	    		
	    		close(newSocket);//*/
	    		
	    	}else{
	    		//cout << "parent create child with pid: " << pid << endl;
	    		close(newSocket);
	    	}

	}	

	cout << "Port is: " << PORT << endl;
	cout << "Hostname is: " << hostBuffer << endl;
	cout << "Host IP is: " << IPBuffer << endl;

	return 0;
}
