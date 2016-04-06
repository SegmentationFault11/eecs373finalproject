import json
from flask import jsonify, Response
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

@api.route("/game")
@api_jsonify
def game():
    """Get all objects of type game"""
    games = [Game(game).to_json() \
            for game in execute_query("SELECT * FROM Games;")]
    return games

@api.route("/game_update")
@api_jsonify
def game_update():
    """Return all the objects of type GameUpdates"""
    game_updates = [GameUpdate(update).to_json \
            for update in execute_query("SELECT * FROM GameUpdates;")]
    return game_updates

@api.route("/player_and_car")
@api_jsonify
def player_and_car():
    """Return information about the player and the car"""
    player_and_cars = [PlayerAndCar(pandc) \
            for pandc in execute_query("SELECT * FROM PlayerAndCars;")]
    return player_and_cars
