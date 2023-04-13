#include <iostream>
#include <ibase.h>

using namespace std;

int main() {
    isc_db_handle db = NULL;
    isc_tr_handle tr = NULL;
    isc_backup_handle backup = NULL;

    // input del nome del database e dello username
    char db_name[100];
    char username[50];
    cout << "Inserisci il nome del database: ";
    cin >> db_name;
    cout << "Inserisci lo username per la connessione al database: ";
    cin >> username;

    // connessione al database
    char db_path[200];
    sprintf(db_path, "localhost:/%s", db_name);
    char password[] = "";
    if (isc_attach_database(0, db_path, &db, strlen(username), username, strlen(password), password) != 0) {
        cout << "Errore durante la connessione al database." << endl;
        return 1;
    }

    // inizio della transazione
    if (isc_start_transaction(&tr, 1, &db, 0, NULL) != 0) {
        cout << "Errore durante l'inizio della transazione." << endl;
        isc_detach_database(&db);
        return 1;
    }

    // creazione dell'handle del backup
    char backup_file[] = "/path/to/backup.fbk";
    if (isc_backup_database(&db, &backup, strlen(backup_file), backup_file, 0, NULL, isc_spb_bkp_file_size, 10485760, isc_spb_bkp_factor, 1.0, isc_spb_bkp_verbose, 1, isc_spb_bkp_ignore_checksums, 1, isc_spb_bkp_ignore_limbo, 1, isc_spb_bkp_metadata_only, 0, isc_spb_bkp_old_descriptions, 1, isc_spb_bkp_convert, 0) != 0) {
        cout << "Errore durante la creazione dell'handle del backup." << endl;
        isc_rollback_transaction(&tr);
        isc_detach_database(&db);
        return 1;
    }

    // esecuzione del backup
    if (isc_start_multiple(&tr, &backup, 0, NULL) != 0) {
        cout << "Errore durante l'esecuzione del backup." << endl;
        isc_release_object(&tr, &backup);
        isc_rollback_transaction(&tr);
        isc_detach_database(&db);
        return 1;
    }

    // fine della transazione e chiusura del database
    isc_commit_transaction(&tr);
    isc_detach_database(&db);
    isc_release_object(&tr, &backup);

    return 0;
}
