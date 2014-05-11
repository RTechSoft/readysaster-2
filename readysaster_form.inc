<?php
function readysaster_form_people_node_form_alter(&$form, &$form_state) {
    // HIDE to User
    unset($form['title']);
    unset($form['body']);
    unset($form['field_raw_hazard_info']);
    
    // Load JSON OBJ
    $fbInfoObj = json_decode(html_entity_decode($_SESSION['capturedFbInfo']));
    
    // Load Default Values
    if(isset($fbInfoObj->first_name)){
        $form['field_first_name']['und'][0]['value']['#default_value'] = $fbInfoObj->first_name;
    }
    
    if(isset($fbInfoObj->last_name)){
        $form['field_last_name']['und'][0]['value']['#default_value'] = $fbInfoObj->last_name;
    }
    
    if(isset($fbInfoObj->email)){
        $form['field_email']['und'][0]['value']['#default_value'] = $fbInfoObj->email;
    }
    
    if(isset($fbInfoObj->hometown->name)){
        $form['field_home_town']['und'][0]['value']['#default_value'] = $fbInfoObj->hometown->name;
    }
    
    if(isset($fbInfoObj->location->name)){
        $form['field_last_location']['und'][0]['value']['#default_value'] = $fbInfoObj->location->name;
    }
    
    if(isset($fbInfoObj->id)){
        $form['field_facebook_id']['und'][0]['value']['#default_value'] = $fbInfoObj->id;
    }
    
    // Return Form
    return $form;
}

function readysaster_form_hazard_node_form_alter(&$form, &$form_state) {
    // // HIDE to User
    // unset($form['body']);
    
    // // Return Form
    // return $form;
}
