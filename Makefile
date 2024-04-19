main: gui.c
	gcc `pkg-config --cflags gtk+-3.0` -o main gui.c `pkg-config --libs gtk+-3.0` -l sqlite3 socket_server.c socket_client.c pthread