# The flask api route object
from flask import Flask, Blueprint
from flask.ext.mysql import MySQL
api = Flask(__name__) 

@api.after_request
def apply_all_origin_headers(response):
    response.headers['Access-Control-Allow-Origin'] = '*'
    return response

# Setup mysql cursor, get cursor and connection every single time
mysql = MySQL()
api.config['MYSQL_DATABASE_USER'] = 'eecs373'
api.config['MYSQL_DATABASE_DB'] = 'eecs373'
api.config['MYSQL_DATABASE_PASSWORD'] = 'password'
api.config['MYSQL_DATABASE_HOST'] = 'localhost'
mysql.init_app(api)
def execute_query(sql, needs_commit = False):
    """Use this function to get a connection object and then get a cursor and
    then run a query and then return the values"""
    db_connection = mysql.connect()
    db_cursor = db_connection.cursor()
    db_cursor.execute(sql)

    # commit and return if the needs_commit flag is set
    if needs_commit:
        db_connection.commit()
        return

    # else return the results
    return db_cursor.fetchall()

