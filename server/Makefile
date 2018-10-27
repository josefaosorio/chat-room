#Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa Osorio
#Netid: ktong1, jhardey, josorio2

CXX=		g++
CXFLAGS=	-g -std=c++11 -lpthread
TARGETS=	../chatserver

all: $(TARGETS)

../chatserver: chatserver.o server_op.o network_utils.o
	$(CXX) $(CXFLAGS) -o $@ $^

chatserver.o: chatserver.cpp chatserver.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

server_op.o: server_op.cpp chatserver.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

network_utils.o: ../network_utils/network_utils.cpp ../network_utils/network_utils.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

clean:
	rm $(TARGETS) *.o
