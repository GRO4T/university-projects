import pymysql
import time
import os

def open_connection():
    username = os.environ["DB_USERNAME"]
    password = os.environ["DB_ROOT_PASSWORD"]
    host = "db"
    database = os.environ["DB_DATABASE"]
    #return pymysql.connect(host, 'root', password, database, client_flag=pymysql.constants.CLIENT.MULTI_STATEMENTS)
    return pymysql.connect(host=host, user='root', password=password, database=database, client_flag=pymysql.constants.CLIENT.MULTI_STATEMENTS)

def try_connect():
    try:
        return open_connection()
    except Exception as e:
        print(e)
        return None

def connect():
    conn = try_connect()
    while conn is None:
        print("Retrying connection")
        conn = try_connect()
        time.sleep(1)
    return conn

TEST_CONN = connect()
