import json
from flask import jsonify, request
from __init__ import api, mysql, execute_query
from db_models import *
from decorators import api_jsonify

def get_request_with_classname(classname):
    """This function is used as an handle to handle a get request to all the
    endpoints here in this file."""

    execute_query("SELECT * FROM " + classname.__name__ + "s;")
    objects = [classname(object).to_json() \
            for object in execute_query("SELECT * FROM " + \
                classname.__name__ + "s;")]
    return objects

def post_request_with_class(classname):
    """This function is used as an handle to handle a post request to all the
    endpoints here.  This assumes that the request.json object has an array of
    objects that can be used to construct an object of type 'classname'."""
    sql_values_tuples = ",".join([classname.from_json(json_object).to_string_tuple() \
                for json_object in request.json])
    try:

        # execute the sql query and then return a json response saying that
        # things were ok
        execute_query(("INSERT INTO " + \
                classname.__name__ + "s VALUES " + sql_values_tuples), 
                needs_commit = True)

        return jsonify({ "status":"ok" })

    except Exception as e:

        # return a json response that has the stringified version of the error
        return jsonify({ "status":str(e) })


@api.route("/pictures/<picture_name>")
def send_picture(picture_name):
    """Used to send the requested picture from the pictures folder."""
    return api.send_static_file(picture_name)

@api.route("/car_type")
@api_jsonify
def car_type():
    """Get all objects of type car_type"""
    return get_request_with_classname(CarType)

@api.route("/game", methods = ["GET", "POST"])
@api_jsonify
def game():
    """Get all objects of type game"""
    if request.method == "GET":
        return get_request_with_classname(Game)
    else:
        return post_request_with_class(Game)


@api.route("/game_update", methods = ["GET", "POST"])
@api_jsonify
def game_update():
    """Return all the objects of type GameUpdates"""
    if request.method == "GET":
        return get_request_with_classname(GameUpdate)

@api.route("/player_and_car", methods = ["GET", "POST"])
@api_jsonify
def player_and_car():
    """Return information about the player and the car"""
    if request.method == "GET":
        return get_request_with_classname(PlayerAndCar)
