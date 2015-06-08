
/*
Main file
*/

#include "server.h"

char tmp[4096];
char output[4352];
char header[256];
char content[4096];

int main(void){
	printf("\
----------------------------------------\n\
    Привет, я - Simple Web Server :)\n\
----------------------------------------\n");

	Conf(); // <- Заполняем струтуру conf
	int listen_socket, retcode;
	struct sockaddr_in serverAddr;
	printf("Запускаю сервер на порту %d\n",conf.port);
	listen_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(listen_socket == -1){
		AddLog("Error","Не могу создать сокет для сервера");
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(conf.port);
	retcode = bind(listen_socket, (struct sockaddr*)(&serverAddr), sizeof(serverAddr));

	if(retcode == -1){
		AddLog("Error","Ошибка привязки сокета к ip-адресу и номеру порта сервера");
	}
	retcode = listen(listen_socket, 10);
	if(retcode == -1){
		AddLog("Error","Ошибка перевода сокета в пассивное состояние");
	}
	Work(listen_socket);
	close(listen_socket);
	return 0;
}

void Work(int listen_socket){
	fd_set ourfds;
	fd_set readfds;
	char buf[8192];
	int len, retcode, accepted_socket;
	int fork_pid;
	socklen_t size;
	struct sockaddr_in clientAddr;
	FD_ZERO(&ourfds);

	while(1){
		printf("Ожидаю подключений ...\n");

		FD_ZERO(&readfds);
		readfds = ourfds;
		FD_SET(listen_socket, &readfds);
		retcode = select(FD_SETSIZE, &readfds, NULL, NULL, NULL);

		if(retcode == -11){
			AddLog("Error","Ошибка select");
		}
		if(FD_ISSET(listen_socket, &readfds)){
			accepted_socket = accept(listen_socket, (struct sockaddr*)(&clientAddr), &size);
			if(accepted_socket == -1){
				AddLog("Error","Ошибка получения адреса пользователя");
			}
			printf("Приняли соединение с адреса %s\n",inet_ntoa(clientAddr.sin_addr));
			FD_SET(accepted_socket, &ourfds);
		}
		char buf[512];
		int len = read(accepted_socket, buf, sizeof(buf));

		RequestParse(buf);

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
		strcat(output, header);
		strcat(output, content);
		*content = *header = 0;
		
		write(accepted_socket, output, strlen(output));
		close(accepted_socket);
		
		continue;
	}
}

void ChildProcess(int accepted_socket){

}