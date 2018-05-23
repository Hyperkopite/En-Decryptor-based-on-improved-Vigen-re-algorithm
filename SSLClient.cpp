#include <WinSock2.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#pragma comment(lib, "WS2_32.lib")

#define MAXBUF 1024

void ShowCerts(SSL * ssl)
{
	X509 *cert;
	char *line;

	cert = SSL_get_peer_certificate(ssl);
	if (cert != NULL) {
		printf("����֤����Ϣ:\n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("֤��: %s\n", line);
		free(line);
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		printf("�䷢��: %s\n", line);
		free(line);
		X509_free(cert);
	}
	else
		printf("��֤����Ϣ��\n");
}

int main(int argc, char **argv)
{
	printf("client\n");
	int sockfd, len;
	struct sockaddr_in dest;
	char buffer[MAXBUF + 1] = { 0 };
	SSL_CTX *ctx;
	SSL *ssl;


	/* SSL ���ʼ�����ο� ssl-server.c ���� */
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	ctx = SSL_CTX_new(SSLv23_client_method());
	if (ctx == NULL) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}

	/* ����һ�� socket ���� tcp ͨ�� */
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)//����0��ʾ�ɹ�
	{
		return 0;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return 0;
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket");
		exit(errno);
	}
	printf("socket created\n");

	/* ��ʼ���������ˣ��Է����ĵ�ַ�Ͷ˿���Ϣ */
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(60005);

	printf("address created\n");

	/* ���ӷ����� */
	if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {
		perror("Connect ");
		exit(errno);
	}
	printf("server connected\n");

	/* ���� ctx ����һ���µ� SSL */
	ssl = SSL_new(ctx);
	SSL_set_fd(ssl, sockfd);
	/* ���� SSL ���� */
	if (SSL_connect(ssl) == -1)
		ERR_print_errors_fp(stderr);
	else {
		printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
		ShowCerts(ssl);
	}

	/* ���նԷ�����������Ϣ�������� MAXBUF ���ֽ� */
	//bzero(buffer, MAXBUF + 1);
	/* ���շ�����������Ϣ */
	len = SSL_read(ssl, buffer, MAXBUF);
	if (len > 0)
		printf("������Ϣ�ɹ�:'%s'����%d���ֽڵ�����\n",
			buffer, len);
	else {
		printf
		("��Ϣ����ʧ�ܣ����������%d��������Ϣ��'%s'\n",
			errno, strerror(errno));
		goto finish;
	}
	//bzero(buffer, MAXBUF + 1);
	memset(buffer, 0, MAXBUF);
	strcpy(buffer, "from client->server");
	/* ����Ϣ�������� */
	len = SSL_write(ssl, buffer, strlen(buffer));
	if (len < 0)
		printf
		("��Ϣ'%s'����ʧ�ܣ����������%d��������Ϣ��'%s'\n",
			buffer, errno, strerror(errno));
	else
		printf("��Ϣ'%s'���ͳɹ�����������%d���ֽڣ�\n",
			buffer, len);

finish:
	/* �ر����� */
	SSL_shutdown(ssl);
	SSL_free(ssl);
	closesocket(sockfd);
	SSL_CTX_free(ctx);
	return 0;
}