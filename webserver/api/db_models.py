class Game():
    def __init__(self, tuple_in):
        self.game_id = tuple_in[0]
        self.game_stage = tuple_in[1]
    def __init__(self):
        pass

    def __repr__(self):
        return "<Game ({album_id})>".\
                format(album_id = self.album_id)

    def __str__(self):
        return self.__repr__()

    def to_json(self):
        return { "game_id" : self.game_id , "game_stage" : self.game_stage }
    def from_json(self, json):
        try:
            self.game_id = json['game_id']
            self.game_stage = json['game_stage']
        except KeyError as ke:
            raise KeyError("Game object cannot be constructed from json")
        return self
    def to_string_tuple(self):
        return "({game_id}, {game_stage})".format(game_id = self.game_id, \
                game_stage = self.game_stage)

class GameUpdate():
    def __init__(self, tuple_in):
        self.game_id = tuple_in[0]
        self.json_update_blob = tuple_in[1]
    def __init__(self):
        pass

    def __repr__(self):
        return "<GameUpdate ({game_id}, {json_update_blob})>".\
                format(game_id = self.game_id, json_update_blob =
                        self.json_update_blob)

    def __str__(self):
        return self.__repr__()

    def to_json(self):
        return { "game_id" : self.game_id, 
                "json_update_blob" : self.json_update_blob }
    def from_json(json):
        try:
            self.game_id = json['game_id']
            self.json_update_blob = json['json_update_blob']
        except KeyError as ke:
            raise KeyError("GameUpdate object cannot be constructed from json")
        return self
    def to_string_tuple(self):
        return "({game_id}, \'{json_update_blob}\')".format(game_id = self.game_id, \
                json_update_blob = self.json_update_blob)

class CarType():
    def __init__(self, tuple_in):
        self.car_type = tuple_in[0]
    def __init__(self):
        pass

    def __repr__(self):
        return "<CarType ({car_type})>".\
                format(car_type = self.car_type)

    def __str__(self):
        return self.__repr__()

    def to_json(self):
        return {"car_type" : self.car_type}
    def from_json(json):
        try:
            self.car_type = json['car_type']
        except KeyError as ke:
            raise KeyError("CarType object cannot be constructed from json")
        return self
    def to_string_tuple(self):
        return "(\'{car_type}\')".format(car_type = self.car_type)

class PlayerAndCar():
    def __init__(self, tuple_in):
        self.player_id = tuple_in[0]
        self.car_type = tuple_in[1]
        self.car_health = tuple_in[2]
        self.kills = tuple_in[3]
    def __init__(self):
        pass

    def __repr__(self):
        return "<PlayerAndCar ({player_id}, {car_type}, {car_health}, {kills})>".\
                format(player_id = self.player_id, car_type = self.car_type,
                        car_health = self.car_health, kills = self.kills)

    def __str__(self):
        return self.__repr__()

    def to_json(self):
        return { "player_id" : self.player_id, "car_type" : self.car_type, \
                "car_health" : self.car_health, "kills" : self.kills }
    def from_json(json):
        try:
            self.player_id = json['player_id']
            self.car_type = json['car_type']
            self.car_health = json['car_health']
            self.kills = json['kills']
        except KeyError as ke:
            raise KeyError("PlayerAndCar object cannot be constructed from"
                "json")
        return self
    def to_string_tuple(self):
        return "({player_id}, \'{car_type}\', {car_health}, {kills})"\
                .format(player_id = self.player_id, car_type = self.car_type,
                        car_health = self.car_health, kills = self.kills)


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
