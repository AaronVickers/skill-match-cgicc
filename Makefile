CXX = g++
CXXFLAGS = -std=c++14
LDFLAGS = -lcgicc -lmysqlcppconn

all: login.cgi register.cgi

login.cgi: login.cpp cgicc-init.h mysql-init.h
	$(CXX) $(CXXFLAGS) login.cpp -o login.cgi $(LDFLAGS)

register.cgi: register.cpp cgicc-init.h mysql-init.h
	$(CXX) $(CXXFLAGS) register.cpp -o register.cgi $(LDFLAGS)

.PHONY: clean
clean:
	-rm -f *.cgi 
