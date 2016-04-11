import json
from flask import jsonify, request, render_template
from __init__ import api, mysql, execute_query
from db_models import *
from decorators import api_jsonify
import time

def get_request_with_class(classname):
    """This function is used as an handle to handle a get request to all the
    endpoints here in this file."""
    
    time.sleep(10)
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
        return post_request_with_class(Game)
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
    return "hello there!"

# @api.route("/app")
# def serve_webapp():
#     return render_template("webapp.html")
