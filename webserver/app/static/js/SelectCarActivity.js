function SelectCarActivity(id_in, router_in) {
    
    Activity.call(this, id_in, router_in);

    // inner function variables
    this.car_types = [];
    this.player_and_cars = [];
    this.current_player = -1;
}

SelectCarActivity.prototype = new Activity();

SelectCarActivity.prototype.on_show = function(optional_data) {
    
    // Make get requests
    this.get_car_types();
    this.get_players_and_cars();
    this.get_current_player();

    // wire up widgets to activity
    this.wire_up_widgets();
}

SelectCarActivity.prototype.wire_up_widgets = function() {
    this.redraw();
};

SelectCarActivity.prototype.redraw = function() {
    var the_template_script = $('#car_thumbnail_template').html();

    // compile the template from the car_types variable
    the_template = Handlebars.compile(the_template_script);
    var context = { car_types : [] };
    for (var i = 0; i < this.car_types.length; ++i) {
        context.car_types.push({
            car_type : this.car_types[i].car_type,
            image_url : "./static/" + this.car_types[i].car_type + ".png"
        });
    }
    console.log(context);

    // set the current player to the context
    context.current_player = this.current_player;

    var compiled_html = the_template(context);
    $('#car_thumbnail_template_placeholder').html(compiled_html)
};

SelectCarActivity.prototype.get_car_types = function() {

    // get the number of players and the types of cars
    $.getJSON("/car_type", function(data) {
        console.log("SelectCarActivity : Made sjax get request to /car_type");
        console.log(data);

        // assign to this variable
        this.car_types = data;
    }.bind(this));
}

SelectCarActivity.prototype.get_players_and_cars = function() {

    // get the players that are in the backend already, these guys have
    // already selected their cars
    $.getJSON("/player_and_car", function(data) {
        console.log("SelectCarActivity : Made sjax get request to /player_and_car");
        console.log(data);

        this.player_and_cars = data;
    }.bind(this));
};

SelectCarActivity.prototype.get_current_player = function() {
    this.current_player = this.player_and_cars.length + 1;
};

