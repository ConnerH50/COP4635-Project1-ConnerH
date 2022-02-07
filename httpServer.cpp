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

	int newSocket, serverFd, pid;
	struct sockaddr_in address;

	cout << "Port is: " << PORT << endl;

	return 0;
}
