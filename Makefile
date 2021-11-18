# Compiler options
CXX = g++
CXXFLAGS = -std=c++14
LDFLAGS = -lcgicc -lmysqlcppconn

# All output files
all: login.cgi register.cgi tfa.cgi database.out

# Login CGI file
login.cgi: login.cpp CgiccInit.o MySqlInit.o Authentication.o
	$(CXX) $(CXXFLAGS) login.cpp CgiccInit.o MySqlInit.o Authentication.o -o login.cgi $(LDFLAGS)

# Register CGI file
register.cgi: register.cpp CgiccInit.o MySqlInit.o Authentication.o
	$(CXX) $(CXXFLAGS) register.cpp CgiccInit.o MySqlInit.o Authentication.o -o register.cgi $(LDFLAGS)

# 2FA CGI file
tfa.cgi: tfa.cpp CgiccInit.o MySqlInit.o Authentication.o
	$(CXX) $(CXXFLAGS) tfa.cpp CgiccInit.o MySqlInit.o Authentication.o -o tfa.cgi $(LDFLAGS)

# Database population file
database.out: database.cpp
	$(CXX) $(CXXFLAGS) database.cpp -o database.out -lmysqlcppconn

# CgiccInit object file
CgiccInit.o: CgiccInit.cpp CgiccInit.h
	$(CXX) $(CXXFLAGS) CgiccInit.cpp -c

# MySqlInit object file
MySqlInit.o: MySqlInit.cpp MySqlInit.h
	$(CXX) $(CXXFLAGS) MySqlInit.cpp -c

# Authentication object file
Authentication.o: Authentication.cpp Authentication.h
	$(CXX) $(CXXFLAGS) Authentication.cpp -c

# Remove output files
.PHONY: clean
clean:
	-rm -f *.cgi *.out *.o
