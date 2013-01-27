<?php 
/**
 * @file
 * Alpha's theme implementation to display a single Drupal page.
 */
?>
<div<?php print $attributes; ?>>
  <?php if (isset($page['header'])) : ?>
    <?php print render($page['header']); ?>
  <?php endif; ?>
  
  <?php if (isset($page['content'])) : ?>
    <?php //print render($page['content']); ?>
  <?php endif; ?>  
  <div style="margin:auto;width:965px;"><img src="/drupal/sites/all/themes/circlezon/images/frontimg.png"/></div>
  

  <div style="margin:auto;width:965px;"><img src="/drupal/sites/all/themes/circlezon/images/front-footer.png"/></div>
  
  <?php if (isset($page['footer'])) : ?>
    <?php // print render($page['footer']); ?>
  <?php endif; ?>
</div>
