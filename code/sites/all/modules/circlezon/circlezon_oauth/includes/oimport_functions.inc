<?php

function cz_custom_import_from_twitter() {
  
   global $user; 
   
    $out = ''; 
     // delete existing of this user .. 
    db_query("delete from {cz_card} where uid = :uid and provider = :provider", array(':provider'=>'twitter', ':uid' => $user->uid)); 
    
    $out .= ''; 
    $provider = oauthconnector_provider_load('twitter');
    $csid = $provider->csid; 
     
     $result = db_query("SELECT * FROM {oauth_common_token} WHERE uid = :uid AND type = :type and csid = :csid", array(
        ':uid' => $user->uid,
        ':type' => OAUTH_COMMON_TOKEN_TYPE_ACCESS,
        ':csid' => $provider->csid,
      )); 
      
      $tokens = array(); 
      while ($token = DrupalOAuthToken::fromResult($result)) {
        $tokens[] = $token;
      }
     
    // echo '<pre>';
    
    // print_r($tokens); 
    $ltoken = $tokens[0]; // print_r( $ltoken); 
    if($ltoken)  { 

     $consumer = $ltoken->consumer;
    
    $method='GET'; 
    
    // print_r($provider);  
    $org_info = cz_oauthconnector_endpoint_call(
          'https://api.twitter.com/1.1/friends/list.json', // ?limit=5000 
          array('include_user_entities'=>true, 'skip_status'=>true, 'cursor'=>-1),
          $provider,
          $ltoken,
          $method,
            'json'
      ); 
      
      $ic = 0;  
      // http://developer.linkedin.com/documents/reading-data#toggleview:id=php  
      //print_r($org_info); // array 
      if( $org_info['users'] && is_array($org_info['users']) && count($org_info['users']) > 0 )  { // 
        
        foreach($org_info['users'] as $conn) { 
          $friendid = $conn['id']; 
          
          if($friendid) {
            $udata = $conn; 

          //print_r($udata); // break;  
          $providername = 'twitter'; 
          $created = time(); 
          $uid = $user->uid; 
          
          $firstname  =  isset($udata['name']) ? $udata['name'] : ''; 
          $lastname = ''; 
          $fullname = isset( $udata['name']) ? $udata['name'] : '';  
          $username = isset($udata['screen_name']) ? $udata['screen_name'] : ''; 
          $email = ''; 
          $address = isset($udata['location']) ? $udata['location'] : '';   
          $country = ''; 
          $organization = ''; 
          $phone = ''; 
          $picture = isset($udata['profile_image_url']) ? $udata['profile_image_url'] : ''; 
          $profileid = isset($udata['id']) ? $udata['id'] : '';  
          $profileurl = isset($udata['url']) ? $udata['url'] : ''; 
          $gender = isset($udata['gender']) ? $udata['gender'] : ''; 
          $industry = ''; 
          $headline = isset($udata['description']) ? $udata['description'] : '';                
          
          // check if profileid and provider doesnt exist 
          if( (bool) db_select('cz_card')->fields('cz_card', array('id'))->condition('profileid', $udata['id'], '=')->condition('provider','twitter', '=')->range(0, 1)->execute()->fetchField() ) { 
            // profile already exist.. 

          } else { 
            
            $rs = db_query("insert into {cz_card} ( `firstname`, `lastname`, `fullname`, `username`, `email`, `address`, `country`, `organization`, `phone`, `picture`, `profileid`, `profileurl`, `gender`, `industry`, `headline`, `created`, provider, uid) VALUES (:firstname, :lastname, :fullname, :username, :email, :address, :country, :organization, :phone, :picture, :profileid, :profileurl, :gender, :industry, :headline, :created, :provider, :uid)  ", array(':firstname'=>$firstname, ':lastname'=>$lastname, ':fullname'=>$fullname, ':username'=>$username, ':email'=>$email, ':address'=>$address, ':country'=>$country, ':organization'=>$organization, ':phone'=>$phone, ':picture'=>$picture, ':profileid'=>$profileid, ':profileurl'=>$profileurl, ':gender'=>$gender, ':industry'=>$industry, ':headline'=>$headline, ':created'=>$created, ':provider'=>$providername, ':uid'=>$uid) ); 
            if($rs) {
              $ic++; 
            } 
          }
          } // end udata 

        } 

        
        
      } 
      
    } else { 
      $out .= 'Account is not connnected..';
    } 
  
  $out .= $ic.' unique contacts imported.' ;

  $out .= '<br/>'.l('View Imported Contacts', 'user/'.$user->uid.'/icontacts'); 
  
  return $out; 
}

function cz_custom_import_from_facebook() {
   global $user; 
   
    $out = ''; 
     // delete existing of this user .. 
    db_query("delete from {cz_card} where uid = :uid and provider = :provider", array(':provider'=>'facebook', ':uid' => $user->uid)); 
    
    $out .= ''; 
    $provider = oauthconnector_provider_load('facebook');
    $csid = $provider->csid; 
     
     $result = db_query("SELECT * FROM {oauth_common_token} WHERE uid = :uid AND type = :type and csid = :csid", array(
        ':uid' => $user->uid,
        ':type' => OAUTH_COMMON_TOKEN_TYPE_ACCESS,
        ':csid' => $provider->csid,
      )); 
      
      $tokens = array(); 
      while ($token = DrupalOAuthToken::fromResult($result)) {
        $tokens[] = $token;
      }
     
    // echo '<pre>';
    
    // print_r($tokens); 
    $ltoken = $tokens[0]; // print_r( $ltoken); 
    if($ltoken)  { 

     $consumer = $ltoken->consumer;
    
    $method='GET'; 
    
    // print_r($provider);  
    $org_info = cz_oauthconnector_endpoint_call(
          'https://graph.facebook.com/me/friends', // ?limit=5000 
          array('limit'=>5000, 'fields' => 'id,picture,name'),
          $provider,
          $ltoken,
          $method,
            'json'
      ); 
      
      $ic = 0;  
      // http://developer.linkedin.com/documents/reading-data#toggleview:id=php  
      // print_r($org_info['data']); exit; // array 
      if( $org_info['data'] && is_array($org_info['data']) )  {
        
        foreach($org_info['data'] as $conn) { 
          $friendid = $conn['id']; 
          $spicurl = isset($conn['picture']['data']['url']) ? $conn['picture']['data']['url'] : ''; 
          
          $uf_picdata = cz_oauthconnector_endpoint_call(
              'https://graph.facebook.com/'.$friendid.'', // ?limit=5000 
              array('fields'=>'picture.type(large)'),
              $provider,
              $ltoken,
              $method,
              'json'
          ); 
          // print_r($uf_picdata);  

          $spicurl =  isset($uf_picdata['picture']['data']['url']) ? $uf_picdata['picture']['data']['url'] : $spicurl; 
          
          $udata = cz_oauthconnector_endpoint_call(
              'https://graph.facebook.com/'.$friendid, // ?limit=5000 
              array(),
              $provider,
              $ltoken,
              $method,
              'json'
          ); 
          
          if($udata) { 
            
          //print_r($udata); // break;  
          $providername = 'facebook'; 
          $created = time(); 
          $uid = $user->uid; 

          $firstname  =  isset($udata['first_name']) ? $udata['first_name'] : ''; 
          $lastname = isset($udata['last_name']) ? $udata['last_name'] : ''; 
          $fullname = isset( $udata['name']) ? $udata['name'] : '';  
          $username = isset($udata['username']) ? $udata['username'] : ''; 
          $email = ''; 
          $address = ''; 
          $country = ''; 
          $organization = ''; 
          $phone = ''; 
          $picture = $spicurl; 

          $profileid = isset($udata['id']) ? $udata['id'] : '';  
          $profileurl = isset($udata['link']) ? $udata['link'] : ''; 
          $gender = isset($udata['gender']) ? $udata['gender'] : ''; 
          $industry = ''; 
          $headline = '';                 
          
          // check if profileid and provider doesnt exist 
          if( (bool) db_select('cz_card')->fields('cz_card', array('id'))->condition('profileid', $udata['id'], '=')->condition('provider','facebook', '=')->range(0, 1)->execute()->fetchField() ) { 
            // profile already exist.. 

          } else { 
            
            $rs = db_query("insert into {cz_card} ( `firstname`, `lastname`, `fullname`, `username`, `email`, `address`, `country`, `organization`, `phone`, `picture`, `profileid`, `profileurl`, `gender`, `industry`, `headline`, `created`, provider, uid) VALUES (:firstname, :lastname, :fullname, :username, :email, :address, :country, :organization, :phone, :picture, :profileid, :profileurl, :gender, :industry, :headline, :created, :provider, :uid)  ", array(':firstname'=>$firstname, ':lastname'=>$lastname, ':fullname'=>$fullname, ':username'=>$username, ':email'=>$email, ':address'=>$address, ':country'=>$country, ':organization'=>$organization, ':phone'=>$phone, ':picture'=>$picture, ':profileid'=>$profileid, ':profileurl'=>$profileurl, ':gender'=>$gender, ':industry'=>$industry, ':headline'=>$headline, ':created'=>$created, ':provider'=>$providername, ':uid'=>$uid) ); 
            if($rs) {
              $ic++; 
            } 
          }
          } // end udata 

        } 

        $out .= $ic.' unique contacts imported.' ;
        
      } 
            
    } else { 
      $out .= 'Account is not connnected..';
    } 
  
  $out .= '<br/>'.l('View Imported Contacts', 'user/'.$user->uid.'/icontacts'); 

  return $out; 
} 

function cz_custom_import_from_linkedin() {
    // ...
    global $user; 

    $out = '';
      // delete existing of this user .. 
    db_query("delete from {cz_card} where uid = :uid and provider = :provider", array(':provider'=>'linkedin', ':uid' => $user->uid)); 
    
    $out .= ''; 
     // ??? 
     $provider = oauthconnector_provider_load('linkedin');
     $csid = $provider->csid;

     $result = db_query("SELECT * FROM {oauth_common_token} WHERE uid = :uid AND type = :type and csid = :csid", array(
        ':uid' => $user->uid,
        ':type' => OAUTH_COMMON_TOKEN_TYPE_ACCESS,
        ':csid' => $provider->csid,
      )); 

      
      $tokens = array();
      while ($token = DrupalOAuthToken::fromResult($result)) {
        $tokens[] = $token;
      }
     
    //echo '<pre>'; 
    // print_r($tokens); 
    $ltoken = $tokens[0]; 
    // print_r( $ltoken); 
    if($ltoken)  { 
   $consumer = $ltoken->consumer; 
    $provider = oauthconnector_provider_load('linkedin');
    $method='GET'; 
     //print_r($provider); 
    $org_info = cz_oauthconnector_endpoint_call(
          'https://api.linkedin.com/v1/people/~/connections',
          array('format'=>'json'),
          $provider,
          $ltoken,
          $method
      ); 

      $ic = 0; 
       // http://developer.linkedin.com/documents/reading-data#toggleview:id=php  
      $ldata = drupal_json_decode($org_info); 
	  //print_r($ldata);exit;
      if( $ldata && $ldata['_total'] ) {         

        if( is_array($ldata['values']) && count($ldata['values']) > 0 ) {
          foreach($ldata['values'] as $udata ) { 

            if($udata) {
              
              //print_r($udata); // break;  
              $providername = 'linkedin'; 
              $created = time(); 
              $uid = $user->uid; 
              
              $firstname  =  $udata['firstName']; 
              $lastname = $udata['lastName']; 
              $fullname = $udata['firstName'].' '.$udata['lastName'];  
              $username = isset($udata['id']) ? $udata['id'] : '';   
              $email = ''; 
              $address = isset($udata['location']['name']) ? $udata['location']['name'] : ''; 
              $country = isset($udata['location']['country']['code']) ? $udata['location']['country']['code'] : ''; 
              $organization = ''; 
              $phone = ''; 
              $picture = isset($udata['pictureUrl']) ? $udata['pictureUrl'] : ''; 
              $profileid = isset($udata['id']) ? $udata['id'] : ''; 
              $profileurl = isset($udata['siteStandardProfileRequest']['url']) ? $udata['siteStandardProfileRequest']['url'] : ''; 
              //isset($udata['apiStandardProfileRequest']['url']) ? $udata['apiStandardProfileRequest']['url'] : (); 
              $gender = ''; 
              $industry = isset($udata['industry']) ? $udata['industry'] : ''; 
              $headline = isset($udata['headline']) ? $udata['headline'] : ''; 
              
              // check if profileid and provider doesnt exist 
              if( (bool) db_select('cz_card')->fields('cz_card', array('id'))->condition('profileid', $udata['id'], '=')->condition('provider','linkedin', '=')->range(0, 1)->execute()->fetchField() ) { 
                // profile already exist.. 
                
              } else { 
                
                $rs = db_query("insert into {cz_card} ( `firstname`, `lastname`, `fullname`, `username`, `email`, `address`, `country`, `organization`, `phone`, `picture`, `profileid`, `profileurl`, `gender`, `industry`, `headline`, `created`, provider, uid) VALUES (:firstname, :lastname, :fullname, :username, :email, :address, :country, :organization, :phone, :picture, :profileid, :profileurl, :gender, :industry, :headline, :created, :provider, :uid)  ", array(':firstname'=>$firstname, ':lastname'=>$lastname, ':fullname'=>$fullname, ':username'=>$username, ':email'=>$email, ':address'=>$address, ':country'=>$country, ':organization'=>$organization, ':phone'=>$phone, ':picture'=>$picture, ':profileid'=>$profileid, ':profileurl'=>$profileurl, ':gender'=>$gender, ':industry'=>$industry, ':headline'=>$headline, ':created'=>$created, ':provider'=>$providername, ':uid'=>$uid) ); 
                if($rs) {
                  $ic++; 
                } 
              }
          } // end udata

          } 
        }
      }
      
      $out .= $ic.' unique contacts imported.' ; 

    } 

    $out .= '<br/>'.l('View Imported Contacts', 'user/'.$user->uid.'/icontacts'); 

    return $out; 

}


function cz_custom_import_from_google() {
   global $user;

    $out = '';
     // delete existing of this user .. 
    db_query("delete from {cz_card} where uid = :uid and provider = :provider", array(':provider'=>'google', ':uid' => $user->uid)); 

    $out .= ''; 
    $provider = oauthconnector_provider_load('google');
    $csid = $provider->csid;

     $result = db_query("SELECT * FROM {oauth_common_token} WHERE uid = :uid AND type = :type and csid = :csid", array(
        ':uid' => $user->uid,
        ':type' => OAUTH_COMMON_TOKEN_TYPE_ACCESS,
        ':csid' => $provider->csid,
      )); 

      $tokens = array(); 
      while ($token = DrupalOAuthToken::fromResult($result)) {
        $tokens[] = $token;
      }
     
    // echo '<pre>'; 
    // print_r($tokens); 
    $ltoken = $tokens[0]; // print_r( $ltoken); 
    if($ltoken)  { 

     $consumer = $ltoken->consumer;
    
    $method = 'GET'; 

    //print_r($provider); 
    $org_info = cz_oauthconnector_endpoint_call(
          'https://www.google.com/m8/feeds/contacts/default/full',
          array('alt'=>'json'),
          $provider,
          $ltoken,
          $method,
      'json'
      ); 
      
      // http://developer.linkedin.com/documents/reading-data#toggleview:id=php
      /* print_r($org_info); exit; 
      //https://www.google.com/m8/feeds/contacts/{userEmail}/full/{contactId} 

      $doc = new DOMDocument;
      $doc->recover = true;
      $doc->loadXML($org_info);

      $xpath = new DOMXPath($doc);
      $xpath->registerNamespace('gd', 'http://schemas.google.com/g/2005');
      
      $emails = $xpath->query('//gd:email');
      */

      //echo  $ltoken->key;
      //print_r($org_info); exit;
      // drupal_realpath('public://...') 
      $gi_dir = variable_get('file_public_path', conf_path() . '/files').'/google'; 
      
      if (!file_prepare_directory($gi_dir)) {
        mkdir($gi_dir, 0775, FALSE);
      } 
      
      $carray = array(); 
      $ic = 0; 
      if( is_array($org_info['feed']['entry']) && count($org_info['feed']['entry']) > 0 ) { 
       foreach ( $org_info['feed']['entry'] as $entry ) { 
        
        // $cemail =  $email->getAttribute('address');
        $gc_id = $entry['id']['$t']; 
        
        $imglink = ''; $gcid = ''; 
        
         //echo $imglink =  $entry['link'][0]['href'].'?access_token='.$ltoken->key;    
        foreach($entry['link'] as $gclink) {
          
          if($gclink['rel'] == 'http://schemas.google.com/contacts/2008/rel#photo'){ 
            $imgurl = $gclink['href'];  
            $imgurla = explode('/', $imgurl); 
            $imgurlalen = count($imgurla) - 1; 
            $gcid = $imgurla[$imgurlalen];  
            
            $cache_path = $gi_dir.'/'.$gcid.'.jpg';
            
            $imggurl = $gclink['href'].'?access_token='.$ltoken->key; 
            $imgdresult = drupal_http_request($imggurl);
            $rcode = floor($imgdresult->code / 100) * 100;
            $img_types  = array('image/jpeg', 'image/png', 'image/gif');
            if ($imgdresult->data && $rcode != 400 && $rcode != 500 ) { // in_array($imgdresult->Content-Type, $img_types) 
              $imglink_r = file_save_data($imgdresult->data, 'public://google/'.$gcid.'.jpg');
              if ($imglink_r) { 
                $imglink = file_create_url(file_build_uri('google/'.$gcid.'.jpg'));
                 //drupal_realpath('public://...');  
                // file_create_url(file_build_uri('images/example.jpeg')) 

              } 
            } 
            
          } 
        } 
        
        // To get the title.
        // This could also be done using XPath.
        // You can also use ->nodeValue instead of ->textContent.
         $fname = $entry['title']['$t']; //$email->parentNode->getElementsByTagName('title')->item(0)->textContent;
         $cemail = $entry['gd$email'][0]['address']; 
         
         if($cemail) { 
          
          //print_r($udata); // break;  
          $providername = 'google'; 
          $created = time(); 
          $uid = $user->uid; 
          
          $firstname  =  $fname; 
          $lastname = ''; 
          $fullname = $fname;  
          $username = '';  
          $email = $cemail; 
          $address = isset($entry['gd$postalAddress'][0]['$t']) ? $entry['gd$postalAddress'][0]['$t'] : '';  
          $country = ''; 
          $organization = ''; $headline = ''; 
          if( isset($entry['gd$organization']) && is_array($entry['gd$organization']) && count($entry['gd$organization']) > 0 ) { 
            $organization = isset($entry['gd$organization'][0]['gd$orgName']['$t']) ? $entry['gd$organization'][0]['gd$orgName']['$t'] : ''; 

            $headline = isset($entry['gd$organization'][0]['gd$orgTitle']['$t']) ? $entry['gd$organization'][0]['gd$orgTitle']['$t'] : ''; 
          } 

          $phone = isset($entry['gd$phoneNumber'][0]['$t']) ? $entry['gd$phoneNumber'][0]['$t'] : ''; 
          $picture = $imglink; 
          $profileid = $gcid; 
          $profileurl = '';  
          $gender = ''; 
          $industry = ''; 
          
          // check if profileid and provider doesnt exist 
          if( (bool) db_select('cz_card')->fields('cz_card', array('id'))->condition('uid', $user->uid, '=')->condition('profileid', $cemail, '=')->condition('provider','google', '=')->range(0, 1)->execute()->fetchField() ) { 
            //include uid as well ..  
            // profile already exist.. 

          } else { 
            
            $rs = db_query("insert into {cz_card} ( `firstname`, `lastname`, `fullname`, `username`, `email`, `address`, `country`, `organization`, `phone`, `picture`, `profileid`, `profileurl`, `gender`, `industry`, `headline`, `created`, provider, uid) VALUES (:firstname, :lastname, :fullname, :username, :email, :address, :country, :organization, :phone, :picture, :profileid, :profileurl, :gender, :industry, :headline, :created, :provider, :uid)  ", array(':firstname'=>$firstname, ':lastname'=>$lastname, ':fullname'=>$fullname, ':username'=>$username, ':email'=>$email, ':address'=>$address, ':country'=>$country, ':organization'=>$organization, ':phone'=>$phone, ':picture'=>$picture, ':profileid'=>$profileid, ':profileurl'=>$profileurl, ':gender'=>$gender, ':industry'=>$industry, ':headline'=>$headline, ':created'=>$created, ':provider'=>$providername, ':uid'=>$uid) ); 

            if($rs) {
              $ic++; 
            } 
          }
          } // end udata 

      } 
      }

      $out .= $ic.' unique contacts imported.' ; 
      
      //print_r($carray);  

      // Call the API with the parameter "alt=json" You will get results in Json format 

      // $dom = new DomDocument();
      // $dom->loadXML($org_info); 
      // echo $dom->saveXML(); 
      
    } else {
      $out .= 'Account is not connnected..';
    } 
  

  $out .= '<br/>'.l('View Imported Contacts', 'user/'.$user->uid.'/icontacts'); 
 
 return $out; 

} 