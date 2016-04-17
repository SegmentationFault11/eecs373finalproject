"""A test for the endpoints given in the API"""
import requests, json, os

if __name__ == "__main__":
    
    from subprocess import call
    os.chdir("sql")
    call(["./drop_create_load.sh"])

    assert requests.post("http://localhost:8000/game", headers =
            {"content-type":"application/json"}, data = json.dumps([{"game_id" : 1,
                "game_stage" : 1}])).json()["status"] == "ok"
    assert requests.get("http://localhost:8000/game").json()[0]["game_id"] \
        == 1
    
    assert requests.post("http://localhost:8000/player_and_car", headers =
            {"content-type":"application/json"}, data = json.dumps([{"game_id"
                : 1, "player_id" : 1, "car_type":"fast", "car_health":10,
                "deaths":2}])).json()["status"] == "ok"
    assert requests.get("http://localhost:8000/player_and_car").json()\
            [0]["player_id"] == 1

    assert len(requests.get("http://localhost:8000/car_type").json()) == 4

    assert requests.post("http://localhost:8000/game_update", headers =
            {"content-type":"application/json"}, 
            data = json.dumps([{"game_id":1,
                "json_update_blob":{"hello":"there"}}])).json()["status"] \
                        == "ok"

    call(["./drop_create_load.sh"])
