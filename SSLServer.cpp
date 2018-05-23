#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#pragma comment(lib, "WS2_32.lib")

#define MAXBUF 1024

int main(int argc, char **argv)
{
	//int sockfd, new_fd;
	printf("server\n");
	SOCKET sockfd, new_fd;
	int len;
	struct sockaddr_in my_addr, their_addr;
	unsigned int myport = 60005, lisnum = 2;
	char buf[MAXBUF + 1];
	SSL_CTX *ctx;


	/* SSL ���ʼ�� */
	SSL_library_init();
	/* �������� SSL �㷨 */
	OpenSSL_add_all_algorithms();
	/* �������� SSL ������Ϣ */
	SSL_load_error_strings();
	/* �� SSL V2 �� V3 ��׼���ݷ�ʽ����һ�� SSL_CTX ���� SSL Content Text */
	ctx = SSL_CTX_new(SSLv23_server_method());
	/* Ҳ������ SSLv2_server_method() �� SSLv3_server_method() ������ʾ V2 �� V3��׼ */
	if (ctx == NULL) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* �����û�������֤�飬 ��֤���������͸��ͻ��ˡ� ֤��������й�Կ */
	if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* �����û�˽Կ */
	if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	//����˽Կ���룬�����ն���ʾ�û��ֶ���������  
	/*����
	#include <openssl/ssl.h>
	void SSL_CTX_set_default_passwd_cb(SSL_CTX *ctx, pem_password_cb *cb);
	void SSL_CTX_set_default_passwd_cb_userdata(SSL_CTX *ctx, void *u);
	int pem_passwd_cb(char *buf, int size, int rwflag, void *userdata);

	int pem_passwd_cb(char *buf, int size, int rwflag, void *password)
	{
	strncpy(buf, (char *)(password), size);
	buf[size - 1] = '\0';
	return(strlen(buf));
	}
	*/
	//SSL_CTX_set_default_passwd_cb_userdata(ctx, "123456");



	/* ����û�˽Կ�Ƿ���ȷ */
	if (!SSL_CTX_check_private_key(ctx)) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}

	/* ����һ�� socket ���� */
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return 0;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return 0;
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	else
		printf("socket created\n");
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr("10.32.16.174");
	my_addr.sin_port = htons(myport);

	if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr))
		== -1) {
		perror("bind");
		exit(1);
	}
	else
		printf("binded\n");

	if (listen(sockfd, lisnum) == -1) {
		perror("listen");
		exit(1);
	}
	else
		printf("begin listen\n");

	while (1) {
		SSL *ssl;
		len = sizeof(struct sockaddr);
		/* �ȴ��ͻ��������� */
		if ((new_fd =
			accept(sockfd, (struct sockaddr *) &their_addr,
				&len)) == -1) {
			perror("accept");
			exit(errno);
		}
		else
			printf("server: got connection from %s, port %d, socket %d\n",
				inet_ntoa(their_addr.sin_addr),
				ntohs(their_addr.sin_port), new_fd);

		/* ���� ctx ����һ���µ� SSL */
		ssl = SSL_new(ctx);
		/* �������û��� socket ���뵽 SSL */
		SSL_set_fd(ssl, new_fd);
		/* ���� SSL ���� */
		if (SSL_accept(ssl) == -1) {
			perror("accept");
			closesocket(new_fd);
			break;
		}

		/* ��ʼ����ÿ���������ϵ������շ� */
		//bzero(buf, MAXBUF + 1);
		memset(buf, 0, MAXBUF);
		strcpy(buf, "server->client");
		/* ����Ϣ���ͻ��� */
		len = SSL_write(ssl, buf, strlen(buf));

		if (len <= 0) {
			printf
			("��Ϣ'%s'����ʧ�ܣ����������%d��������Ϣ��'%s'\n",
				buf, errno, strerror(errno));
			goto finish;
		}
		else
			printf("��Ϣ'%s'���ͳɹ�����������%d���ֽڣ�\n",
				buf, len);

		//bzero(buf, MAXBUF + 1);
		memset(buf, 0, MAXBUF);
		/* ���տͻ��˵���Ϣ */
		len = SSL_read(ssl, buf, MAXBUF);
		if (len > 0)
			printf("������Ϣ�ɹ�:'%s'����%d���ֽڵ�����\n",
				buf, len);
		else
			printf
			("��Ϣ����ʧ�ܣ����������%d��������Ϣ��'%s'\n",
				errno, strerror(errno));
		/* ����ÿ���������ϵ������շ����� */
	finish:
		/* �ر� SSL ���� */
		SSL_shutdown(ssl);
		/* �ͷ� SSL */
		SSL_free(ssl);
		/* �ر� socket */
		closesocket(new_fd);
	}

	/* �رռ����� socket */
	closesocket(sockfd);
	WSACleanup();
	/* �ͷ� CTX */
	SSL_CTX_free(ctx);
	return 0;
}