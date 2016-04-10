function SelectCarActivity(id_in, router_in) {

    Activity.call(this, id_in, router_in);

    // inner function variables
    this.car_types = [];
    this.player_and_cars = [];
    this.current_player = -1;

    // the buttons that are used to select which car the player is going to
    // use
    this.select_car_buttons = [];

}

SelectCarActivity.prototype = new Activity();

SelectCarActivity.prototype.on_show = function(optional_data) {

    // Make get requests
    $.getJSON("/game", function(data) {
        if (data[0].game_stage > 1) {
            this.router.switch_to("start_game_controller");
        }
    }.bind(this));
    this.make_cascading_ajax_requests_and_redraw();
}

SelectCarActivity.prototype.wire_up_widgets = function() {

    for (var i = 0; i < this.car_types.length; ++i) {

        // run a closure so that the value of i is copied over
        (function(i) {

            this.select_car_buttons.push($("#button_select_car_" +
                    this.car_types[i].car_type));

            this.select_car_buttons[this.select_car_buttons.length - 1]
            .click(function() {

                console.log("PRESSED SLEECT BUTTON!");
                // log to the console 
                // console.log("MAKING POST TO PLAYER AND CAR");
                // console.log(this.car_types[i].car_type);

                this.make_post_request_to_url("/player_and_car", 
                    [{player_id: this.current_player,
                        car_type: this.car_types[i].car_type, car_health: 10,
                        kills: 0, game_id: 1}
                    ], function() {

                        // redraw the screen
                        this.select_car_buttons = [];
                        this.router.switch_to("select_car_controller");

                }.bind(this));

            }.bind(this));

        }).bind(this)(i);

    }
    // console.log("SelectCarActivity : buttons hooked up");
    // console.log(this.select_car_buttons);
};

SelectCarActivity.prototype.make_cascading_ajax_requests_and_redraw = function() {

    this.get_car_types(function() {
        this.get_players_and_cars(function() {
            this.get_current_player(function() {
                this.redraw();
            }.bind(this));
        }.bind(this));
    }.bind(this));
};


SelectCarActivity.prototype.redraw = function() {

    var context = { car_types : [] };
    for (var i = 0; i < this.car_types.length; ++i) {
        context.car_types.push({
            car_type : this.car_types[i].car_type,
            image_url : "/pictures/" + this.car_types[i].car_type + ".png"
        });
    }
    // set the current player to the context
    context.current_player = this.current_player;

    this.redraw_handlebar_template_with_context(
            '#car_thumbnail_template', '#car_thumbnail_template_placeholder',
            context);

    // since new views have been draws they need to be wired up
    this.wire_up_widgets();
};

SelectCarActivity.prototype.get_car_types = function(cascading_callback) {

    // get the number of players and the types of cars
    $.getJSON("http://localhost:8000/car_type", function(data) {
        // console.log("SelectCarActivity : Made sjax get request to /car_type");
        // console.log(data);

        // assign to this variable
        this.car_types = data;
        cascading_callback();
    }.bind(this));
}

SelectCarActivity.prototype.get_players_and_cars = function(cascading_callback) {

    // get the players that are in the backend already, these guys have
    // already selected their cars
    $.getJSON("http://localhost:8000/player_and_car", function(data) {
        // console.log("SelectCarActivity : Made sjax get request to /player_and_car");
        // console.log(data);

        this.player_and_cars = data;
        cascading_callback();
    }.bind(this));
};

SelectCarActivity.prototype.get_current_player = function(cascading_callback) {
    this.current_player = this.player_and_cars.length + 1;
    cascading_callback();
};

