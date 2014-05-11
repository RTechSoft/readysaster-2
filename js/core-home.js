jQuery(document).ready(function(){
    // Window Resize Listener
    jQuery(window).resize(function() {
        resizeMap();
    });
});

function registerOrLogin(response){
    jQuery.post(Drupal.settings.basePath+'readysaster/get-user-info', { profileString : JSON.stringify(response)}, function(data){
        // Check if registered
        if(data.status){
            plotHazardsToMap(data, response);
        }else{
            // ELSE Send to registration Page
            window.location.href = Drupal.settings.basePath+'readysaster/register';
        }
    }, 'json');
}

function plotHazardsToMap(data, response) {
    var lastLocCoor = String(data.content.field_last_location_geo_code['und'][0]['safe_value']).split(' ');
    var userLastLocation = new google.maps.LatLng(lastLocCoor[0], lastLocCoor[1]);
    var mapOptions = {
        zoom: 9,
        center: userLastLocation
    }
    
    var map = new google.maps.Map(document.getElementById('map-canvas'), mapOptions);
    
    // var ctaLayer = new google.maps.KmlLayer({
        // url: 'http://downloads.noah.dost.gov.ph/downloads/Iba_na_ang_Panahon_Nationwide_IEC/Region%20III/storm%20surge/HM_4_6.kmz'
    // });
    
    // ctaLayer.setMap(map);
    
    // Fixate WIDTH AND HEIGHT OF CANVAS
    resizeMap();
    userInfoPopulate(data, response);
}

function userInfoPopulate(data, response){
    console.log(data);
    console.log(response);
}

function resizeMap(){
    jQuery('#map-canvas').css('height', jQuery(window).height()+'px');
}