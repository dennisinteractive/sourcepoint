<?php
/**
 * @file
 * Hooks provided by the Sourcepoint module.
 */

/**
 * Modify JS config.
 *
 * @param array $config
 */
function hook_sourcepoint_js_config_alter(&$config) {
  $config['string_value'] = "'hello world'";
  $config['function_value'] = "function() { return true; }";
  $config['integer_value'] = "123";
}

/**
 * Provide an ad server.
 *
 * @return array
 */
function hook_sourcepoint_ad_servers() {
  return array(
    'smart' => t('Smart'),
  );
}
