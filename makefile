main: main.o
	g++ -g main.cpp -L/usr/lib/ -lboost_filesystem -ltwitcurl
