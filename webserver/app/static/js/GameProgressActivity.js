function GameProgressActivity(id_in, router_in) {

    // inheritance? Whatever the fuck this is
    Activity.call(this, id_in, router_in);

    // get a new instance of an ajax requester
    this.ajax_requester = new AjaxRequester();

    // the object that contains all the data on the page
    this.player_and_cars = [];
    this.events = [];
}

GameProgressActivity.prototype = new Activity();

GameProgressActivity.prototype.on_show = function(optional_data) {

    this.ajax_requester.get("/player_and_car", function(data) {
        this.player_and_cars = data;
    }.bind(this));
    this.ajax_requester.get("/events", function(data) {
        this.events = data;
    }.bind(this));

    this.ajax_requester.wait_for_all(function() {
        this.redraw();
        this.wire_up_widgets();
        this.show_views();
    }.bind(this));

    setTimeout(function() {
        this.on_show();
    }.bind(this), 2000);
};

GameProgressActivity.prototype.redraw = function() {
    var context = {};
    context.player_and_cars = this.player_and_cars;
    context.events = this.events;
    console.log(context);

    this.redraw_handlebar_template_with_context('#player_progress_template',
            '#player_progress_template_placeholder', context);

}
GameProgressActivity.prototype.wire_up_widgets = function() {};

