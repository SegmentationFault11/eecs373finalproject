import json
from flask import jsonify, request
from __init__ import api, mysql, execute_query
from db_models import *
from decorators import api_jsonify

@api.route("/pictures/<picture_name>")
def send_picture(picture_name):
    """Used to send the requested picture from the pictures folder."""
    return api.send_static_file(picture_name)

@api.route("/car_type")
@api_jsonify
def car_type():
    """Get all objects of type car_type"""
    car_types = [CarType(car_type).to_json() \
            for car_type in execute_query("SELECT * FROM CarTypes;")]
    return car_types


@api.route("/game", methods = ["GET", "POST"])
@api_jsonify
def game():
    """Get all objects of type game"""

    if request.method == "GET":
        games = [Game(game).to_json() \
                for game in execute_query("SELECT * FROM Games;")]
        return games
    else:
        games = [Game().from_json(json_object).to_string_tuple() for \
                    json_object in request.json]
        print games
        return "yes"


@api.route("/game_update", methods = ["GET", "POST"])
@api_jsonify
def game_update():
    """Return all the objects of type GameUpdates"""

    if request.method == "GET":
        game_updates = [GameUpdate(update).to_json \
                for update in execute_query("SELECT * FROM GameUpdates;")]
        return game_updates


@api.route("/player_and_car", methods = ["GET", "POST"])
@api_jsonify
def player_and_car():
    """Return information about the player and the car"""

    if request.method == "GET":
        player_and_cars = [PlayerAndCar(pandc) \
                for pandc in execute_query("SELECT * FROM PlayerAndCars;")]
        return player_and_cars
