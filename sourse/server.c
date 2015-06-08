
/*
Main file
*/

#include "server.h"

char tmp[4096];
char output[4352];
char header[256];
char content[4096];
query_t query;

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

		QueryParse(buf);

		//
		Content("<!DOCTYPE html><html><head><title>hello</title></head><body><h1>Hello, world!</h1>");
		Content("<form method='POST' action=''>" \
			"<p><label>Ваше имя:<br><input type='text' name='name'></label></p>" \
			"<p><label>Ваш e-mail:<br><input type='text' name='email'></label></p>" \
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
void QueryParse(char buf[512]){
	QueryFirstParse(buf);
	printf("Method: %s\n",query.method);
	printf("Url: %s\n",query.url);
	printf("Get: %s\n",query.get);
	printf("HttpVer: %s\n",query.httpVer);
	printf("Host: %s\n",query.host);
	printf("U-A: %s\n",query.userAgent);
	printf("Accept: %s\n",query.accept);
	printf("Accept-Lang: %s\n",query.acceptLanguage);
	printf("Accept-Enc: %s\n",query.acceptEncoding);
	printf("Referer: %s\n",query.referer);
	printf("Connection: %s\n",query.connection);
	printf("Content-Type: %s\n",query.contentType);
	printf("Content-Len: %d\n",query.contentLength);
	printf("CahcheControl: %s\n",query.cacheControl);
	printf("Cookie: %s\n",query.cookie);
	printf("Post: %s\n",query.post);
}
void QueryFirstParse(char buf[512]){
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
			int len = strlen(pBegin);
			if(len > 0){
				pEnd = pBegin + len;
				*(pEnd) = '\0';
				//printf("----\nFOUND!: %s\n----\n", pBegin);
				strcpy(query.post, pBegin);
			}
			else
				break;
		}
		//printf("Str%d: %s\n", count, pBegin);
		QuerySecondParse(pBegin);
		pBegin = pEnd+2;
		count++;
	}
}
void QuerySecondParse(char* str){
	if(strstr(str,"HTTP")){
		sscanf(str,"%s %s HTTP/%s",query.method,query.url,query.httpVer);
		char* get = strstr(query.url, "?");
		if(get != NULL){
			strcpy(query.get, get+1);
		}
	}
	if(strstr(str,"Host:")){
		strcpy(query.host, str+6);
	}
	else if(strstr(str,"User-Agent:")){
		strcpy(query.userAgent, str+12);
	}
	else if(strstr(str,"Accept:")){
		strcpy(query.accept, str+8);
	}
	else if(strstr(str,"Accept-Language:")){
		strcpy(query.acceptLanguage, str+17);
	}
	else if(strstr(str,"Accept-Encoding:")){
		strcpy(query.acceptEncoding, str+17);
	}
	else if(strstr(str,"Referer:")){
		strcpy(query.referer, str+9);
	}
	else if(strstr(str,"Connection:")){
		strcpy(query.connection, str+12);
	}
	else if(strstr(str,"Content-Type:")){
		strcpy(query.contentType, str+14);
	}
	else if(strstr(str,"Content-Length:")){
		sscanf(str,"Content-Length: %d",&query.contentLength);
	}
	else if(strstr(str,"Cache-Control:")){
		strcpy(query.cacheControl, str+15);
	}
}