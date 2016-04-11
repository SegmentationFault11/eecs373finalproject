class Game():
    def __init__(self, tuple_in):
        self.game_id = tuple_in[0]
        self.game_stage = tuple_in[1]

    def __repr__(self):
        return "<Game {tuple}>".format(tuple = self.to_string_tuple())

    def __str__(self):
        return self.__repr__()

    def to_json(self):
        return { "game_id" : self.game_id , "game_stage" : self.game_stage }
    @staticmethod
    def from_json(json):
        try:
            return Game((json['game_id'], json['game_stage']))
        except KeyError as ke:
            raise KeyError("Game object cannot be constructed from json")
    def to_string_tuple(self):
        return "({game_id}, {game_stage})".format(game_id = self.game_id, \
                game_stage = self.game_stage)

class GameUpdate():
    def __init__(self, tuple_in):
        self.game_id = tuple_in[0]
        self.json_update_blob = tuple_in[1]

    def __repr__(self):
        return "<GameUpdate {tuple}>".format(tuple = self.to_string_tuple())

    def __str__(self):
        return self.__repr__()

    # JSON AND SQL METHODS
    def to_json(self):
        return { "game_id" : self.game_id, 
                "json_update_blob" : self.json_update_blob }
    @staticmethod
    def from_json(json):
        try:
            return GameUpdate((json['game_id'], json['json_update_blob']))
        except KeyError as ke:
            raise KeyError("GameUpdate object cannot be constructed from json")
    def to_string_tuple(self):
        return "({game_id}, \"{json_update_blob}\")".format(game_id = self.game_id, \
                json_update_blob = self.json_update_blob)

class CarType():
    def __init__(self, tuple_in):
        self.car_type = tuple_in[0]

    def __repr__(self):
        return "<CarType {tuple}>".format(tuple = self.to_string_tuple())

    def __str__(self):
        return self.__repr__()

    # JSON AND SQL METHODS
    def to_json(self):
        return {"car_type" : self.car_type}
    @staticmethod
    def from_json(json):
        try:
            return CarType((json['car_type']))
        except KeyError as ke:
            raise KeyError("CarType object cannot be constructed from json")
    def to_string_tuple(self):
        return "(\'{car_type}\')".format(car_type = self.car_type)

class PlayerAndCar():
    def __init__(self, tuple_in):
        self.player_id = tuple_in[0]
        self.car_type = tuple_in[1]
        self.car_health = tuple_in[2]
        self.kills = tuple_in[3]
        self.game_id = tuple_in[4]

    def __repr__(self):
        return "<PlayerAndCar {tuple}>".format(tuple = self.to_string_tuple())

    def __str__(self):
        return self.__repr__()

    # JSON AND SQL METHODS
    def to_json(self):
        return { "player_id" : self.player_id, "car_type" : self.car_type, \
                "car_health" : self.car_health, "kills" : self.kills,
                "game_id" : self.game_id}
    @staticmethod
    def from_json(json):
        try:
            return PlayerAndCar((json['player_id'], json['car_type'],
                    json['car_health'], json['kills'], json['game_id']))
        except KeyError as ke:
            raise KeyError("PlayerAndCar object cannot be constructed from"
                "json")
    def to_string_tuple(self):
        return "({player_id}, \'{car_type}\', {car_health}, {kills}, {game_id})"\
                .format(player_id = self.player_id, car_type = self.car_type,
                        car_health = self.car_health, kills = self.kills,
                        game_id = self.game_id)

class Event():
    def __init__(self, tuple_in):
        self.information = tuple_in[0]
        self.timestamp = tuple_in[1]

    def __repr__(self):
        return "<EventInformation {tuple}>".format(tuple =
                self.to_string_tuple())

    def __str__(self):
        return self.__repr__()

    # JSON AND SQL METHODS
    def to_json(self):
        return {"information" : self.information, "timestamp" :
                self.timestamp}
    @staticmethod
    def from_json(json):
        try:
            return EventInformation(json["information"], json["timestamp"])
        except KeyError as ke:
            raise KeyError("EventInformation cannot be constructed from json")
    def to_string_tuple(self):
        return "({information}, {timestamp})".format(information =
                self.information, timestamp = self.timestamp)


if __name__ == "__main__":

    import json

    pandc = PlayerAndCar((1, "tank", 100, 10))
    print pandc.to_json()
    print (pandc.to_json())

    car_type = CarType(("tank"))
    print (car_type.to_json())

    game_update = GameUpdate((1, "{\"asdasd\":1}"))
    print (game_update.to_json())

    game = Game((1,))
    print (game.to_json())
