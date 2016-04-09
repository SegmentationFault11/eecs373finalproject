function MainActivity(id_in, router_in) {
    
    Activity.call(this, id_in, router_in);

    // The button to start the game
    this.start_button = {};
    this.game_data = {};
}

MainActivity.prototype = new Activity();

MainActivity.prototype.on_show = function(optional_data) {
    
    // get data from the server and route appropriately 
    var data_from_get_request;

    // check if a game exists
    $.getJSON("/game", function(data) {
        this.game_data = data;
        console.log(data);

    }.bind(this));

    // switch to other controller if a game has started in the backend
    if (this.game_data.length == 0) {
        console.log("MainActivity : Staying in current activity");
    } else {
        console.log("MainActivity : Switching to select_car_controller");
        return "select_car_controller";
    }

    // wire up widgets to activity
    this.wire_up_widgets();
};

MainActivity.prototype.wire_up_widgets = function() {
    this.start_button = $('#start_button');

    this.start_button.click(function() {
        console.log("MainActivity : Button #start_button pressed");
        console.log("MainActivity : Switching to select_car_controller");
        this.router.switch_to('select_car_controller');
    }.bind(this));
};

