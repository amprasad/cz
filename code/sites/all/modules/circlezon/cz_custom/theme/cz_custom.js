jQuery(document).ready(function () {
  jQuery(".card_front").bind("click",function(e){
      
	  e.preventDefault(); 
      
      chtml = jQuery(this).next().html(); 
      jQuery(this).next().html(jQuery(this).html()); 
      cdirection = "LEFT"; 
      
      if(jQuery(this).attr('dirc') != '' ) {
        cdirection = jQuery(this).attr('dirc'); 
      }
      if(cdirection == 'RIGHT') { 
        jQuery(this).attr('dirc', "LEFT"); 
      } else {
        jQuery(this).attr('dirc', "RIGHT"); 
      } 

      jQuery(this).flippy({
        color_target:"#4484D4",
        content: chtml,
        direction: cdirection,
        duration:"300", 
        onStart:function(){
          
        },
        onFinish:function(){
          
		 }
	  }); 
	});

});
