class Game():
    def __init__(self, tuple_in):
        self.game_id = tuple_in[0]

    def __repr__(self):
        return "<Game ({album_id})>".\
                format(album_id = self.album_id)

    def __str__(self):
        return self.__repr__()

    def to_json(self):
        return { "game_id" : self.game_id }

class GameUpdate():
    def __init__(self, tuple_in):
        self.game_id = tuple_in[0]
        self.json_update_blob = tuple_in[1]

    def __repr__(self):
        return "<GameUpdate ({game_id}, {json_update_blob})>".\
                format(game_id = self.game_id, json_update_blob =
                        self.json_update_blob)

    def __str__(self):
        return self.__repr__()

    def to_json(self):
        return { "game_id" : self.game_id, 
                "json_update_blob" : self.json_update_blob }

class CarType():
    def __init__(self, tuple_in):
        self.car_type = tuple_in[0]

    def __repr__(self):
        return "<CarType ({car_type})>".\
                format(car_type = self.car_type)

    def __str__(self):
        return self.__repr__()

    def to_json(self):
        return {"car_type" : self.car_type}

class PlayerAndCar():
    def __init__(self, tuple_in):
        self.player_id = tuple_in[0]
        self.car_type = tuple_in[1]
        self.car_health = tuple_in[2]
        self.kills = tuple_in[3]

    def __repr__(self):
        return "<PlayerAndCar ({player_id}, {car_type}, {car_health}, {kills})>".\
                format(player_id = self.player_id, car_type = self.car_type,
                        car_health = self.car_health, kills = self.kills)

    def __str__(self):
        return self.__repr__()

    def to_json(self):
        return { "player_id" : self.player_id, "car_type" : self.car_type, \
                "car_health" : self.car_health, "kills" : self.kills }
        # return "{{\"player_id\":{player_id},\"car_type\":\"{car_type}\",\"car_health\":{car_health},\"kills\":{kills}}}".format(player_id = self.player_id, car_type = self.car_type,car_health = self.car_health, kills = self.kills)


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
