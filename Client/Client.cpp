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

	// argc=3 �� 3���� �μ��� �޾ƾ��� �ٴ� ��.
	// ���⼭�� ����� �ɼǿ��� ����� �μ��� �־���(�� ���α׷� �⺻��)
	// Ŭ���̾�Ʈ�� IP�ּҸ� �Է��ؾ��ϰ� ��Ʈ��ȣ�� �Է��ؾ��Ѵ�.
	// ���� argv[0] = ���α׷� �̸� , argv[1] = IP ��ȣ  argv[2] = ��Ʈ��ȣ
	// ���⼭ IP�� 127.0.0.1 , �� �ּҴ� �׻� ���� ��ǻ�͸� ����Ų��.
	// �� ������ INADDR_ANY(��� ���� ����) Ŭ���̾�Ʈ�� 127.0.0.1 �ϸ� ���������� ����
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
	// inet_addr �Լ��� IPv4 �ּҸ� ��Ʈ��ũ ����Ʈ ������ ���� ������ ��ȯ�ϴ� ����� ����������, 
	// �ֽ� ���α׷��� ȯ�濡���� �� �Լ� ��� inet_pton �Ǵ� InetPton �Լ��� ����ϴ� ���� ����˴ϴ�. 
	// �� �Լ����� IPv6 �ּҸ� �����Ͽ� �� �پ��� ������ IP �ּҸ� ó���� �� �ֽ��ϴ�.
	// ���� �Ʒ� ó�� �ۼ�.
	inet_pton(AF_INET, argv[1], &servAddr.sin_addr);
	servAddr.sin_port = htons(atoi(argv[2]));



	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");

	while (1) {
		printf("�޽��� �Է� >>");
		// gets��� gets_s�� ���� ������ gets�� ȣ��� �Է� ������ ũ�⸦ Ȯ������ �ʱ� ������ ����ڰ� �Է��� ������ ���̰�
		// ���� �ʰ��ϸ� ������ �޸� ������ �����.
		// gets_s �� ȣ��� �̹����� ũ�⸦ ���ڷ� �޾� �����÷ο츦 �����Ѵ�.
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
// char*�� �ϴϱ� const ����� �Ѵ�.
// C++ǥ�ؿ����� ���ڿ� ���ͷ��� �б����뿡 ����ǰ� const char*�� ó���ȴ�.
// ���⼭ Ÿ�� ����ġ �߻��Ѵ�. const ���� �Լ��� ���޵� ���ڿ��� �������� �ʰڴٴ� �ǹ�
void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}