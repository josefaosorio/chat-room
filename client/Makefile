#Name: Kwan Ho Herman Tong, Jessica Hardey, Josefa Osorio
#Netid: ktong1, jhardey, josorio2

CXX=		g++
CXFLAGS=	-g -std=c++11 -lpthread -lcrypto
TARGETS=	../chatclient

all: $(TARGETS)

../chatclient: chatclient.o client_op.o network_utils.o recv_thread.o
	$(CXX) $(CXFLAGS) -o $@ $^

chatclient.o: chatclient.cpp chatclient.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

client_op.o: client_op.cpp chatclient.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

network_utils.o: ../network_utils/network_utils.cpp ../network_utils/network_utils.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

recv_thread.o: recv_thread.cpp chatclient.h ../network_utils/network_utils.h
	$(CXX) $(CXFLAGS) -o $@ -c $<

clean:
	rm $(TARGETS) *.o
