import json
from flask import jsonify, request, render_template, abort
from __init__ import api, mysql, execute_query
from db_models import *
from decorators import api_jsonify
import time
import socket, pickle

def send_data_to_server(data):
    """Sends data to the C++ server which is connected to the SmartFusion"""
    assert len(data) == 14

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(('localhost', 8070))
    data = "14\0" + data
    sock.sendall(data)

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

@api.route("/start_game")
def start_game():

    barray = bytearray()
    barray.append(0)
    barray.append(1)
    barray.append('g')
    for i in range(0, 11):
        barray.append(0)
    send_data_to_server(barray)           

    execute_query("update games set game_stage=3 where game_stage=2;",
            True)
    return jsonify({"status":"ok"})

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
        return post_request_with_class(Game)
    else:
        return delete_request_with_class(Game)

@api.route("/end_game")
def end_game():

    barray = bytearray()
    barray.append(0)
    barray.append(1)
    barray.append('t')
    for i in range(0, 11):
        barray.append(0)
    send_data_to_server(barray)           

    return jsonify({"status":"ok"})

@api.route("/restart_game")
def reset_state():
    execute_query("DELETE FROM Games;", True)
    return jsonify({"status":"ok"})

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
        if 'player_id' in request.json[0] and 'car_type' in request.json[0] \
        and 'car_health' in request.json[0] and 'deaths' in request.json[0] \
        and 'game_id' in request.json[0]:
            car_to_byte = {"tank":2, "fast":0, "strong":1, "trump":3}

            barray = bytearray()
            barray.append(0)
            barray.append(int(request.json[0]['player_id']) + 1)
            barray.append('s')
            barray.append(car_to_byte[request.json[0]["car_type"]])
            for i in range(0, 10):
                barray.append(0)
            send_data_to_server(barray)           

            response = post_request_with_class(PlayerAndCar)
            if request.json[0]['player_id'] == 3:
                print "GAME MOVED ON TO NEXT STAGE ", 2
                execute_query("update games set game_stage=2 where game_stage=1;",
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

    import time
    import datetime

    REQUEST_QUEUE_SIZE = 5
    self_server_address = (address, port_number)
    listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listen_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listen_socket.bind(self_server_address)
    listen_socket.listen(REQUEST_QUEUE_SIZE)
    print "Spawned server on address ", self_server_address

    while True:
        client_connection, client_address = listen_socket.accept()
        request = client_connection.recv(1024)
        print "length of message received ", len(request)
        print request

        # get the current time
        ts = time.time()
        st = datetime.datetime.fromtimestamp(ts)\
                .strftime('%Y-%m-%d %H:%M:%S')
        if "died" in request:
            execute_query("UPDATE PlayerAndCars SET PlayerAndCars.car_health = {health}, PlayerAndCars.deaths = PlayerAndCars.deaths + 1 WHERE PlayerAndCars.player_id = {player_id};"\
                            .format(health = 255, player_id =
                                int(request[7])), True)
        execute_query("INSERT INTO Events VALUES ('{information}','{timestamp}');"\
                .format(information = request, \
                            timestamp = st), True)

        if "Health" in request:
            new_health = int(filter(str.isdigit, request.split("!")[1]))
            execute_query("UPDATE PlayerAndCars SET PlayerAndCars.car_health = {health} WHERE PlayerAndCars.player_id = {player_id};".\
                    format(health = new_health, player_id = int(request[7])), \
                    True)

        if "Killed" in request:
            player_who_killed = int(filter(str.isdigit, 
                request.split("Killed")[1]))
            execute_query("INSERT INTO Events VALUES ('{information}', '{timestamp}');".\
                    format(information = "Gave upgrade to player " + \
                    str(player_who_killed), timestamp = st), True)
            execute_query("UPDATE PlayerAndCars SET PlayerAndCars.upgrades = PlayerAndCars.upgrades + 1 WHERE PlayerAndCars.player_id = {player_id};".\
                    format(player_id = player_who_killed), True)

        client_connection.close()
