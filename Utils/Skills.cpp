// Include header file
#include "Utils/Skills.hpp"

// Initialised MariaDB header
#include "Utils/MariaDBInit.hpp"

// MariaDB header
#include <mariadb/conncpp.hpp>

// Required headers
#include <string>

int Skill::getSkillId() {
    // Return skill ID
    return skillId;
}

std::string Skill::getName() {
    // Return name
    return name;
}

Skill::Skill(int _skillId) {
    // Set skill ID
    skillId = _skillId;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variable
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare skill select statement
    pstmt = db.conn->prepareStatement("SELECT * FROM Skills WHERE SkillId=?");

    // Execute query
    pstmt->setInt(1, skillId);
    res = pstmt->executeQuery();

    // Delete statement from memory
    delete pstmt;

    // Get first row
    res->next();

    // Get skill name from row
    name = res->getString("Name").c_str();

    // Delete result from memory
    delete res;
}

Skill::Skill(std::string _name) {
    // Set name
    name = _name;

    // Check for existing skill
    SkillResult existingSkillResult = Skills::getSkillByName(_name);
    if (existingSkillResult.getSuccess()) {
        // Set skill ID from existing skill
        skillId = existingSkillResult.skill->getSkillId();

        return;
    }

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variables
    sql::Statement *stmt = db.conn->createStatement();
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare skill search insert
    pstmt = db.conn->prepareStatement("INSERT INTO Skills (Name) VALUES (?)");

    // Insert values into statement
    pstmt->setString(1, name);

    // Execute query
    pstmt->execute();

    // Delete statement from memory
    delete pstmt;

    // Get primary key of new row
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");

    // Get first row
    res->next();

    // Get skill search ID from row
    skillId = res->getInt(1);

    // Delete result from memory
    delete res;
}

Skill::Skill(int _skillId, std::string _name) {
    // Initialise all attributes with passed parameters
    skillId = _skillId;
    name = _name;
}

int SkillSearch::getSkillSearchId() {
    // Return skill search ID
    return skillSearchId;
}

int SkillSearch::getSkillId() {
    // Return skill ID
    return skillId;
}

Skill SkillSearch::getSkill() {
    // Get skill
    Skill skill = Skill(skillId);

    // Return skill
    return skill;
}

int SkillSearch::getUserId() {
    // Return user ID
    return userId;
}

User SkillSearch::getUser() {
    // Get user
    User user = User(userId);

    // Return skill
    return user;
}

bool SkillSearch::getApprovedState() {
    // Return approved state
    return approvedState;
}

void SkillSearch::setApprovedState(bool newApprovedState) {
    // Update approved state
    approvedState = newApprovedState;

    // TODO: Update approved state in database
}

SkillSearch::SkillSearch(int _skillSearchId) {
    // Set skill search ID
    skillSearchId = _skillSearchId;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variable
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare skill search select statement
    pstmt = db.conn->prepareStatement("SELECT * FROM Skills WHERE SkillId=?");

    // Execute query
    pstmt->setInt(1, skillId);
    res = pstmt->executeQuery();

    // Delete statement from memory
    delete pstmt;

    // Get first row
    res->next();

    // Get skill search details from row
    skillId = res->getInt("SkillId");
    userId = res->getInt("UserId");
    approvedState = res->getBoolean("ApprovedState");

    // Delete result from memory
    delete res;
}

SkillSearch::SkillSearch(Skill &skill, User &user) {
    // Initialise attributes with passed parameters
    skillId = skill.getSkillId();
    userId = user.getUserId();
    approvedState = false;

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // SQL statement variables
    sql::Statement *stmt = db.conn->createStatement();
    sql::PreparedStatement *pstmt;
    // SQL result variable
    sql::ResultSet *res;

    // Prepare skill search insert
    pstmt = db.conn->prepareStatement("INSERT INTO SkillSearches (SkillId, UserId, ApprovedState) VALUES (?,?,?)");

    // Insert values into statement
    pstmt->setInt(1, skillId);
    pstmt->setInt(2, userId);
    pstmt->setBoolean(3, approvedState);

    // Execute query
    pstmt->execute();

    // Delete statement from memory
    delete pstmt;

    // Get primary key of new row
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");

    // Get first row
    res->next();

    // Get skill search ID from row
    skillSearchId = res->getInt(1);

    // Delete result from memory
    delete res;
}

SkillSearch::SkillSearch(int _skillSearchId, int _skillId, int _userId, bool _approvedState) {
    // Initialise all attributes with passed parameters
    skillSearchId = _skillSearchId;
    skillId = _skillId;
    userId = _userId;
    approvedState = _approvedState;
}

SkillResult Skills::getSkillByName(std::string name) {
    // Create result
    SkillResult skillResult = SkillResult();

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // Handle connection error
    if (!db.getSuccess()) {
        skillResult.setError(db.getErrorMsg());

        // Return result
        return skillResult;
    }

    // Attempt to complete operation
    try {
        // SQL statement variable
        sql::PreparedStatement *pstmt;
        // SQL result variable
        sql::ResultSet *res;

        // Prepare skill select statement
        pstmt = db.conn->prepareStatement("SELECT * FROM Skills WHERE Name=?");

        // Execute query
        pstmt->setString(1, name);
        res = pstmt->executeQuery();

        // Delete statement from memory
        delete pstmt;

        // Error if no rows selected
        if (!res->next()) {
            // Delete result from memory
            delete res;

            skillResult.setError("Skill with provided name doesn't exist.");

            return skillResult;
        }

        // Get skill details from first row
        int skillId = res->getInt("SkillId");
        std::string name = res->getString("Name").c_str();

        // Store skill in result
        skillResult.skill = new Skill(skillId, name);

        // Delete result from memory
        delete res;
    } catch (sql::SQLException &sql_error) {
        // Handle SQL exception
        skillResult.setError(sql_error.what());

        // Return result
        return skillResult;
    }

    // Set result success
    skillResult.setSuccess(true);

    // Return result
    return skillResult;
}

UsersResult Skills::getApprovedApplicantsBySkill(std::string skillName) {
    // Create result
    UsersResult applicantsResult = UsersResult();

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // Handle connection error
    if (!db.getSuccess()) {
        applicantsResult.setError(db.getErrorMsg());

        // Return result
        return applicantsResult;
    }

    // Attempt to complete operation
    try {
        // SQL statement variable
        sql::PreparedStatement *pstmt;
        // SQL result variable
        sql::ResultSet *res;

        // Prepare skill search select statement
        pstmt = db.conn->prepareStatement(" \
            SELECT * FROM SkillSearches \
            INNER JOIN Users ON Users.UserId=SkillSearches.UserId \
            INNER JOIN Roles ON Users.RoleId=Roles.RoleId \
            INNER JOIN Skills ON SkillSearches.SkillId=Skills.SkillId \
            WHERE ApprovedState=? AND Roles.Name=? AND Skills.Name=? \
        ");

        // Execute query
        pstmt->setBoolean(1, true);
        pstmt->setString(2, "Applicant");
        pstmt->setString(3, skillName);
        res = pstmt->executeQuery();

        // Delete statement from memory
        delete pstmt;

        // Loop over all results
        while (res->next()) {
            // Get user details from current row
            int userId = res->getInt("UserId");
            std::string username = res->getString("Username").c_str();
            std::string email = res->getString("Email").c_str();
            std::string passwordHashEncoded = res->getString("PasswordHashEncoded").c_str();
            int roleId = res->getInt("RoleId");
            bool locked = res->getBoolean("Locked");

            // Create instance of user
            User user = User(userId, username, email, passwordHashEncoded, roleId, locked);

            // Add user to result
            applicantsResult.users.emplace_back(user);
        }

        // Delete result from memory
        delete res;
    } catch (sql::SQLException &sql_error) {
        // Handle SQL exception
        applicantsResult.setError(sql_error.what());

        // Return result
        return applicantsResult;
    }

    // Set result success
    applicantsResult.setSuccess(true);

    // Return result
    return applicantsResult;
}

UsersResult Skills::getApprovedCompaniesBySkill(std::string skillName) {
    // Create result
    UsersResult companiesResult = UsersResult();

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // Handle connection error
    if (!db.getSuccess()) {
        companiesResult.setError(db.getErrorMsg());

        // Return result
        return companiesResult;
    }

    // Attempt to complete operation
    try {
        // SQL statement variable
        sql::PreparedStatement *pstmt;
        // SQL result variable
        sql::ResultSet *res;

        // Prepare skill search select statement
        pstmt = db.conn->prepareStatement(" \
            SELECT * FROM SkillSearches \
            INNER JOIN Users ON Users.UserId=SkillSearches.UserId \
            INNER JOIN Roles ON Users.RoleId=Roles.RoleId \
            INNER JOIN Skills ON SkillSearches.SkillId=Skills.SkillId \
            WHERE ApprovedState=? AND Roles.Name=? AND Skills.Name=? \
        ");

        // Execute query
        pstmt->setBoolean(1, true);
        pstmt->setString(2, "Company");
        pstmt->setString(3, skillName);
        res = pstmt->executeQuery();

        // Delete statement from memory
        delete pstmt;

        // Loop over all results
        while (res->next()) {
            // Get user details from current row
            int userId = res->getInt("UserId");
            std::string username = res->getString("Username").c_str();
            std::string email = res->getString("Email").c_str();
            std::string passwordHashEncoded = res->getString("PasswordHashEncoded").c_str();
            int roleId = res->getInt("RoleId");
            bool locked = res->getBoolean("Locked");

            // Create instance of user
            User user = User(userId, username, email, passwordHashEncoded, roleId, locked);

            // Add user to result
            companiesResult.users.emplace_back(user);
        }

        // Delete result from memory
        delete res;
    } catch (sql::SQLException &sql_error) {
        // Handle SQL exception
        companiesResult.setError(sql_error.what());

        // Return result
        return companiesResult;
    }

    // Set result success
    companiesResult.setSuccess(true);

    // Return result
    return companiesResult;
}

SkillSearchesResult Skills::getUnapprovedApplicantSkillSearches() {
    // Create result
    SkillSearchesResult skillSearchesResult = SkillSearchesResult();

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // Handle connection error
    if (!db.getSuccess()) {
        skillSearchesResult.setError(db.getErrorMsg());

        // Return result
        return skillSearchesResult;
    }

    // Attempt to complete operation
    try {
        // SQL statement variable
        sql::PreparedStatement *pstmt;
        // SQL result variable
        sql::ResultSet *res;

        // Prepare skill search select statement
        pstmt = db.conn->prepareStatement(" \
            SELECT * FROM SkillSearches \
            INNER JOIN Users ON Users.UserId=SkillSearches.UserId \
            INNER JOIN Roles ON Users.RoleId=Roles.RoleId \
            WHERE ApprovedState=? AND Roles.Name=? \
        ");

        // Execute query
        pstmt->setBoolean(1, false);
        pstmt->setString(2, "Applicant");
        res = pstmt->executeQuery();

        // Delete statement from memory
        delete pstmt;

        // Loop over all results
        while (res->next()) {
            // Get skill search details from current row
            int skillSearchId = res->getInt("SkillSearchId");
            int skillId = res->getInt("SkillId");
            int userId = res->getInt("UserId");
            bool approvedState = res->getBoolean("ApprovedState");

            // Create instance of skill search
            SkillSearch skillSearch = SkillSearch(skillSearchId, skillId, userId, approvedState);

            // Add skill search to result
            skillSearchesResult.skillSearches.emplace_back(skillSearch);
        }

        // Delete result from memory
        delete res;
    } catch (sql::SQLException &sql_error) {
        // Handle SQL exception
        skillSearchesResult.setError(sql_error.what());

        // Return result
        return skillSearchesResult;
    }

    // Set result success
    skillSearchesResult.setSuccess(true);

    // Return result
    return skillSearchesResult;
}

SkillSearchesResult Skills::getUnapprovedCompanySkillSearches() {
    // Create result
    SkillSearchesResult skillSearchesResult = SkillSearchesResult();

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // Handle connection error
    if (!db.getSuccess()) {
        skillSearchesResult.setError(db.getErrorMsg());

        // Return result
        return skillSearchesResult;
    }

    // Attempt to complete operation
    try {
        // SQL statement variable
        sql::PreparedStatement *pstmt;
        // SQL result variable
        sql::ResultSet *res;

        // Prepare skill search select statement
        pstmt = db.conn->prepareStatement(" \
            SELECT * FROM SkillSearches \
            INNER JOIN Users ON Users.UserId=SkillSearches.UserId \
            INNER JOIN Roles ON Users.RoleId=Roles.RoleId \
            WHERE ApprovedState=? AND Roles.Name=? \
        ");

        // Execute query
        pstmt->setBoolean(1, false);
        pstmt->setString(2, "Company");
        res = pstmt->executeQuery();

        // Delete statement from memory
        delete pstmt;

        // Loop over all results
        while (res->next()) {
            // Get skill search details from current row
            int skillSearchId = res->getInt("SkillSearchId");
            int skillId = res->getInt("SkillId");
            int userId = res->getInt("UserId");
            bool approvedState = res->getBoolean("ApprovedState");

            // Create instance of skill search
            SkillSearch skillSearch = SkillSearch(skillSearchId, skillId, userId, approvedState);

            // Add skill search to result
            skillSearchesResult.skillSearches.emplace_back(skillSearch);
        }

        // Delete result from memory
        delete res;
    } catch (sql::SQLException &sql_error) {
        // Handle SQL exception
        skillSearchesResult.setError(sql_error.what());

        // Return result
        return skillSearchesResult;
    }

    // Set result success
    skillSearchesResult.setSuccess(true);

    // Return result
    return skillSearchesResult;
}

SkillSearchResult Skills::getUserSkillSearch(User &user) {
    // Create result
    SkillSearchResult userSkillSearchResult = SkillSearchResult();

    // Initialise MariaDB connection
    MariaDBInit db = MariaDBInit();

    // Handle connection error
    if (!db.getSuccess()) {
        userSkillSearchResult.setError(db.getErrorMsg());

        // Return result
        return userSkillSearchResult;
    }

    // Attempt to complete operation
    try {
        // SQL statement variable
        sql::PreparedStatement *pstmt;
        // SQL result variable
        sql::ResultSet *res;

        // Prepare skill search select statement
        pstmt = db.conn->prepareStatement("SELECT * FROM SkillSearches WHERE UserId=?");

        // Execute query
        pstmt->setInt(1, user.getUserId());
        res = pstmt->executeQuery();

        // Delete statement from memory
        delete pstmt;

        // Error if no rows selected
        if (!res->next()) {
            // Delete result from memory
            delete res;

            userSkillSearchResult.setError("Could not find user's skill search.");

            return userSkillSearchResult;
        }

        // Get skill search details from first row
        int skillSearchId = res->getInt("SkillSearchId");
        int skillId = res->getInt("SkillId");
        int userId = res->getInt("UserId");
        bool approvedState = res->getBoolean("ApprovedState");

        // Store skill search in result
        userSkillSearchResult.skillSearch = new SkillSearch(skillSearchId, skillId, userId, approvedState);

        // Delete result from memory
        delete res;
    } catch (sql::SQLException &sql_error) {
        // Handle SQL exception
        userSkillSearchResult.setError(sql_error.what());

        // Return result
        return userSkillSearchResult;
    }

    // Set result success
    userSkillSearchResult.setSuccess(true);

    // Return result
    return userSkillSearchResult;
}
