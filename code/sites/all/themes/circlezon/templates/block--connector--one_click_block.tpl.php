<?php $tag = $block->subject ? 'section' : 'div'; ?>
<<?php print $tag; ?><?php print $attributes; ?>>
  <div class="block-inner clearfix">
    <?php print render($title_prefix); ?>
    <?php if ($block->subject): ?>
      <h2<?php print $title_attributes; ?>><?php print $block->subject; ?></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    
    <div<?php print $content_attributes; ?>>
      <div style="text-align:center;">
      <?php if( arg(0) == 'user' && arg(1) == 'login' )  {  ?>
       <div> ------------- OR ------------- </div>
       <div style="">use your account from <br/><br/></div>
       <?php } ?>
       <?php if( arg(0) == 'user' && arg(1) == 'register' )  {  ?>
       <div> ------------- OR ------------- </div>
       <div style="">use your account from <br/><br/></div>
       <?php } ?>

      <?php print $content ?>
      </div>
    </div>
  </div>
</<?php print $tag; ?>>
