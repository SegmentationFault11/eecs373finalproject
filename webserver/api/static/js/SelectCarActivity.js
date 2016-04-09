function SelectCarActivity(id_in, router_in) {
    
    Activity.call(this, id_in, router_in);

}

SelectCarActivity.prototype = new Activity();

SelectCarActivity.prototype.on_show = function(optional_data) {
    
    // wire up widgets to activity
    this.wire_up_widgets();
}

SelectCarActivity.prototype.wire_up_widgets = function() {
    var the_template_script = $('#car_thumbnail_template').html()

    the_template = Handlebars.compile(the_template_script);
    var context = { car_types : [{image_url : './static/fast.png', car_type :'Fast'}, 
        {image_url : './static/tank.png', car_type : 'Fucking tank'}, 
        {image_url : './static/strong.png', car_type : 'Monster Truck'}, 
        {image_url : './static/trump.png', car_type : 'LOL'}] };
    var compiled_html = the_template(context);
    $('#car_thumbnail_template_placeholder').html(compiled_html)
}
