jQuery(document).ready(function(){
    initialize();
    jQuery('#edit-field-kml-geocode-und-0-value').val('');
    
    // Listener to OnSubmit
    jQuery('#edit-submit').click(function(e){
        e.preventDefault();
        var polygonCoorRaw = String(jQuery('#edit-field-kml-geocode-und-0-value').val());
        var polygonCoorSplit = polygonCoorRaw.split(',');
        var polygonCoorFinal = polygonCoorRaw+','+polygonCoorSplit[0];
        jQuery('#edit-field-kml-geocode-und-0-value').val(polygonCoorFinal);
        jQuery('#hazard-node-form').submit();
    });
});

function initialize() {
    // Configure Map
    var mapOptions = {
        center: new google.maps.LatLng(14.5809814, 121.0446369),
        zoom: 8
    };
    
    // Plot map on DIV
    var map = new google.maps.Map(document.getElementById('map-canvas'), mapOptions);
    
    // Put Marker
    var marker = new google.maps.Marker({
        position: mapOptions.center,
        map: map,
        title: 'This is your current location!'
    });
    
    var marker = new google.maps.Marker({
        position: new google.maps.LatLng(14.6514286, 120.96177039999998),
        map: map,
        title: 'This is your current locations!'
    });
    
    // Path Storage
    var path = [];
    
    // Initiate
    polygonLayer = new google.maps.Polygon({
        strokeColor: '#ff0000',
        strokeOpacity: 1.0,
        strokeWeight: 2
    });
    
    polygonLayer.setMap(map);
    
    // Listener
    google.maps.event.addListener(map, 'click', addNewPoint);
    
    // Override Style HEIGHT + WIDTH
    jQuery('#map-canvas').css('height', '400px');
    jQuery('#map-canvas').css('width', jQuery('#block-system-main').width()+'px');
    
}

function addNewPoint(e) {
    var path = polygonLayer.getPath();
    path.push(e.latLng);
    
    // Save / Append to KML Geo Code Field
    var capturedCoor = String(jQuery('#edit-field-kml-geocode-und-0-value').val());
    var leadingComma = (capturedCoor.length > 0) ? ',' : '';
    jQuery('#edit-field-kml-geocode-und-0-value').val(String(capturedCoor)+String(String(leadingComma)+e.latLng.k+' '+e.latLng.A));
}