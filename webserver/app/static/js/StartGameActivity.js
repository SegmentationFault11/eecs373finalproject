function StartGameActivity(id_in, router_in) {

    // inheritance? Whatever the fuck this is
    Activity.call(this, id_in, router_in);

    // the button to start the game
    this.start_game_button = {};
    this.ajax_requester = new AjaxRequester();
}

StartGameActivity.prototype = new Activity();

StartGameActivity.prototype.on_show = function(optional_data) {

    // wire up the static widget
    this.wire_up_widgets();
    this.show_views();
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

