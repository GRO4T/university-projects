#!/usr/bin/python3

import generators.generators as generators
import pymysql
import os
from connection import connect

def check_db_empty(conn):
    with conn.cursor() as cursor:
        cursor.execute("""
SELECT count(*)
FROM information_schema.TABLES
WHERE (TABLE_SCHEMA = '{}') AND (TABLE_NAME = '{}')
        """.format(os.environ["DB_DATABASE"], 'pracownik'))
        return cursor.fetchone()[0] == 0

def run_init_scripts(conn):
    with conn.cursor() as cursor:
        files = list(map(lambda x: os.path.join('/app/sql', x), os.listdir('/app/sql')))
        files.sort()
        for file_name in files:
            with open(file_name, "r") as file:
                print("Executing file {}".format(file_name))
                sql = file.read()
                cursor.execute(sql)
    conn.commit()

def run_data_generator(conn):
    with conn.cursor() as cursor:
        objects = generators.generate(1000)
        for obj in objects:
            for table_name, table_values in obj:
                column_names = ','.join(map(lambda x: "%s" % x[0], table_values.items()))
                column_values = ','.join(map(lambda x: "'%s'" % x[1], table_values.items()))
                sql = "INSERT INTO {} ({}) VALUES ({})".format(table_name, column_names, column_values)
                # print(sql)
                cursor.execute(sql)
    conn.commit()


def main():
    conn = connect()

    if check_db_empty(conn):
        run_init_scripts(conn)
        run_data_generator(conn)
    print("Database initialized")





if __name__ == "__main__":
    main()
