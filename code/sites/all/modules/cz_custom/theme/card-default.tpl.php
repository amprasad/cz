<div class="contact_grid_container">
	<?php foreach ($data as $account) {
  	$pic =  $account->picture;
  	if($pic == NULL || $pic == '') {
    	$image = '<img src ="' . base_path() . 'sites/all/themes/circlezon/images/profile_placeholder.png'.'"/>'; 
  	} else {
      $image = '<img class = "contact_picture" src="'. $pic .'" />'; 
    } ?>
		<div class="contact_container">
    	<div class="contact_data">
    		<?php print $image ?>
    	</div>
      <div class="contact_data">
      	<a href="<?php print $account->profileurl; ?>" target="_blank"><?php print $account->fullname; ?></a>
      </div>
     	<div class="contact_data contact_data_bold">
     		<?php print $account->headline; ?>
     	</div>
      <div class="contact_data contact_data_bold">
      	<?php $account->industry; ?>
      </div>
      <div class="contact_data">
      	<?php $account->address; ?>
      </div>
      <div class="widget-favorite">
      	<?php print widgets_get_favorite_widget($account->id, $account->is_favorite); ?>
      </div>
    </div>
  <?php } ?> 
</div>