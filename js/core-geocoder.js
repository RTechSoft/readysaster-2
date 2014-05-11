jQuery(document).ready(function(){
    // READONLY SELECTED FIELDS
    jQuery('#edit-field-facebook-id-und-0-value, #edit-field-last-location-geo-code-und-0-value, #edit-field-home-town-geo-code-und-0-value').attr('readonly','readonly');
    
    // GET GECODE
    jQuery('#edit-field-home-town-und-0-value').change(function(){
        getHomeTownGeoCode(jQuery(this).val());
    });
    jQuery('#edit-field-home-town-und-0-value').trigger('change');
    
    // GET GEOCODE
    jQuery('#edit-field-last-location-und-0-value').change(function(){
        getLastLocationGeoCode(jQuery(this).val());
    });
    jQuery('#edit-field-last-location-und-0-value').trigger('change');
});

function getHomeTownGeoCode(address){
    var geocoder = new google.maps.Geocoder();
    geocoder.geocode({ 'address': address}, function(results, status) {
        if (status == google.maps.GeocoderStatus.OK) {
            jQuery('#edit-field-home-town-geo-code-und-0-value').val(results[0].geometry.location.lat()+' '+results[0].geometry.location.lng());
        }
    });
}

function getLastLocationGeoCode(address){
    var geocoder = new google.maps.Geocoder();
    geocoder.geocode({ 'address': address}, function(results, status) {
        if (status == google.maps.GeocoderStatus.OK) {
            jQuery('#edit-field-last-location-geo-code-und-0-value').val(results[0].geometry.location.lat()+' '+results[0].geometry.location.lng());
        }
    });
}