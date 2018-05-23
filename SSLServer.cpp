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


	/* SSL 库初始化 */
	SSL_library_init();
	/* 载入所有 SSL 算法 */
	OpenSSL_add_all_algorithms();
	/* 载入所有 SSL 错误消息 */
	SSL_load_error_strings();
	/* 以 SSL V2 和 V3 标准兼容方式产生一个 SSL_CTX ，即 SSL Content Text */
	ctx = SSL_CTX_new(SSLv23_server_method());
	/* 也可以用 SSLv2_server_method() 或 SSLv3_server_method() 单独表示 V2 或 V3标准 */
	if (ctx == NULL) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* 载入用户的数字证书， 此证书用来发送给客户端。 证书里包含有公钥 */
	if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* 载入用户私钥 */
	if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	//载入私钥密码，否则终端提示用户手动输入密码  
	/*或者
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



	/* 检查用户私钥是否正确 */
	if (!SSL_CTX_check_private_key(ctx)) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}

	/* 开启一个 socket 监听 */
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
		/* 等待客户端连上来 */
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

		/* 基于 ctx 产生一个新的 SSL */
		ssl = SSL_new(ctx);
		/* 将连接用户的 socket 加入到 SSL */
		SSL_set_fd(ssl, new_fd);
		/* 建立 SSL 连接 */
		if (SSL_accept(ssl) == -1) {
			perror("accept");
			closesocket(new_fd);
			break;
		}

		/* 开始处理每个新连接上的数据收发 */
		//bzero(buf, MAXBUF + 1);
		memset(buf, 0, MAXBUF);
		strcpy(buf, "server->client");
		/* 发消息给客户端 */
		len = SSL_write(ssl, buf, strlen(buf));

		if (len <= 0) {
			printf
			("消息'%s'发送失败！错误代码是%d，错误信息是'%s'\n",
				buf, errno, strerror(errno));
			goto finish;
		}
		else
			printf("消息'%s'发送成功，共发送了%d个字节！\n",
				buf, len);

		//bzero(buf, MAXBUF + 1);
		memset(buf, 0, MAXBUF);
		/* 接收客户端的消息 */
		len = SSL_read(ssl, buf, MAXBUF);
		if (len > 0)
			printf("接收消息成功:'%s'，共%d个字节的数据\n",
				buf, len);
		else
			printf
			("消息接收失败！错误代码是%d，错误信息是'%s'\n",
				errno, strerror(errno));
		/* 处理每个新连接上的数据收发结束 */
	finish:
		/* 关闭 SSL 连接 */
		SSL_shutdown(ssl);
		/* 释放 SSL */
		SSL_free(ssl);
		/* 关闭 socket */
		closesocket(new_fd);
	}

	/* 关闭监听的 socket */
	closesocket(sockfd);
	WSACleanup();
	/* 释放 CTX */
	SSL_CTX_free(ctx);
	return 0;
}