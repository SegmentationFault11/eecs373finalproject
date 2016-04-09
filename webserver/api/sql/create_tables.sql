CREATE TABLE Games 
(
    game_id INT NOT NULL AUTO_INCREMENT,
    game_stage INT CHECK(game_stage < 3),

    PRIMARY KEY(game_id)
);

CREATE TABLE GameUpdates 
(
    game_id INT,
    json_update_blob VARCHAR(2048),

    FOREIGN KEY(game_id) REFERENCES Games(game_id) ON DELETE CASCADE
);

CREATE TABLE CarTypes 
(
    car_type CHAR(30) UNIQUE
);

CREATE TABLE PlayerAndCars 
(
    player_id INT,
    car_type CHAR(30),
    car_health INT,
    kills INT,
    game_id INT,

    FOREIGN KEY(car_type) REFERENCES CarTypes(car_type) ON DELETE CASCADE,
    FOREIGN KEY(game_id) REFERENCES Games(game_id) ON DELETE CASCADE,
    PRIMARY KEY(player_id)
);

