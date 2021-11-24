# Library flags
LIB_CGICC = -lcgicc
LIB_MARIADB = -lmariadbcpp
LIB_ARGON2 = -largon2

# Compiler options
CXX = g++
CXXFLAGS = -std=c++14
LDFLAGS = $(LIB_CGICC) $(LIB_MARIADB) $(LIB_ARGON2)
INC_DIRS = -I.

# File locations
LOGIN_CPP = Pages/login.cpp
REGISTER_CPP = Pages/register.cpp
TFA_CPP = Pages/tfa.cpp

BUILD_DATABASE_CPP = Tools/build-database.cpp

CGICCINIT_CPP = Utils/CgiccInit.cpp
CGICCINIT_HPP = Utils/CgiccInit.hpp
MARIADBINIT_CPP = Utils/MariaDBInit.cpp
MARIADBINIT_HPP = Utils/MariaDBInit.hpp

AUTHENTICATION_CPP = Utils/Authentication.cpp
AUTHENTICATION_HPP = Utils/Authentication.hpp
RESULT_CPP = Utils/Result.cpp
RESULT_HPP = Utils/Result.hpp
ROLES_CPP = Utils/Roles.cpp
ROLES_HPP = Utils/Roles.hpp
SKILLS_CPP = Utils/Skills.cpp
SKILLS_HPP = Utils/Skills.hpp
USERS_CPP = Utils/Users.cpp
USERS_HPP = Utils/Users.hpp

# All output files
all: login.cgi register.cgi tfa.cgi build-database.out

# Login CGI file
login.cgi: $(LOGIN_CPP) CgiccInit.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(LOGIN_CPP) CgiccInit.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o -o $@ $(LDFLAGS)

# Register CGI file
register.cgi: $(REGISTER_CPP) CgiccInit.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(REGISTER_CPP) CgiccInit.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o -o $@ $(LDFLAGS)

# 2FA CGI file
tfa.cgi: $(TFA_CPP) CgiccInit.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(TFA_CPP) CgiccInit.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o -o $@ $(LDFLAGS)

# Database build file
build-database.out: $(BUILD_DATABASE_CPP) Result.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(BUILD_DATABASE_CPP) Result.o -o $@ $(LIB_MARIADB)

# CgiccInit object file
CgiccInit.o: $(CGICCINIT_CPP) $(CGICCINIT_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(CGICCINIT_CPP) -c -o $@

# MariaDBInit object file
MariaDBInit.o: $(MARIADBINIT_CPP) $(MARIADBINIT_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(MARIADBINIT_CPP) -c -o $@

# Authentication object file
Authentication.o: $(AUTHENTICATION_CPP) $(AUTHENTICATION_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(AUTHENTICATION_CPP) -c -o $@

# Result object file
Result.o: $(RESULT_CPP) $(RESULT_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(RESULT_CPP) -c -o $@

# Roles object file
Roles.o: $(ROLES_CPP) $(ROLES_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(ROLES_CPP) -c -o $@

# Skills object file
Skills.o: $(SKILLS_CPP) $(SKILLS_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(SKILLS_CPP) -c -o $@

# Users object file
Users.o: $(USERS_CPP) $(USERS_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(USERS_CPP) -c -o $@

# Remove output files
.PHONY: clean
clean:
	-rm -f *.cgi *.out *.o
