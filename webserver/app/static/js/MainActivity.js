function MainActivity(id_in, router_in) {
    
    Activity.call(this, id_in, router_in);

    // The button to start the game
    this.start_button = {}
}

MainActivity.prototype = new Activity();

MainActivity.prototype.on_show = function(optional_data) {
    
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
