// Initialised header
#include "MySqlInit.h"

// MySQL headers
#include <cppconn/statement.h>
#include <cppconn/resultset.h>

// Use required namespaces
using namespace std;

// Entry function
int main(int argc, char *argv[]) {
    // Initialise MySQL connection
    MySqlInit *db = new MySqlInit();

    sql::Statement *stmt = db->conn->createStatement();
    sql::ResultSet *res = stmt->executeQuery("SELECT 'Hello World!' AS _message");

    while (res->next()) {
        cout << "\t... MySQL replies: ";

        cout << res->getString("_message") << endl;
        cout << "\t... MySQL says it again: ";

        cout << res->getString(1) << endl;
    }

    return 0;
}
