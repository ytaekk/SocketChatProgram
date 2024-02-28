#include <winsock2.h> // ��������� ������ ��� �� 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <istream>


void ErrorHandling(const char* message);

int main(int argc, char* argv[]) {

	WSADATA wsaData; // ������������ȭ 
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	char message[30];
	int strLen;
	
	// argc=2 �� 2���� �μ��� �޾ƾ��� �ٴ� ��.
	// ���⼭�� ����� �ɼǿ��� ����� �μ��� �־���(�� ���α׷� �⺻��)
	// ������ IP�ּҸ� �Է��� �ʿ� ���� ��Ʈ��ȣ�� �ʼ��� �Է��ؾ��Ѵ�.
	// ���� argv[0] = ���α׷� �̸� , argv[1] = ��Ʈ ��ȣ
	// ���� �ӽſ��� ����� �� IP�ּҸ� �Է����� �ʾƵ� ����� �Ǵ� ������ 
	// ������ Ư��IP�ּҸ� �������� �ʰ� ��� ��Ʈ��ũ �������̽�(INADDR_ANY)�� ����
	// ������ ������ �����ϵ��� �����Ǳ� ����.
	// �� ������ INADDR_ANY(��� ���� ����) Ŭ���̾�Ʈ�� 127.0.0.1 �ϸ� ���������� ����

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	// ���������̱� ������ WSAStartup �� ��.
	//���� ����� ���� ���µ�, MAKEWORD �� ���� Ȯ�ο��̶�� �����ϸ� �ȴ�. 
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	// bind�� �ι� ȣ�� �ϸ� ������ �߻��Ѵ�.
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
		printf("�޽��� �Է� >>");
		gets_s(message);
		// gets��� gets_s�� ���� ������ gets�� ȣ��� �Է� ������ ũ�⸦ Ȯ������ �ʱ� ������ ����ڰ� �Է��� ������ ���̰�
		// ���� �ʰ��ϸ� ������ �޸� ������ �����.
		// gets_s �� ȣ��� �̹����� ũ�⸦ ���ڷ� �޾� �����÷ο츦 �����Ѵ�.
		send(hClntSock, message, strlen(message) + 1, 0);

	}

	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();

	return 0;
}
// char*�� �ϴϱ� const ����� �Ѵ�.
// C++ǥ�ؿ����� ���ڿ� ���ͷ��� �б����뿡 ����ǰ� const char*�� ó���ȴ�.
// ���⼭ Ÿ�� ����ġ �߻��Ѵ�. const ���� �Լ��� ���޵� ���ڿ��� �������� �ʰڴٴ� �ǹ�
void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}