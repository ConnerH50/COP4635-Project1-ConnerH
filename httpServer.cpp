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

	int newSocket, serverFD, pid;
	struct sockaddr_in sockAddress;
	int addressLength = sizeof(sockAddress);

	// set address variables
	sockAddress.sin_family = AF_INET;
	sockAddress.sin_addr.s_addr = INADDR_ANY; //set ip of address
	sockAddress.sin_port = htons(PORT); //set port of address

	if((serverFD = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		cout << "Error cannot create socket" << endl;
		return 0;
	}	

	cout << "Port is: " << PORT << endl;

	return 0;
}
