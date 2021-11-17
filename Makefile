CXX = g++
CXXFLAGS = -std=c++14
LDFLAGS = -lcgicc -lmysqlcppconn

all: login.cgi register.cgi database.out

login.cgi: login.cpp CgiccInit.cpp MySqlInit.cpp
	$(CXX) $(CXXFLAGS) login.cpp CgiccInit.cpp MySqlInit.cpp -o login.cgi $(LDFLAGS)

register.cgi: register.cpp CgiccInit.cpp MySqlInit.cpp
	$(CXX) $(CXXFLAGS) register.cpp CgiccInit.cpp MySqlInit.cpp -o register.cgi $(LDFLAGS)

database.out: database.cpp
	$(CXX) $(CXXFLAGS) database.cpp -o database.out -lmysqlcppconn

.PHONY: clean
clean:
	-rm -f *.cgi *.out
