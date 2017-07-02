#include "server.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

#include <pthread.h>

#include "Base.h" // define extern int threadScanEvent_id; 
#include "RawSignal.h" // define extern bool endRawSignal; 

#define BUFSIZE 1000

int clientSocket=-1; // if of client socket


int socket_getline(char* buf, int size)
{
	int i=0;
	for (i=0; i<size; i++) {
		if ( recv(clientSocket, buf+i, 1, 0) <= 0){
			clientSocket=-1;
			Serial.set_send_function(NULL); // disable Serial.println send line by socket
			log(LOG_ERROR,"TCP server: Problem with receivere data.");
			return -1;
		}
		if ( buf[i]=='\r'){ // ignor
			buf[i]=0x00;
			i--;
		}
		if ( buf[i]=='\r' or buf[i]=='\n') {
			buf[i]=0x00;
			return i;
		}
	}
}

int socket_send(const char* buf, const int size)
{
	log(LOG_STATUS,"TCP server send: ",false);
	log(LOG_STATUS,buf);
	int ret=send(clientSocket, buf, size, 0);
	if ( ret == -1 ) {  // +1 for NUL terminator
		clientSocket=-1;
		Serial.set_send_function(NULL); // disable Serial.println send line by socket = disable this function
		log(LOG_ERROR,"TCP server: Problem send data.");
	}
	return ret;
}

int TCPserver(int port)
{
	log(LOG_STATUS,"TCP server: START.");
	sockaddr_in sockName;         // "name" of port
	sockaddr_in clientInfo;       // client who connected
	int mainSocket;               // Soket
	char buf[BUFSIZE];            // Přijímací buffer
	char inBuf[BUFSIZE];            // Input buffer
	int size;                     // count of sender or receivered bytes
	socklen_t addrlen;            // size remote addres

	// Create main socket
	if ((mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		log(LOG_ERROR,"TCP server: Main socket can't be created.");
		return -1;
	}
	sockName.sin_family = AF_INET;
	sockName.sin_port = htons(port);
	sockName.sin_addr.s_addr = INADDR_ANY; //allow connect from anywhere
	if (bind(mainSocket, (sockaddr *)&sockName, sizeof(sockName)) == -1) // add name to scoket
	{
		log(LOG_ERROR,"TCP server: Problem add name to socket.");
		return -1;
	}
	if (listen(mainSocket, 1) == -1) // create reguest queue  (max 1)
	{
		log(LOG_ERROR,"TCP server: Problem create reguest queue.");
		return -1;
	}
	do
	{
		sprintf(pbuffer,"TCP server: Wait for next connection on port: %d .",port);
		log(LOG_STATUS, pbuffer);
		addrlen = sizeof(clientInfo); //size struct clienInfo for function accept
		clientSocket = accept(mainSocket, (sockaddr*)&clientInfo, &addrlen); // accpet one connection from queue, client is new soccket join clinet with server
		int totalSize = 0;
		if (clientSocket == -1)
		{
			log(LOG_ERROR,"TCP server: Problem conect with client.");
			return -1;
		}
		sprintf(pbuffer, "TCP server: Connect client with addr: %s.", inet_ntoa((in_addr)clientInfo.sin_addr) );
		log(LOG_STATUS, pbuffer);
		//log(LOG_STATUS,"TCP server: Client connect.");
		// --------------------- conction is stored now can work -----------------------------
		Serial.set_send_function(socket_send); // enable Serial.println send line by socket
		Serial.print(WELCOME);
		sprintf(inBuf,"R%02x;",REVNR);
		Serial.println(inBuf); 
		StartScanEventTheader(); // create separate thread
		while( clientSocket!=-1 ){ // enableRawScan is use together as flag for enable server
			if (socket_getline(inBuf, BUFSIZE)==-1){ //get line into inBuf
				log(LOG_ERROR,"TCP server: End when reading input.");
				break;
			}
			//std::cout << "client input:" << inBuf << std::endl;
			if (serve_input(inBuf)==-1) { // serve line
				//std::cout << "end from input:" << std::endl;
				log(LOG_ERROR,"TCP server: End from input.");
				break;
			}
		}
		Serial.set_send_function(NULL); // disable Serial.println send line by socket
		close(clientSocket);
		//std::cout << "Connection close." << std::endl;
		log(LOG_ERROR,"TCP server: Connection close.");
		StopScanEventTheader();
	}
	while (true);
	// next newer run (only for inspiration)
	log(LOG_STATUS,"TCP server: END.");
	close(mainSocket);
	return 0;
}
