#export LD_LIBRARY_PATH=/home/kazumi/download/affdex_sdk/lib

#Makefile
TARGET=main

CC=g++
OS=$(shell uname | cut -d_ -f1)
CXXFLAGS = -std=c++11 -g -Wl,-rpath,/usr/local/lib/affdex -Wl,-rpath-link,/usr/local/lib/affdex

AFFDEX_PATH_LIB = /usr/local/lib/affdex
AFFDEX_PATH_INC = /usr/local/include/affdex
#ここにインクルードライブラリなどのパスを入力

LDFLAGS=`pkg-config opencv --libs` -L$(AFFDEX_PATH_LIB)
CFLAGS=`pkg-config opencv --cflags` -I$(AFFDEX_PATH_INC)
PROGRAM=$(addsuffix .cpp, $(TARGET))

$(TARGET): $(PROGRAM)
#	$(CC) -o $(TARGET) $(PROGRAM) $(CXXFLAGS) $(CFLAGS) $(LDFLAGS)
	g++ main.cpp -o main -std=c++11 -I/usr/local/include/affdex -L/usr/local/lib/affdex -laffdex-native `pkg-config opencv --cflags` `pkg-config opencv --libs`
	#g++ -std=c++11 $(LDFLAGS) $(CFLAGS) -o test -laffdex-native main.cpp
	#g++ -std=c++11 $(LDFLAGS) -laffdex-native -Wl,-rpath=/usr/local/lib/affdex -Wl,-rpath-link=/usr/local/lib/affdex -o test main.cpp $(CFLAGS)
#	g++ main.cpp -o main -std=c++11 -I/usr/local/include/affdex -L/usr/local/lib/affdex -laffdex-native


clean:
	rm -rf $(TARGET)

