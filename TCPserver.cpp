/*
	Bind socket to port 8888 on localhost
*/
#include<io.h>
#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main(int argc, char* argv[])
{

	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c;
	const char* message;
	char recvbuf[512];
	int recvbuflen = 512;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(atoi(argv[1]));

	//Bind
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
	}

	puts("Bind done");

	//Listen to incoming connections
	listen(s, 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);
	new_socket = accept(s, (struct sockaddr*)&client, &c);
	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
	}
	puts("Connection accepted");

	int iResult = recv(new_socket, recvbuf, recvbuflen, 0);

	puts("Server Received\n");

	recvbuf[iResult] = '\0';

	FILE* fptr;
	int count = 0;
	if ((fptr = fopen(argv[3], "a")) == NULL) {
		printf("Error! opening file");

		// Program exits if the file pointer returns NULL.
		exit(1);
	}
	fprintf(fptr, "%s", recvbuf);
	fclose(fptr);

	FILE* file_;
	char buffer[100];
	file_ = fopen(argv[2], "r");
	while (!feof(file_)) // to read file
	{
		// function used to read the contents of file
		fread(buffer, sizeof(buffer), 1, file_);
	}
	fseek(file_, 0L, SEEK_SET);
	for (c = getc(file_); c != EOF; c = getc(file_)){

		// Increment count for this character
		count = count + 1;
	}
	
	int size = ftell(file_);
	buffer[count] = '\0';
	fclose(file_);
	
	//Reply to client
	message = "Hello Client , I have received your connection. But I have to go now, bye\n";
	send(new_socket, (const char*)buffer, count, 0);
	puts("End\n");
	getchar();

	closesocket(s);
	WSACleanup();

	return 0;
}