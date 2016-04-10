function StartGameActivity(id_in, router_in) {

    // inheritance? Whatever the fuck this is
    Activity.call(this, id_in, router_in);

    // the button to start the game
    this.start_game_button = {};
}

StartGameActivity.prototype = new Activity();

StartGameActivity.prototype.on_show = function(optional_data) {

    // wire up the static widget
    this.wire_up_widgets();
};

StartGameActivity.prototype.wire_up_widgets = function() {

    this.start_game_button = $('button_start_game');

    this.start_game_button.click(function() {
        alert("STARTED GAME!");
    });
};

