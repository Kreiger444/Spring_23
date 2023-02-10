#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

PGconn *conn;

void executeQuery1() {
  PGresult *res;

  res = PQexec(conn, "SELECT * FROM table1");
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    printf("Query 1 failed: %s\n", PQerrorMessage(conn));
    PQclear(res);
    return;
  }

  int nfields = PQnfields(res);
  int ntuples = PQntuples(res);

  printf("Query 1 results:\n");
  printf("----------------\n");
  printf("Number of fields: %d\n", nfields);
  printf("Number of tuples: %d\n", ntuples);

  for (int i = 0; i < ntuples; i++) {
    for (int j = 0; j < nfields; j++) {
      printf("%s\t", PQgetvalue(res, i, j));
    }
    printf("\n");
  }

  PQclear(res);
}

void executeQuery2() {
  PGresult *res;

  res = PQexec(conn, "SELECT * FROM table2");
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    printf("Query 2 failed: %s\n", PQerrorMessage(conn));
    PQclear(res);
    return;
  }

  int nfields = PQnfields(res);
  int ntuples = PQntuples(res);

  printf("Query 2 results:\n");
  printf("----------------\n");
  printf("Number of fields: %d\n", nfields);
  printf("Number of tuples: %d\n", ntuples);

  for (int i = 0; i < ntuples; i++) {
    for (int j = 0; j < nfields; j++) {
      printf("%s\t", PQgetvalue(res, i, j));
    }
    printf("\n");
  }

  PQclear(res);
}

int main() {
  int choice;

  conn = PQconnectdb("host=hostname port=5432 dbname=database_name user=username password=password");
  if (PQstatus(conn) != CONNECTION_OK) {
    fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
    exit(1);
  }

  printf("Connected to the database\n");
  printf("========================\n");
  printf("Choose one of the following options:\n");
  printf("1. Query 1\n");
  printf("2. Query 2\n");
  printf("3. Exit\n");
  printf("Enter your choice: ");
  scanf("%d", &choice);

  while (choice != 3) {
    switch (choice) {
      case 1:
        executeQuery1();
        break;
      case 2:
        executeQuery2();
        break;
      default:
        printf("Invalid choice. Try again\n");
        break;
    }

    printf("\n\n");
    printf("Choose one of the following options:\n");
    printf("1. Query 1\n");
    printf("2. Query 2\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
  }

  PQfinish(conn);

  return 0;
}