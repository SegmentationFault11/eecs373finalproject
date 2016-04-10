function SelectCarActivity(id_in, router_in) {

    Activity.call(this, id_in, router_in);

    // get the ajax requester object
    this.ajax_requester = new AjaxRequester();

    // inner function variables
    this.game = {};
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
    this.ajax_requester.get("/game", function(data) {
        this.game = data;
    }.bind(this));

    this.ajax_requester.wait_for_all(function() {

        if (this.game[0].game_stage > 1) {
            this.router.switch_to("start_game_controller");
        } else {
            this.get_data_and_redraw();
        }
    }.bind(this));
}

SelectCarActivity.prototype.get_data_and_redraw = function() {

    // get data from the API server
    this.ajax_requester.get("/car_type", function(data) {
        this.car_types = data;
    }.bind(this));
    this.ajax_requester.get("/player_and_car", function(data) {
        this.player_and_cars = data;
    }.bind(this));

    // do this when the above two calls are complete
    this.ajax_requester.wait_for_all(function() {
        this.current_player = this.player_and_cars.length + 1;
        this.redraw();
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

SelectCarActivity.prototype.wire_up_widgets = function() {

    for (var i = 0; i < this.car_types.length; ++i) {

        // run a closure so that the value of i is copied over
        (function(i) {

            this.select_car_buttons.push($("#button_select_car_" +
                    this.car_types[i].car_type));

            this.select_car_buttons[this.select_car_buttons.length - 1]
                .click(function() {

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
};

