// Include initialised header
#include "mysql-init.h"

// Use required namespaces
using namespace std;
using namespace mysqlInit;

// Entry function
int main(int argc, char *argv[]) {
    sqlConn->setSchema("coursework");

    sql::Statement *stmt = sqlConn->createStatement();
    sql::ResultSet *res = stmt->executeQuery("SELECT 'Hello World!' AS _message");

    while (res->next()) {
        cout << "\t... MySQL replies: ";

        cout << res->getString("_message") << endl;
        cout << "\t... MySQL says it again: ";

        cout << res->getString(1) << endl;
    }

    return 0;
}
