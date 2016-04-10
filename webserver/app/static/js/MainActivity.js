function MainActivity(id_in, router_in) {

    // inheritance? Whatever the fuck this is
    Activity.call(this, id_in, router_in);

    // get a new instance of an ajax requester
    this.ajax_requester = new AjaxRequester();

    // The button to start the game
    this.start_button = {};
    this.game_data = {};
}

MainActivity.prototype = new Activity();

MainActivity.prototype.on_show = function(optional_data) {

    // check if a game exists
    this.ajax_requester.get('/game', function(data) {
        this.game_data = data;
    }.bind(this));
    
    this.ajax_requester.wait_for_all(function() {
         
        // switch to other controller if a game has started in the backend
        if (!this.game_data || this.game_data.length == 0) {
            console.log("MainActivity : Staying in current activity");

            // wire up widgets to activity now because all requests have been
            // made
            this.redraw(); // empty but here for clarity
            this.wire_up_widgets();
            this.show_views();

        } else {
            console.log("MainActivity : Switching to select_car_controller");
            this.router.switch_to("select_car_controller");
            return;
        }

    }.bind(this));

};

MainActivity.prototype.wire_up_widgets = function() {
    this.start_button = $('#start_button');

    this.start_button.click(function() {

        // make ajax request to post a new game onto the server
        this.ajax_requester.post('/game', [{game_id:1, game_stage:1}], 
            function() {
                this.router.switch_to('select_car_controller');
        }.bind(this));

    }.bind(this));
};

