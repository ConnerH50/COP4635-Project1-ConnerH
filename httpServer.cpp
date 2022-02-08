/*
 * Conner Hendricks, COP4635 Project 1
 */


/* 
 * C++ includes
 */

#include <iostream>
#include <thread>
#include <mutex>
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
 * DEFINES
 */

#define PORT 60069

/*
 * Other Methods
 */




/*
 * Main method
 */

int main(int argc, char **argv){

	int newSocket, serverFD, pid, hostName;
	char hostBuffer[256];
	char *IPBuffer;
	struct hostent *hostEntry;
	struct sockaddr_in sockAddress;
	int addressLength = sizeof(sockAddress);

	// set address variables
	sockAddress.sin_family = AF_INET;
	sockAddress.sin_addr.s_addr = INADDR_ANY; //set ip of address
	sockAddress.sin_port = htons(PORT); //set port of address
	memset(sockAddress.sin_zero, '\0', sizeof sockAddress.sin_zero); // set last struct variable

	//get host things
	hostName = gethostname(hostBuffer, sizeof(hostBuffer)); //get host name
	hostEntry = gethostbyname(hostBuffer); //get host information. Needed to get IP address
	IPBuffer = inet_ntoa(*((struct in_addr*)hostEntry->h_addr_list[0])); //convert network into string


	// create serverFD w. error checking
	if((serverFD = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		cout << "Error cannot create socket" << endl;
		return 0;
	}

	//Bind the serverFD
	if(bind(serverFD, (struct sockaddr *)&sockAddress, sizeof(sockAddress)) < 0){
		cout << "Error in bind" << endl;
		exit(EXIT_FAILURE);
	}

	//listen on socket
	if(listen(serverFD, 10) < 0){
		cout << "error in listen" << endl;
		exit(EXIT_FAILURE);
	}	

	cout << "Port is: " << PORT << endl;
	cout << "Hostname is: " << hostBuffer << endl;
	cout << "Host IP is: " << IPBuffer << endl;

	return 0;
}
