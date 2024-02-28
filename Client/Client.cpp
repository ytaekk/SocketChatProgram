#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <WS2tcpip.h>

void ErrorHandling(const char* message);

int main(int argc, char* argv[]) {

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	char tmp[100];
	int strLen;

	// argc=3 은 3개의 인수를 받아야한 다는 것.
	// 여기서는 디버깅 옵션에서 명령줄 인수로 넣어줌(즉 프로그램 기본값)
	// 클라이언트는 IP주소를 입력해야하고 포트번호도 입력해야한다.
	// 따라서 argv[0] = 프로그램 이름 , argv[1] = IP 번호  argv[2] = 포트번호
	// 여기서 IP는 127.0.0.1 , 이 주소는 항상 로컬 컴퓨터를 가르킨다.
	// 즉 서버는 INADDR_ANY(모든 연결 수락) 클라이언트는 127.0.0.1 하면 성공적으로 연결
	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	printf("Program Name: %s\n", argv[0]);
	printf("IP Address: %s\n", argv[1]);
	printf("Port: %s\n", argv[2]);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error!");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	//servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	// 
	// inet_addr 함수는 IPv4 주소를 네트워크 바이트 순서의 이진 값으로 변환하는 기능을 제공하지만, 
	// 최신 프로그래밍 환경에서는 이 함수 대신 inet_pton 또는 InetPton 함수를 사용하는 것이 권장됩니다. 
	// 이 함수들은 IPv6 주소를 포함하여 더 다양한 형태의 IP 주소를 처리할 수 있습니다.
	// 따라서 아래 처럼 작성.
	inet_pton(AF_INET, argv[1], &servAddr.sin_addr);
	servAddr.sin_port = htons(atoi(argv[2]));



	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");

	while (1) {
		printf("메시지 입력 >>");
		// gets대신 gets_s를 쓰는 이유는 gets는 호출시 입력 버퍼의 크기를 확인하지 않기 때문에 사용자가 입력한 데이터 길이가
		// 버퍼 초과하면 인접한 메모리 영역을 덮어쓴다.
		// gets_s 는 호출시 이버퍼의 크기를 인자로 받아 오버플로우를 방지한다.
		gets_s(tmp);
		send(hSocket, tmp, strlen(tmp) + 1, 0);
		strLen = recv(hSocket, message, sizeof(message) - 1, 0);
		if (strLen <= 0)
			ErrorHandling("read() error!");
		printf("Message from server : %s \n", message);
	}

	closesocket(hSocket);
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