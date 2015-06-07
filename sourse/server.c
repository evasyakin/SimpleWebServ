/*
Main file
*/

#include "server.h"

char tmp[4096];
char output[4352];
char head[256];
char content[4096];
header_t header;

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

		HttpParse(buf);

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
		strcat(output,head);
		strcat(output,content);
		*content = *head = 0;
		
		write(sock2,output,strlen(output));
		close(sock2);
		
		continue;
	}
}
void HttpParse(char buf[512]){
	char* pBegin = buf;
	char* pEnd = buf;
	pEnd = strstr(pBegin,"\r\n");
	printf(pBegin);
	if(pEnd)
		*(pEnd) = '\0';

}