(function(d, s, id){
    var js, fjs = d.getElementsByTagName(s)[0];
    if (d.getElementById(id)) {return;}
    js = d.createElement(s); js.id = id;
    js.src = '//connect.facebook.net/en_US/sdk.js';
    fjs.parentNode.insertBefore(js, fjs);
}(document, 'script', 'facebook-jssdk'));

window.fbAsyncInit = function() {
    
    FB.init({
        appId      : '1430624693862514',
        status     : true,
        xfbml      : true,
        cookie     : true,
        version    : 'v2.0'
    });
    
    FB.login(function(response){
        if (response.authResponse){
                // SUCCESS
                FB.api('/me', function(response){
                    // var fbProfleString = JSON.stringify(response);
                    // console.log(fbProfleString);
                    registerOrLogin(response);
                });
            } else {
            // ERROR
            console.log(response);
        }
    },{
        scope: 'publish_actions,user_about_me,user_location,user_birthday,user_hometown,email',
        return_scope: true
    });
    
};