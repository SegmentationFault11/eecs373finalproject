function StartGameActivity(id_in, router_in) {

    // inheritance? Whatever the fuck this is
    Activity.call(this, id_in, router_in);

    // the button to start the game
    this.start_game_button = {};
    this.game = {};
    this.ajax_requester = new AjaxRequester();
}

StartGameActivity.prototype = new Activity();

StartGameActivity.prototype.on_show = function(optional_data) {

    // check the game stage
    this.ajax_requester.get("/game", function(data) {
        this.game = data[0];
    }.bind(this));

    this.ajax_requester.wait_for_all(function() {

        console.log("GAME IS ");
        console.log(this.game);
        if(this.game.game_stage > 2) {
            this.router.switch_to("game_progress_controller");
        } else {

            // wire up the static widget
            this.wire_up_widgets();
            this.show_views();
        }
    }.bind(this));
};

StartGameActivity.prototype.wire_up_widgets = function() {

    this.start_game_button = $('#button_start_game');

    this.start_game_button.click(function() {
        this.ajax_requester.get("/start_game", function(data) {
            alert("STARTED GAME!");
            this.router.switch_to("game_progress_controller");
        }.bind(this));
    }.bind(this));
};

