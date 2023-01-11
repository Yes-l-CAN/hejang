$(NAME1) : client

$(NAME2) : server


CC = gcc
CXX = C++

all : client server

client : test_client.c
	$(CC)  -o $@ $<

server : echoServer.cpp
	$(CXX)  -o $@ $<

clean:
	rm -f test_client.o echoServer.o

fclean: clean
	rm -rf client
	rm -rf server

re: fclean all

.PHONY : all clean fclean re