<?php
/**
 * @file
 * Default template for dfp tags.
 */
?>
<div <?php print drupal_attributes($placeholder_attributes) ?>>
  <?php if (isset($slug)):
    print drupal_render($slug);
  endif; ?>
  <script type="text/javascript"<?php print !empty($sourcepoint_attributes) ? drupal_attributes($sourcepoint_attributes) : ''; ?>>
    googletag.cmd.push(function() {
      googletag.display("<?php print $tag->placeholder_id ?>");
    });
  </script>
</div>
