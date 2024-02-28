#include <winsock2.h> // 윈도우즈기 때문에 들어 감 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <istream>


void ErrorHandling(const char* message);

int main(int argc, char* argv[]) {

	WSADATA wsaData; // 윈도우즈최적화 
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	char message[30];
	int strLen;
	
	// argc=2 은 2개의 인수를 받아야한 다는 것.
	// 여기서는 디버깅 옵션에서 명령줄 인수로 넣어줌(즉 프로그램 기본값)
	// 서버는 IP주소를 입력할 필요 없고 포트번호는 필수로 입력해야한다.
	// 따라서 argv[0] = 프로그램 이름 , argv[1] = 포트 번호
	// 같은 머신에서 통신할 때 IP주소를 입력하지 않아도 통신이 되는 이유는 
	// 서버가 특정IP주소를 지정하지 않고 모든 네트워크 인터페이스(INADDR_ANY)를 통해
	// 들어오는 연결을 수락하도록 구성되기 때문.
	// 즉 서버는 INADDR_ANY(모든 연결 수락) 클라이언트는 127.0.0.1 하면 성공적으로 연결

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	// 윈도우즈이기 때문에 WSAStartup 이 들어감.
	//소켓 만들기 전에 들어가는데, MAKEWORD 는 버전 확인용이라고 생각하면 된다. 
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	// bind를 두번 호출 하면 오류가 발생한다.
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error");
	while (1) {
		memset(message, '\0', 30);
		strLen = recv(hClntSock, message, sizeof(message) - 1, 0);
		if (strLen == -1)
			ErrorHandling("read() error!");
		printf("Message from client : %s \n", message);
		printf("메시지 입력 >>");
		gets_s(message);
		// gets대신 gets_s를 쓰는 이유는 gets는 호출시 입력 버퍼의 크기를 확인하지 않기 때문에 사용자가 입력한 데이터 길이가
		// 버퍼 초과하면 인접한 메모리 영역을 덮어쓴다.
		// gets_s 는 호출시 이버퍼의 크기를 인자로 받아 오버플로우를 방지한다.
		send(hClntSock, message, strlen(message) + 1, 0);

	}

	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();

	return 0;
}
// char*로 하니까 const 쓰라고 한다.
// C++표준에서는 문자열 리터럴이 읽기전용에 저장되고 const char*로 처리된다.
// 여기서 타입 불일치 발생한다. const 쓰면 함수가 전달된 문자열을 변경하지 않겠다는 의미
void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}