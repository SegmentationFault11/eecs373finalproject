import json
from flask import jsonify, request, render_template, abort
from __init__ import api, mysql, execute_query
from db_models import *
from decorators import api_jsonify
import time
import socket

def get_request_with_class(classname):
    """This function is used as an handle to handle a get request to all the
    endpoints here in this file."""
    
    # time.sleep(10)
    try:
        # execute_query("SELECT * FROM " + classname.__name__ + "s;")
        objects = [classname(object).to_json() \
                for object in execute_query("SELECT * FROM " + \
                    classname.__name__ + "s;")]
        return objects
    except Exception as exc:
        return jsonify({ "status":str(exc) })

def post_request_with_class(classname):
    """This function is used as an handle to handle a post request to all the
    endpoints here.  This assumes that the request.json object has an array of
    objects that can be used to construct an object of type 'classname'."""

    print "Passed in ", request.json
    sql_values_tuples = ",".join([classname.from_json(json_object).to_string_tuple() \
                for json_object in request.json])
    try:

        # execute the sql query and then return a json response saying that
        # things were ok
        sql = "INSERT INTO " + \
                classname.__name__ + "s VALUES " + sql_values_tuples
        print sql
        execute_query(sql, True)

        return jsonify({ "status":"ok" })

    except Exception as e:

        # return a json response that has the stringified version of the error
        return jsonify({ "status":str(e) })

def delete_request_with_class(classname):
    """This function is used as an handle to handle a delete request to all the
    endpoints here in this file."""

    try:
        execute_query("DELETE FROM " + classname.__name__ + "s;")
        return jsonify({ "status":"ok" })
    except Exception as exc:
        return jsonify({ "status":str(exc) })

@api.route("/pictures/<picture_name>")
def send_picture(picture_name):
    """Used to send the requested picture from the pictures folder."""
    return api.send_static_file(picture_name)

@api.route("/car_type")
@api_jsonify
def car_type():
    """Get all objects of type car_type"""
    return get_request_with_class(CarType)

@api.route("/game", methods = ["GET", "POST", "DELETE"])
@api_jsonify
def game():
    """Get all objects of type game"""
    if request.method == "GET":
        return get_request_with_class(Game)
    elif request.method == "POST":
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect(('localhost', 8070))
        sock.sendall(b'5\0start')

        response =  post_request_with_class(Game)
        return response
    else:
        return delete_request_with_class(Game)


@api.route("/game_update", methods = ["GET", "POST"])
@api_jsonify
def game_update():
    """Return all the objects of type GameUpdate"""
    if request.method == "GET":
        return get_request_with_class(GameUpdate)
    else:
        return post_request_with_class(GameUpdate)

@api.route("/player_and_car", methods = ["GET", "POST"])
@api_jsonify
def player_and_car():
    """Return information about the player and the car"""
    if request.method == "GET":
        return get_request_with_class(PlayerAndCar)
    else:
        response = post_request_with_class(PlayerAndCar)
        if 'player_id' in request.json[0] and 'car_type' in request.json[0] \
        and 'car_health' in request.json[0] and 'kills' in request.json[0] \
        and 'game_id' in request.json[0]:
            if request.json[0]['player_id'] == 3:
                print "GAME MOVED ON TO NEXT STAGE ", 2
                execute_query("UPDATE Games SET game_stage=2 WHERE game_stage=1;",
                        True)

        return response

@api.route("/events")
@api_jsonify
def event_information():
    if request.method == "GET":
        print "here"
        return get_request_with_class(Event)
    else:
        return jsonify({"status":"post requests not allowed on route "
            "/event_information"})

@api.route("/open_api_connection")
def open_api_connection():
    """Returns the address of a new host which will be the endpoint for all
    game specific communication to the client
    """

    # spawn server which is least busy
    (address, port, number_of_connections) = \
        execute_query("SELECT * FROM Servers ORDER BY "\
            "Servers.number_of_connections ASC LIMIT 1")[0]
    # make it busier
    execute_query(("UPDATE Servers SET Servers.number_of_connections = "
            "number_of_connections + 1 WHERE Servers.address='{address}' and "
            "Servers.port='{port}';").format(address = address, port = port), \
            True)

    from threading import Thread
    thread = Thread(target = start_message_server, args = (address, port))
    thread.daemon = True
    thread.start()

    return jsonify({"host":"localhost", "port":port})

def start_message_server(address, port_number):

    REQUEST_QUEUE_SIZE = 5
    self_server_address = (address, port_number)
    listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listen_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listen_socket.bind(self_server_address)
    listen_socket.listen(REQUEST_QUEUE_SIZE)
    print "Spawned server on address ", self_server_address

    while True:
        client_connection, client_address = listen_socket.accept()
        request = client_connection.recv(24)
        timestamp = request[0:9]
        information = request[10:23]
        print "timestamp is ", timestamp
        print "information is ", information
        client_connection.close()
