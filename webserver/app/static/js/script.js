$(document).ready(function() {
    // make all requests synchronous
    // $.ajaxSetup({async:false});

    $('#home_controller').css('display', 'none');
    $('#select_car_controller').css('display', 'none');
    $('#game_progress_controller').css('display', 'none');
    $('#start_game_controller').css('display', 'none');

    var router = new Router(window.location.hash)
    router.set_public_activities({
        'home_controller' : new MainActivity('home_controller', router),
        'game_progress_controller' : new Activity('game_progress_controller', router)
    });
    router.set_private_activities({
        'select_car_controller' : new SelectCarActivity('select_car_controller', router),
        'start_game_controller' : new StartGameActivity('start_game_controller', router)
    });
    router.route_to_current_activity();

    this.number_of_requests = 1;
    function wait_for(condition, milliseconds, callback) {

        // if the condition is not met then wait and recurse, otherwise
        // callback()
        if (!condition()) {
            setTimeout(function() {
                wait_for(condition, milliseconds, callback);
            }, milliseconds);

        } else {
            callback();
        }
    }
    wait_for(function() { return this.number_of_requests === 0; }.bind(this), 100, 
            function() { console.log("done!"); });
    setTimeout(function() { --this.number_of_requests; }.bind(this), 2000);
});
