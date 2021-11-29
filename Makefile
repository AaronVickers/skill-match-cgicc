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
APPLICANT_CPP = Pages/applicant.cpp
COMPANY_CPP = Pages/company.cpp
ADMIN_CPP = Pages/admin.cpp
APPROVE_APPLICANTS_CPP = Pages/approveApplicants.cpp
APPROVE_COMPANIES_CPP = Pages/approveCompanies.cpp

LOGIN_FORM_CPP = Components/LoginForm.cpp
LOGIN_FORM_HPP = Components/LoginForm.hpp
REGISTER_FORM_CPP = Components/RegisterForm.cpp
REGISTER_FORM_HPP = Components/RegisterForm.hpp
TFA_FORM_CPP = Components/TFAForm.cpp
TFA_FORM_HPP = Components/TFAForm.hpp
APPROVED_COMPANIES_LIST_CPP = Components/ApprovedCompaniesList.cpp
APPROVED_COMPANIES_LIST_HPP = Components/ApprovedCompaniesList.hpp
APPROVED_APPLICANTS_LIST_CPP = Components/ApprovedApplicantsList.cpp
APPROVED_APPLICANTS_LIST_HPP = Components/ApprovedApplicantsList.hpp
APPROVE_APPLICANTS_FORM_CPP = Components/ApproveApplicantsForm.cpp
APPROVE_APPLICANTS_FORM_HPP = Components/ApproveApplicantsForm.hpp
APPROVE_COMPANIES_FORM_CPP = Components/ApproveCompaniesForm.cpp
APPROVE_COMPANIES_FORM_HPP = Components/ApproveCompaniesForm.hpp
AUTHENTICATION_REDIRECT_CPP = Components/AuthenticationRedirect.cpp
AUTHENTICATION_REDIRECT_HPP = Components/AuthenticationRedirect.hpp

BUILD_DATABASE_CPP = Tools/build-database.cpp

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
TFA_SESSIONS_CPP = Utils/TFASessions.cpp
TFA_SESSIONS_HPP = Utils/TFASessions.hpp
SESSIONS_CPP = Utils/Sessions.cpp
SESSIONS_HPP = Utils/Sessions.hpp
CGI_COMPONENT_CPP = Utils/CGIComponent.cpp
CGI_COMPONENT_HPP = Utils/CGIComponent.hpp
CGI_PAGE_CPP = Utils/CGIPage.cpp
CGI_PAGE_HPP = Utils/CGIPage.hpp

# All output files
all: login.cgi register.cgi tfa.cgi applicant.cgi company.cgi admin.cgi approveApplicants.cgi approveCompanies.cgi build-database.out

# Login CGI file
login.cgi: $(LOGIN_CPP) LoginForm.o AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(LOGIN_CPP) LoginForm.o AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o -o $@ $(LDFLAGS)

# Register CGI file
register.cgi: $(REGISTER_CPP) RegisterForm.o AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(REGISTER_CPP) RegisterForm.o AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o -o $@ $(LDFLAGS)

# 2FA CGI file
tfa.cgi: $(TFA_CPP) TFAForm.o AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(TFA_CPP) TFAForm.o AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o -o $@ $(LDFLAGS)

# Applicant CGI file
applicant.cgi: $(APPLICANT_CPP) ApprovedCompaniesList.o AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(APPLICANT_CPP) ApprovedCompaniesList.o AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o -o $@ $(LDFLAGS)

# Company CGI file
company.cgi: $(COMPANY_CPP) ApprovedApplicantsList.o AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(COMPANY_CPP) ApprovedApplicantsList.o AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o -o $@ $(LDFLAGS)

# Admin CGI file
admin.cgi: $(ADMIN_CPP) AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(ADMIN_CPP) AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o -o $@ $(LDFLAGS)

# Approve applicants CGI file
approveApplicants.cgi: $(APPROVE_APPLICANTS_CPP) ApproveApplicantsForm.o AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(APPROVE_APPLICANTS_CPP) ApproveApplicantsForm.o AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o -o $@ $(LDFLAGS)

# Approve companies CGI file
approveCompanies.cgi: $(APPROVE_COMPANIES_CPP) ApproveCompaniesForm.o AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(APPROVE_COMPANIES_CPP) ApproveCompaniesForm.o AuthenticationRedirect.o MariaDBInit.o Authentication.o Result.o Roles.o Skills.o Users.o TFASessions.o Sessions.o CGIComponent.o CGIPage.o -o $@ $(LDFLAGS)

# Database build file
build-database.out: $(BUILD_DATABASE_CPP) Result.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(BUILD_DATABASE_CPP) Result.o -o $@ $(LIB_MARIADB)

# Login form object file
LoginForm.o: $(LOGIN_FORM_CPP) $(LOGIN_FORM_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(LOGIN_FORM_CPP) -c -o $@

# Register form object file
RegisterForm.o: $(REGISTER_FORM_CPP) $(REGISTER_FORM_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(REGISTER_FORM_CPP) -c -o $@

# 2FA form object file
TFAForm.o: $(TFA_FORM_CPP) $(TFA_FORM_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(TFA_FORM_CPP) -c -o $@

# Approved companies list object file
ApprovedCompaniesList.o: $(APPROVED_COMPANIES_LIST_CPP) $(APPROVED_COMPANIES_LIST_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(APPROVED_COMPANIES_LIST_CPP) -c -o $@

# Approved applicants list object file
ApprovedApplicantsList.o: $(APPROVED_APPLICANTS_LIST_CPP) $(APPROVED_APPLICANTS_LIST_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(APPROVED_APPLICANTS_LIST_CPP) -c -o $@

# Approve applicants form object file
ApproveApplicantsForm.o: $(APPROVE_APPLICANTS_FORM_CPP) $(APPROVE_APPLICANTS_FORM_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(APPROVE_APPLICANTS_FORM_CPP) -c -o $@

# Approve companies form object file
ApproveCompaniesForm.o: $(APPROVE_COMPANIES_FORM_CPP) $(APPROVE_COMPANIES_FORM_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(APPROVE_COMPANIES_FORM_CPP) -c -o $@

# Authentication redirect object file
AuthenticationRedirect.o: $(AUTHENTICATION_REDIRECT_CPP) $(AUTHENTICATION_REDIRECT_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(AUTHENTICATION_REDIRECT_CPP) -c -o $@

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

# TFASessions object file
TFASessions.o: $(TFA_SESSIONS_CPP) $(TFA_SESSIONS_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(TFA_SESSIONS_CPP) -c -o $@

# Sessions object file
Sessions.o: $(SESSIONS_CPP) $(SESSIONS_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(SESSIONS_CPP) -c -o $@

# CGI component object file
CGIComponent.o: $(CGI_COMPONENT_CPP) $(CGI_COMPONENT_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(CGI_COMPONENT_CPP) -c -o $@

# CGI page object file
CGIPage.o: $(CGI_PAGE_CPP) $(CGI_PAGE_HPP)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $(CGI_PAGE_CPP) -c -o $@

# Remove output files
.PHONY: clean
clean:
	-rm -f *.cgi *.out *.o
