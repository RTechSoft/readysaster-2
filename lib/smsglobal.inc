<?php
class SmsGlobal{
    
    const username = '1r7m02qw';
    const password = 'M3poZEHZ';
    
    public static function send($params){
        
        // Header Request Values
        $content =  'action=sendsms'. 
                    '&user='.rawurlencode(self::username). 
                    '&password='.rawurlencode(self::password). 
                    '&to='.rawurlencode($params['destination']). 
                    '&from='.rawurlencode($params['source']). 
                    '&text='.rawurlencode($params['text']);
        
        // Fire and Forget ( SILENT )
        $smsglobal_response = file_get_contents('http://www.smsglobal.com.au/http-api.php?'.$content);
        
    }
}