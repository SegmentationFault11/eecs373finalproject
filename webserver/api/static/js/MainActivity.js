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
    $.ajax({
        url: 'http://localhost:8000/game',
        success: function (result) {
            console.log("WE ARE HERE " + result);
        },
        async: false
    });
    

    // wire up widgets to activity
    this.wire_up_widgets();
}

MainActivity.prototype.wire_up_widgets = function() {
    this.start_button = $('#start_button');

    this.start_button.click(function() {
        console.log("button pressed");
        this.router.switch_to('select_car_controller');
    }.bind(this));
}
