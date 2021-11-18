# Compiler options
CXX = g++
CXXFLAGS = -std=c++14
LDFLAGS = -lcgicc -lmysqlcppconn

# All output files
all: login.cgi register.cgi database.out

# Login CGI file
login.cgi: login.cpp CgiccInit.cpp MySqlInit.cpp
	$(CXX) $(CXXFLAGS) login.cpp CgiccInit.cpp MySqlInit.cpp -o login.cgi $(LDFLAGS)

# Register CGI file
register.cgi: register.cpp CgiccInit.cpp MySqlInit.cpp
	$(CXX) $(CXXFLAGS) register.cpp CgiccInit.cpp MySqlInit.cpp -o register.cgi $(LDFLAGS)

# Database population file
database.out: database.cpp
	$(CXX) $(CXXFLAGS) database.cpp -o database.out -lmysqlcppconn

# Remove output files
.PHONY: clean
clean:
	-rm -f *.cgi *.out
