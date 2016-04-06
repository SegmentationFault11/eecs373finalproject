$(document).ready(function() {
    $('#home_controller').css('display', 'none');
    $('#select_car_controller').css('display', 'none');
    $('#game_progress_controller').css('display', 'none');

    router = new Router(window.location.hash)
    router.set_public_activities({
        'home_controller' : new MainActivity('home_controller', router),
        'game_progress_controller' : new Activity('game_progress_controller', router)
    });
    router.set_private_activities({
        'select_car_controller' : new Activity('select_car_controller', router)
    });
    router.route_to_current_activity();

    // random = {'aary' : new Activity('home_controller', router),
    //           'someone' : new Activity('select_car_controller', router)};
    // console.log(Object.keys(random)[0]);
});
