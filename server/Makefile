#Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa Osorio
#Netid: ktong1, jhardey, josorio2

CXX=		g++
CXFLAGS=	-g -std=c++11 -lpthread -lcrypto
CRYPTOFLAGS= -lcrypto
TARGETS=	../chatserver

all: $(TARGETS)

../chatserver: chatserver.o server_op.o network_utils.o ClientMap.o
	$(CXX) $(CXFLAGS) -o $@ $^

chatserver.o: chatserver.cpp chatserver.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

server_op.o: server_op.cpp chatserver.h ../network_utils/pg3lib.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

network_utils.o: ../network_utils/network_utils.cpp ../network_utils/network_utils.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

ClientMap.o: ClientMap.cpp chatserver.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

clean:
	rm $(TARGETS) *.o
