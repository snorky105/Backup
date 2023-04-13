import fdb

# input del nome del database e dello username
db_name = input("Inserisci il nome del database: ")
username = input("Inserisci lo username per la connessione al database: ")

# connessione al database
con = fdb.connect(dsn='localhost:/' + db_name, user=username, password='')

# creazione dell'handle del backup
backup_file = '/path/to/backup.fbk'
backup_service = fdb.services.connect(host='localhost', user=username, password='')
backup_service.backup(backup_file, con)

# chiusura della connessione al database e all'handle del backup
backup_service.close()
con.close()
