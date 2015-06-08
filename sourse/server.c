/*
Main file
*/

#include "server.h"

char tmp[4096];
char output[4352];
char header[256];
char content[4096];
header_t incoming;

int main(void){
	printf("\
----------------------------------------\n\
    Привет, я - Simple Web Server :)\n\
----------------------------------------\n");

	Conf(); // <- Заполняем струтуру conf
	int sock, retcode;
	struct sockaddr_in serverAddr;
	printf("Запускаю сервер на порту %d\n",conf.port);
	sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sock == -1){
		AddLog("Error","Не могу создать сокет для сервера");
	}

	memset(&serverAddr,0,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(conf.port);
	retcode = bind(sock,(struct sockaddr*)(&serverAddr),sizeof(serverAddr));

	if(retcode == -1){
		AddLog("Error","Ошибка привязки сокета к ip-адресу и номеру порта сервера");
	}
	retcode = listen(sock,10);
	if(retcode == -1){
		AddLog("Error","Ошибка перевода сокета в пассивное состояние");
	}
	Work(sock);
	close(sock);
	return 0;
}
void Work(int sock){
	fd_set ourfds;
	fd_set readfds;
	char buf[8192];
	int len, retcode, sock2;
	socklen_t size;
	struct sockaddr_in clientAddr;
	FD_ZERO(&ourfds);

	while(1){
		printf("Ожидаю подключений ...\n");

		FD_ZERO(&readfds);
		readfds = ourfds;
		FD_SET(sock,&readfds);
		retcode = select(FD_SETSIZE,&readfds,NULL,NULL,NULL);

		if(retcode == -11){
			AddLog("Error","Ошибка select");
		}
		if(FD_ISSET(sock,&readfds)){
			sock2 = accept(sock,(struct sockaddr*)(&clientAddr),&size);
			if(sock2 == -1){
				AddLog("Error","Ошибка получения адреса пользователя");
			}
			printf("Приняли соединение с адреса %s\n",inet_ntoa(clientAddr.sin_addr));
			FD_SET(sock2,&ourfds);
		}
		char buf[512];
		int len = read(sock2,buf,sizeof(buf));

		HeaderParse(buf);

		//
		Content("<!DOCTYPE html><html><head><title>hello</title></head><body><h1>Hello, world!</h1>");
		Content("<form method='POST' action=''><p><label>Ваше имя:<br><input type='text' name='name'></label></p>" \
		"<p><input type='submit' value='Отправить'></form>");
		Content("</body></html>");
		
		Header("HTTP/1.1 200 OK\n");
		Header("Content-Type: text/html; charset=utf-8\n");
		Header("Content-Length: %d\n", strlen(content));
		Header("Server: C\n\n");
		//
		*output = 0;
		strcat(output,header);
		strcat(output,content);
		*content = *header = 0;
		
		write(sock2,output,strlen(output));
		close(sock2);
		
		continue;
	}
}
// ----- HeaderParse -----
void HeaderParse(char buf[512]){
	HeaderFirstParse(buf);
	printf("Method: %s\n",incoming.method);
	printf("Url: %s\n",incoming.url);
	printf("Protocol: %s\n",incoming.protocol);
	printf("Host: %s\n",incoming.host);
	printf("U-A: %s\n",incoming.userAgent);
	printf("Accept: %s\n",incoming.accept);
	printf("Accept-Lang: %s\n",incoming.acceptLanguage);
	printf("Accept-Enc: %s\n",incoming.acceptEncoding);
	printf("Referer: %s\n",incoming.referer);
	printf("Connection: %s\n",incoming.connection);
	printf("Content-Type: %s\n",incoming.contentType);
	printf("Content-Len: %d\n",incoming.contentLength);
	printf("CahcheControl: %s\n",incoming.cacheControl);
}
void HeaderFirstParse(char buf[512]){
	char* pBegin = buf;
	char* pEnd = buf;
	int count = 0;
	printf("-----\n%s\n-----\n",buf);
	while(count < 20){
		//printf("\n>>>>\nStart%d: %s\n", count, pBegin);
		pEnd = strstr(pBegin,"\r\n");
		if(pEnd)
			*(pEnd) = '\0';
		else{
			pEnd = strstr(pBegin,"\n");
			if(pEnd){
				*(pEnd) = '\0';
			}
			else{
				int len = strlen(pBegin);
				if(len > 0)
					pEnd = pBegin + len;
				else
					break;
			}
		}
		//printf("Str%d: %s\n", count, pBegin);
		HeaderSecondParse(pBegin);
		pBegin = pEnd+2;
		count++;
	}
}
void HeaderSecondParse(char* str){
	char* methodGET = strstr(str,"GET");
	char* methodPOST = strstr(str,"POST");
	if(methodGET != NULL || methodPOST != NULL){
		sscanf(str,"%s %s %s",incoming.method,incoming.url,incoming.protocol);

	}
	if(strstr(str,"Host:")){
		sscanf(str,"Host: %s",incoming.host);
	}
	else if(strstr(str,"User-Agent:")){
		strcpy(incoming.userAgent, str+11);
	}
	else if(strstr(str,"Accept:")){
		strcpy(incoming.accept, str+7);
	}
	else if(strstr(str,"Accept-Language:")){
		strcpy(incoming.acceptLanguage, str+16);
	}
	else if(strstr(str,"Accept-Encoding:")){
		strcpy(incoming.acceptEncoding, str+16);
	}
	else if(strstr(str,"Referer:")){
		sscanf(str,"Referer: %s",incoming.referer);
	}
	else if(strstr(str,"Connection:")){
		sscanf(str,"Connection: %s",incoming.connection);
	}
	else if(strstr(str,"Content-Type:")){
		sscanf(str,"Content-Type: %s",incoming.contentType);
	}
	else if(strstr(str,"Content-Length:")){
		sscanf(str,"Content-Length: %d",&incoming.contentLength);
	}
	else if(strstr(str,"Cache-Control:")){
		sscanf(str,"Cache-Control: %s",incoming.cacheControl);
	}
}