main: main.o
	g++ -g main.cpp -L/usr/local/lib/ -lboost_filesystem -ltwitcurl
