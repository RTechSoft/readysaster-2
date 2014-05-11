<?php
function readysaster_home_page() {
	// Attach javascript code.
    drupal_add_css(drupal_get_path('module', 'readysaster') .'/css/readysaster.css');
    drupal_add_js(drupal_get_path('module', 'readysaster') .'/js/core-fb.js');
	drupal_add_js(drupal_get_path('module', 'readysaster') .'/js/core-home.js');
	drupal_add_js(drupal_get_path('module', 'readysaster') .'/js/helper-arr-json.js');
    drupal_add_js('http://maps.googleapis.com/maps/api/js?v=3.exp&sensor=false');
    
	// Return constructed table to Drupal for theming
	return theme('view_home', array('user_profile' => 'Google Map'));
}

function readysaster_register_page() {
    drupal_add_js('http://maps.googleapis.com/maps/api/js?v=3.exp&sensor=false');
    drupal_add_js(drupal_get_path('module', 'readysaster') .'/js/core-geocoder.js');
    
    // Check first if module is already loaded before attaching it.
    if(module_exists('node')){
        module_load_include('inc', 'node', 'node.pages');
    }
    
    // Initialize Input form of User
    $node_form = new stdClass;
    $node_form->type = 'people';
    $node_form->language = LANGUAGE_NONE;
    $form = drupal_get_form('people_node_form', $node_form);
    
	// Return constructed table to Drupal for theming
	return theme('view_register', array('user_register_profile' => $form));
}

function readysaster_hazard_input_page() {
    drupal_add_js('http://maps.googleapis.com/maps/api/js?v=3.exp&sensor=false&libraries=drawing');
    drupal_add_js(drupal_get_path('module', 'readysaster') .'/js/core-hazard-form.js');
    
    // Check first if module is already loaded before attaching it.
    if(module_exists('node')){
        module_load_include('inc', 'node', 'node.pages');
    }
    
    // Initialize Input form of User
    $node_form = new stdClass;
    $node_form->type = 'hazard';
    $node_form->language = LANGUAGE_NONE;
    $form = drupal_get_form('hazard_node_form', $node_form);
    
	// Return constructed table to Drupal for theming
	return theme('view_hazard_form', array('user_register_profile' => $form));
}

/* Helpers */

// LISTENER ON HAZARD SUBMIT TO CHECK ALL USERs THAT IS AFFECTED BY THE HAZARD
function readysaster_hazard_instersect_entity($object, $context) {
    if($context['node']->type == 'hazard'){
        
        // Construct Query for People
        $query = new EntityFieldQuery;
        $query->entityCondition('entity_type', 'node');
        $query->entityCondition('bundle', 'people');
        $query->propertyCondition('status', 1);
        $results = $query->execute();// Execute Query
        
        if($results){
            foreach($results['node'] as $key => $value){
                
                // ENTITY
                $entityNodeLoad = node_load($value->nid, $value->vid);
                
                // DISASTER COVERAGE
                $polygon = explode(',', $context['node']->field_kml_geocode['und'][0]['value']);
                
                // EXECUTE CROSS CHECK FOR INTERSECTIONS
                $pointLocation = new pointLocation();
                $location = $pointLocation->pointInPolygon($entityNodeLoad->field_last_location_geo_code['und'][0]['safe_value'], $polygon);
                
                // IF INSIDE
                if($location == 'inside'){
                    
                    // EMAIL ALERT
                    $mailFrom = 'kim.gutierrez@1150tech.com';
                    $mailTo = $entityNodeLoad->field_email['und'][0]['safe_value'];
                    $mailSubject = 'Disaster Alert!';
                    $mailMessage = 'Imminent disaster in your location';
                    readysaster_drupal_mail($mailFrom, $mailTo, $mailSubject, $mailMessage);
                    
                    // SMS TEXT ALERT
                    $params = array(
                        'destination' => $entityNodeLoad->field_mobile_no_['und'][0]['safe_value'],
                        'source' => 'ReadySaster Alert',
                        'text' => 'Imminent disaster in your location',
                    );
                    // smsGlobal::send($params);
                    
                }
            }
            
            // REDIRECT
            drupal_goto('readysaster/home');
        }
        
    }
}

// LISTENER ON PEOPLE SUBMIT TO CHECK ALL HAZARD
function readysaster_entity_instersect_hazard($object, $context){
    if($context['node']->type == 'people'){
        
        // Construct Query for HAZARD
        $query = new EntityFieldQuery;
        $query->entityCondition('entity_type', 'node');
        $query->entityCondition('bundle', 'hazard');
        $query->propertyCondition('status', 1);
        $results = $query->execute();// Execute Query
        
        if($results){
            // COLLECTED HAZARD
            $warnHazard = array();
            
            // START LOOPING TO CHECK INTERSECTIONS
            foreach($results['node'] as $key => $value){
                
                // HAZARD
                $hazardNodeLoad = node_load($value->nid, $value->vid);
                
                // DISASTER COVERAGE
                $polygon = explode(',', $hazardNodeLoad->field_kml_geocode['und'][0]['value']);
                
                // EXECUTE CROSS CHECK FOR INTERSECTIONS
                $pointLocation = new pointLocation();
                $location = $pointLocation->pointInPolygon($context['node']->field_last_location_geo_code['und'][0]['value'], $polygon);
                
                // IF INSIDE
                if($location == 'inside'){
                    
                    // FEED TO WARNING HAZARD
                    $hazardTypeNodeLoad = node_load($hazardNodeLoad->field_hazard_type['und'][0]['target_id']);
                    $hazardLevelNodeLoad = taxonomy_term_load($hazardNodeLoad->field_hazard_level['und'][0]['tid']);
                    
                    $warnHazard[$hazardNodeLoad->nid] = array(
                        'title' => $hazardNodeLoad->title,
                        'type' => $hazardTypeNodeLoad->title,
                        'level' => $hazardLevelNodeLoad->name,
                        'description' => $hazardNodeLoad->body['und'][0]['safe_value'],
                    );
                    
                }
            }
            
            // CHECK AND GET COLLECTED HAZARD
            if(count($warnHazard) > 0){
                // dpm($warnHazard);
                
                // EMAIL ALERT
                $mailFrom = 'kim.gutierrez@1150tech.com';
                $mailTo = $context['node']->field_email['und'][0]['value'];
                $mailSubject = 'Disaster Alert!';
                $mailMessage = 'Imminent disaster in your location.';
                readysaster_drupal_mail($mailFrom, $mailTo, $mailSubject, $mailMessage);
                
                // SMS TEXT ALERT
                $params = array(
                    'destination' => $context['node']->field_mobile_no_['und'][0]['value'],
                    'source' => 'ReadySaster Alert',
                    'text' => 'Imminent disaster in your location.',
                );
                smsGlobal::send($params);
                
                // Update Entity
                $entityNodeLoad = node_load($context['node']->nid);
                $entityNodeLoad->field_raw_hazard_info['und'][0]['value'] = serialize($warnHazard);
                node_save($entityNodeLoad);
            }
            
        }
        
        // REDIRECT
        drupal_goto('readysaster/home');
    }
}

// AJAX OF USER INFO
function readysaster_get_user_info(){
    $fbProfileObj = json_decode(html_entity_decode($_POST['profileString']));
    
    // Construct Query
    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'people');
    $query->propertyCondition('status', 1);
    $query->fieldCondition('field_facebook_id', 'value', $fbProfileObj->id);
    $query->range(0,1);
    
    // Execute Query
    $results = $query->execute();
    
    // If No result save the posted value on session for later access.
    if(!$results){
        $_SESSION['capturedFbInfo'] = $_POST['profileString'];
    }
    
    // Load result into Object and print as JSON
    print json_encode(
            array(
                'status' => ($results) ? TRUE : FALSE, 
                'content' => node_load(array_shift(array_keys($results['node'])))
            )
        );
}

// AJAX OF USER UPDATE LAST LOCATION
function readysaster_update_user_last_location($lat, $lng, $fbId, $mobileNo){
    
    // CONSTRUCT DB QUERY WITH FBID CONDITION
    $query = db_select('node', 'n');
    $query->join('field_data_field_facebook_id', 'fb', 'fb.entity_id = n.nid');
    $query->fields('n', array('nid','vid'));
    $query->condition('n.type', 'people');
    $query->condition('fb.field_facebook_id_value', $fbId);
    $query->range(0,1);
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    
    if($result){
        $entityNodeLoad = node_load($result[0]['nid'], $result[0]['vid']);
        $postedLatLangCoor = $lat.' '.$lng;
        $savedLatLangCoor = $entityNodeLoad->field_last_location_geo_code['und'][0]['value'];
        
        // CHECK IF POSTED COORDINATE IS DIFFERENT, THIS MEANS HE CHANGED LOCATION
        if($postedLatLangCoor != $savedLatLangCoor){
            
            // UPDATE LOCATION
            $entityNodeLoad->field_last_location_geo_code['und'][0]['value'] = $postedLatLangCoor;
            node_save($entityNodeLoad);
        }
    }
}

// EMAIL FUNCTIONALITY
function readysaster_drupal_mail($from = 'default_from', $to, $subject, $message) {
    $my_module = 'readysaster';
    $my_mail_token = microtime();
    
    if($from == 'default_from') {
        $from = 'kim.d.gutierrez@gmail.com';
    }
    
    $message = array(
        'id' => $my_module . '_' . $my_mail_token,
        'to' => $to,
        'subject' => $subject,
        'body' => array($message),
        'headers' => array(
            'From' => $from, 
            'Sender' => $from, 
            'Return-Path' => $from,
        ),
    );
    
    $system = drupal_mail_system($my_module, $my_mail_token);
    $message = $system->format($message);
    
    if ($system->mail($message)) {
        return TRUE;
    }else{
        return FALSE;
    }
}

// CHANGE MAIN TEMPLATE ONLY ON HOME PAGE
function readysaster_preprocess_page(&$variables, $hook){
    if($_GET['q'] == 'readysaster/home'){
        $variables['theme_hook_suggestions'][] = 'readysaster_page';
    }
}

function readysaster_helper_split_uri($uri){
    $uriPieces = explode('/', $uri);
    return $uriPieces[0];
}