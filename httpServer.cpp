/*
 * Conner Hendricks, COP4635 Project 1
 * 2/13/2022
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
#include <bits/stdc++.h>
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
 
 /*
 * Method to parse the client's request
 */

vector<char *> getRequestInfo(char request[]){
	vector<char *> requestInfo;
	char *requestType, *filePath, *fileExt, *token;
	char *string = strtok(request, "\n");
	
	// tokenize by " ", " ", and "."
	
	//get requestType
	token = strtok(string, " ");
	requestType = token;
	requestInfo.push_back(requestType);
	
	
	//get filePath
	token = strtok(NULL, " ");
	filePath = token;
	requestInfo.push_back(filePath);
	
	
	// get fileExt
	token = strtok(filePath, ".");
	token = strtok(NULL, ".");
	fileExt = token;
	requestInfo.push_back(fileExt);
	
	
	return requestInfo;

}

/*
 * Method to send client the message
 */

void sendClientMessage(int socket, char httpPath[], char copyOfHeader[]){
	cout << "http path: " << httpPath << endl;
	
	struct stat inputFile;  // hold information about input file 

    write(socket, copyOfHeader, strlen(copyOfHeader));

    int serverFDTransfer = open(httpPath, O_RDONLY); //open for byte transfer, read only
    
    if(serverFDTransfer < 0){
        perror("Cannot open file path!");
        send(socket , "404 Not Found!", strlen("404 Not Found!") , 0); //404 error stuff
        close(socket);
    }
     
    fstat(serverFDTransfer, &inputFile); // initialize inputFile with info from serverFDTransfer
    
    
    int totalSize = inputFile.st_size; //total size of the file
    int blockSize = inputFile.st_blksize;
    
    
    if(serverFDTransfer >= 0){

        while(totalSize > 0){ 
              int sentBytes = sendfile(socket, serverFDTransfer, NULL, blockSize);
              totalSize -= sentBytes;
             
        }
        close(serverFDTransfer); //close byte transfer
    }
}

/*
 * Methods to create and then send the various http headers and paths to the client
 */

void sendDefaultPage(char *header, int socket){
	char httpPath[1024] = ".";
	strcat(httpPath, "/index.html");
    strcat(header, "Content-Type: text/html\r\n\r\n");
    sendClientMessage(socket, httpPath, header);

}

void sendHTML(char *header, int socket, char *filePath, char* fileExtension){
	char httpPath[1024] = ".";
	strcat(httpPath, filePath);
	strcat(httpPath, ".");
	strcat(httpPath, fileExtension);
	strcat(header, "Content-Type: text/html\r\n\r\n");
    sendClientMessage(socket, httpPath, header);
} 

void sendJPEG(char *header, int socket, char *filePath, char* fileExtension){
	char httpPath[1024] = ".";
	strcat(httpPath, filePath);
	strcat(httpPath, ".");
	strcat(httpPath, fileExtension);
	strcat(header, "Content-Type: image/jpeg\r\n\r\n");
	sendClientMessage(socket, httpPath, header);
}


/*
 * Main method
 */

int main(int argc, char **argv){

	int newSocket, serverFD, pid;
	vector<char *> requestData;
	char hostBuffer[1024];
	char buffer[1024];
	char *IPBuffer, *requestType, *filePath, *fileExtension;
	struct hostent *hostEntry;
	struct sockaddr_in sockAddress;
	int addressLength = sizeof(sockAddress);
	
	//get host things
	gethostname(hostBuffer, sizeof(hostBuffer)); //get host name
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
	cout << "Host Name is: " << hostBuffer << endl;
	cout << "Host IP is: " << IPBuffer << endl;

	while(1){ // Infinite loop for the server
	
		cout << endl << "---------------------------------------------" << endl << endl;
	
		
		//Accept testing
		if((newSocket = accept(serverFD, (struct sockaddr *)&sockAddress, (socklen_t*)&addressLength)) < 0){
			cout << "Error in accept testing" << endl;
			exit(EXIT_FAILURE);
		}
		
		//fork process
		if((pid = fork()) < 0){
			cout << "Error in fork" << endl;
			exit(EXIT_FAILURE);
		}
		
		if(pid == 0){ 
			read(newSocket , buffer, 1024); //get request data
	    	cout << buffer << endl;
	    	
	    	requestData = getRequestInfo(buffer); // parse request into vector
            
            requestType = requestData[0];  //get request type
            cout << "Request Type: "<< requestType << endl;

            filePath = requestData[1];  //get needed file
            cout << "File Path: " << filePath << endl;
            
            fileExtension = requestData[2]; //get file extension
            cout << "File Extension: " << fileExtension << endl;
            

   			char *copyOfHeader = (char *)malloc(strlen(httpHeader) +200);
            strcpy(copyOfHeader, httpHeader);
	    	
	    	if(strcmp(requestType, "GET") == 0){
	    		if((strcmp(filePath, "/") == 0) || (strcmp(filePath, "") == 0)){
                    sendDefaultPage(copyOfHeader, newSocket);
	    		}else if(strcmp(fileExtension, "html") == 0){
	    			sendHTML(copyOfHeader, newSocket, filePath, fileExtension);
	    		}else if((strcmp(fileExtension, "jpg") == 0) || (strcmp(fileExtension, "jpeg") == 0)){
	    			sendJPEG(copyOfHeader, newSocket, filePath, fileExtension);
	    		}
	    	
	    	}
	    	
	    	free(copyOfHeader);
	    	cout << "closing socket" << endl;	
	    	close(newSocket);
	    		
	    	}else{
	    		close(newSocket);
	    	}

	}

	close(serverFD);
	return 0;
}
