#include <iostream>
#include <fstream>
#include <string>
#include <mysql/mysql.h> // esempio per MySQL, si deve includere la libreria corrispondente al database usato

using namespace std;

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    conn = mysql_init(NULL);

    if (conn == NULL) {
        cout << "Errore durante l'inizializzazione della connessione al database." << endl;
        return 1;
    }

    // connessione al database
    if (!mysql_real_connect(conn, "host", "username", "password", "nome_database", 0, NULL, 0)) {
        cout << "Errore durante la connessione al database: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return 1;
    }

    // esecuzione di una query per ottenere i dati da esportare
    if (mysql_query(conn, "SELECT * FROM nome_tabella")) {
        cout << "Errore durante l'esecuzione della query: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return 1;
    }

    res = mysql_use_result(conn);

    // scrittura dei dati su un file di backup
    ofstream backup("backup.sql");
    while ((row = mysql_fetch_row(res)) != NULL) {
        backup << "INSERT INTO nome_tabella (colonna1, colonna2, colonna3) VALUES (" << row[0] << ", " << row[1] << ", " << row[2] << ");" << endl;
    }

    // chiusura della connessione al database e del file di backup
    mysql_free_result(res);
    mysql_close(conn);
    backup.close();

    return 0;
}
