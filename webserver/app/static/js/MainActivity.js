function MainActivity(id_in, router_in) {

    // inheritance? Whatever the fuck this is
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
    $.getJSON("http://localhost:8000/game", function(data) {
        this.game_data = data;
        console.log(data);

        // switch to other controller if a game has started in the backend
        if (!this.game_data || this.game_data.length == 0) {
            console.log("MainActivity : Staying in current activity");
        } else {
            console.log("MainActivity : Switching to select_car_controller");
            this.router.switch_to("select_car_controller");
        }

        this.redraw();
    }.bind(this));

    // wire up widgets to activity
    this.wire_up_widgets();
};

MainActivity.prototype.wire_up_widgets = function() {
    this.start_button = $('#start_button');

    this.start_button.click(function() {

        // make ajax request to post a new game onto the server
        $.ajax({
            type: "POST", url:'/game',
            contentType: "application/json; charset=utf-8", 
            dataType: "json", data: JSON.stringify([{game_id:1, game_stage:1}]),

            success: function (data) {
                console.log(data);
            }
        });

        // then switch to the other controller
        console.log("MainActivity : Button #start_button pressed");
        console.log("MainActivity : Switching to select_car_controller");
        this.router.switch_to('select_car_controller');
    }.bind(this));
};

