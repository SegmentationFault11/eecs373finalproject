# The flask api route object
from flask import Flask
from flask.ext.mysql import MySQL
api = Flask(__name__)

# Setup mysql cursor, get cursor and connection every single time
mysql = MySQL()
api.config['MYSQL_DATABASE_USER'] = 'eecs373'
api.config['MYSQL_DATABASE_DB'] = 'eecs373'
api.config['MYSQL_DATABASE_PASSWORD'] = 'password'
api.config['MYSQL_DATABASE_HOST'] = 'localhost'
mysql.init_app(api)
def execute_query(sql):
    """Use this function to get a connection object and then get a cursor and
    then run a query and then return the values"""
    db_cursor = mysql.connect().cursor()
    db_cursor.execute(sql)
    return db_cursor.fetchall()

constant = 9000
