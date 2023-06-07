#include <mysql/mysql.h>
#include <stdio.h>

int main() {
    MYSQL *conn;
    conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "Error initializing MySQL: %s\n", mysql_error(conn));
        return 1;
    }

    if (mysql_real_connect(conn, "localhost", "zjc", "998877", NULL, 0, NULL, 0) == NULL) {
        fprintf(stderr, "Error connecting to MySQL: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    if (mysql_query(conn, "CREATE DATABASE IF NOT EXISTS mydb")) {
        fprintf(stderr, "Error creating database: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    if (mysql_query(conn, "USE mydb")) {
        fprintf(stderr, "Error selecting database: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    if (mysql_query(conn, "CREATE TABLE IF NOT EXISTS employees (id INT PRIMARY KEY, name VARCHAR(50), age INT)")) {
        fprintf(stderr, "Error creating table: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    mysql_close(conn);
    printf("Table created successfully!\n");

    return 0;
}

