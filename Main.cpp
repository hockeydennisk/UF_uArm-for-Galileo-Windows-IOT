// Main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "arduino.h"
#include "UF_uArm.h"
//#include "uArm_socket.h"
int __cdecl listening(void);

UF_uArm uarm;
//uArm_Socket socket;

__int16  heightTemp = 0, stretchTemp = 0, rotationTemp = 0, handRotTemp = 0;
char stateMachine = 0, counter = 0;
int _tmain(int argc, _TCHAR* argv[])
{
    return RunArduinoSketch();
}

void setup()
{
	Serial1.begin(CBR_9600, Serial.SERIAL_8N1);
	Log("init() start");
	uarm.init();
	Log("init() end");

}

// the loop routine runs over and over again forever:
void loop()
{
	listening();
}

//Socket

#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

int listening()
{
	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	
	char buf[BUFLEN];
	WSADATA wsa;

	slen = sizeof(si_other);

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done");

	//keep listening for data
	while (1)
	{
		//printf("Waiting for data...");
		fflush(stdout);

		//clear the buffer by filling null, it might have previously received data
		memset(buf, 0, BUFLEN);

		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		/*unsigned char [BUFLEN];*/
		unsigned char *buf_unsigned = (unsigned char *)buf;
		//print details of the client/peer and the data received
		//printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		*((char *)(&rotationTemp)) = buf[3]; // recevive 1byte
		*((char *)(&rotationTemp) + 1) = buf[2];
		*((char *)(&stretchTemp)) = buf[5];
		*((char *)(&stretchTemp) + 1) = buf[4];
		*((char *)(&heightTemp)) = buf[7];
		*((char *)(&heightTemp) + 1) = buf[6];
		*((char *)(&handRotTemp)) = buf[9];
		*((char *)(&handRotTemp) + 1) = buf[8];
		uarm.setPosition(stretchTemp, heightTemp, rotationTemp, handRotTemp);
		if (buf[10] & CATCH)
		{
			printf("CATCH\n");
			uarm.gripperCatch();
		}
		/* pump stop, Valve action.
		Note: The air relief valve can not work for a long time,
		should be less than ten minutes. */
		if (buf[10] & RELEASE)
		{
			printf("RELEASE\n");
			uarm.gripperRelease();
		}

		printf("%02x", buf[0]);
		printf("%02x", buf[1]);
		printf("%02x", buf[2]);
		printf("%02x", buf[3]);
		printf("%02x", buf[4]);
		printf("%02x", buf[5]);
		printf("%02x", buf[6]);
		printf("%02x", buf[7]);
		printf("%02x", buf[8]);
		printf("%02x", buf[9]);
		printf("%02x\n", buf[10]);

		printf("rotationTemp: %02d\n", rotationTemp);
		printf("stretchTemp: %02d\n", stretchTemp);
		printf("heightTemp: %02d\n", heightTemp);
		printf("handRotTemp: %02d\n", handRotTemp);

		uarm.gripperDetach();
		//printf("%02x\n", buf[10]);
		//if (buf[0] == "F" && buf[1] == "F") {

		//}
		//printf("%c\n", buf[0]);
		//printf("stretchTemp: %x\n", stretchTemp);
		//printf("rotationTemp: %x\n", rotationTemp);
		//printf("heightTemp: %x\n", heightTemp);
		//printf("handRotTemp: %x\n", handRotTemp);
		//printf("Data2: %02x\n", buf[3]);
		//now reply the client with the same data
		//if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
		//{
		//	printf("sendto() failed with error code : %d", WSAGetLastError());
		//	exit(EXIT_FAILURE);
		//}
	}

	closesocket(s);
	WSACleanup();

	return 0;
}