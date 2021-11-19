# Library flags
LIB_CGICC = -lcgicc
LIB_MYSQL = -lmysqlcppconn

# Compiler options
CXX = g++
CXXFLAGS = -std=c++14
LDFLAGS = $(LIB_CGICC) $(LIB_MYSQL)
INC_DIRS = -I.

# File locations
LOGIN_CPP = Pages/login.cpp
REGISTER_CPP = Pages/register.cpp
TFA_CPP = Pages/tfa.cpp
POPULATE_DATABASE_CPP = Tools/populate-database.cpp
CGICCINIT_CPP = Utils/CgiccInit.cpp
CGICCINIT_HPP = Utils/CgiccInit.hpp
MYSQLINIT_CPP = Utils/MySqlInit.cpp
MYSQLINIT_HPP = Utils/MySqlInit.hpp
AUTHENTICATION_CPP = Utils/Authentication.cpp
AUTHENTICATION_HPP = Utils/Authentication.hpp

# All output files
all: login.cgi register.cgi tfa.cgi database.out

# Login CGI file
login.cgi: $(LOGIN_CPP) CgiccInit.o MySqlInit.o Authentication.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(LOGIN_CPP) CgiccInit.o MySqlInit.o Authentication.o -o $@ $(LDFLAGS)

# Register CGI file
register.cgi: $(REGISTER_CPP) CgiccInit.o MySqlInit.o Authentication.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(REGISTER_CPP) CgiccInit.o MySqlInit.o Authentication.o -o $@ $(LDFLAGS)

# 2FA CGI file
tfa.cgi: $(TFA_CPP) CgiccInit.o MySqlInit.o Authentication.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(TFA_CPP) CgiccInit.o MySqlInit.o Authentication.o -o $@ $(LDFLAGS)

# Database population file
populate-database.out: $(POPULATE_DATABASE_CPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(POPULATE_DATABASE_CPP) -o $@ $(LIB_MYSQL)

# CgiccInit object file
CgiccInit.o: $(CGICCINIT_CPP) $(CGICCINIT_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(CGICCINIT_CPP) -c -o $@

# MySqlInit object file
MySqlInit.o: $(MYSQLINIT_CPP) $(MYSQLINIT_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(MYSQLINIT_CPP) -c -o $@

# Authentication object file
Authentication.o: $(AUTHENTICATION_CPP) $(AUTHENTICATION_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(AUTHENTICATION_CPP) -c -o $@

# Remove output files
.PHONY: clean
clean:
	-rm -f *.cgi *.out *.o
