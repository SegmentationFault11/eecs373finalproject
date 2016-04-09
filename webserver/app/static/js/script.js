$(document).ready(function() {
    // make all requests synchronous
    // $.ajaxSetup({async:false});

    $('#home_controller').css('display', 'none');
    $('#select_car_controller').css('display', 'none');
    $('#game_progress_controller').css('display', 'none');

    var router = new Router(window.location.hash)
    router.set_public_activities({
        'home_controller' : new MainActivity('home_controller', router),
        'game_progress_controller' : new Activity('game_progress_controller', router)
    });
    router.set_private_activities({
        'select_car_controller' : new SelectCarActivity('select_car_controller', router)
    });
    router.route_to_current_activity();
});
