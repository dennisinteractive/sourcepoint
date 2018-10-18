<?php
/**
 * @file
 * Code for the Sourcepoint drush commands.
 */

/**
 * Implements of hook_drush_command().
 */
function sourcepoint_drush_command() {
  $items = array();

  $items['sourcepoint-fetch-script'] = array(
    'callback' => 'sourcepoint_drush_fetch_script',
    'description' => t("Downloads and sets Sourcepoint script."),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'options' => array(
      'sp-type' => t('Script type'),
      'sp-version' => t('Version'),
      'sp-path' => t('Path that script should be served from'),
      'sp-env' => t('Environment e.g. prod'),
      'sp-pub-adserver' => t('Ad Server being used e.g. DFP'),
      'sp-format' => t('Format e.g. js'),
      'sp-pub-page' => t('Publisher base'),
      'sp-include-msg' => t('Include recovery message'),
    ),
    'examples' => array(
      'drush sourcepoint-fs --sp-type="bootstrap" --sp-version="1.7.150"',
      'drush sourcepoint-fs --sp-type="msg" --sp-version="1.7.150" --sp-path="msg.js" ',
      'drush sourcepoint-fs --sp-type="mms_client" --sp-version="1.7.150" --sp-path="mms.js" ',
      'drush sourcepoint-fs --sp-type="recovery" --sp-version="1.7.150" --sp-path="ab.js" --sp-env="prod" --sp-pub-adserver="dfp" --sp-format="js" --sp-pub-page="sub.example.com"',
    ),
    'aliases' => array('sourcepoint-fs'),
  );

  return $items;
}

/**
 * Drush callback to fetch scripts.
 */
function sourcepoint_drush_fetch_script() {
  // Check that API key is set.
  if (!variable_get('sourcepoint_api_key')) {
    return drush_set_error(t('Please set variable sourcepoint_api_key'));
  }

  // Validate types.
  $script_type = drush_get_option('sp-type');
  $types = array('bootstrap', 'msg', 'mms_client', 'recovery');
  if (!in_array($script_type, $types)) {
    return drush_set_error(t('Please specify a valid sp-type. Allowed types: !allowed', array(
      '!type' => $script_type,
      '!allowed' => implode(', ', $types),
    )));
  }

  // Check that path is available for external scripts.
  if (in_array($script_type, array('msg', 'mms_client', 'recovery'))) {
    $path_variable_name = 'sourcepoint_' . $script_type . '_script_path';
    if ($path = drush_get_option('sp-path')) {
      variable_set($path_variable_name, $path);
    }
    elseif (!variable_get($path_variable_name)) {
      return drush_set_error(t('Please specify script sp-path.'));
    }
  }

  // Recovery require extra configuration.
  if ($script_type == 'recovery') {
    $recovery_options = array(
      'env' => 'env',
      'pub-adserver' => 'pub_adserver',
      'format' => 'format',
      'pub-page' => 'pub_base',
      'include-msg' => 'include_msg',
    );
    foreach ($recovery_options as $drush_opt => $variable_name_part) {
      $recovery_variable_name = 'sourcepoint_' . $script_type . '_script_' . $variable_name_part;
      if ($opt_value = drush_get_option('sp-' . $drush_opt)) {
        variable_set($recovery_variable_name, $opt_value);
      }
      elseif (!variable_get($recovery_variable_name)) {
        return drush_set_error(t('Please specify recovery option "!option".', array(
          '!option' => 'sp-' . $drush_opt,
        )));
      }
    }
  }

  // Set script version if specified.
  $version_variable_name = 'sourcepoint_' . $script_type . '_script_version';
  if ($version = drush_get_option('sp-version')) {
    variable_set($version_variable_name, $version);
  }
  elseif (!variable_get($version_variable_name)) {
    return drush_set_error(t('Please specify sp-version'));
  }

  // Fetch script.
  try {
    $script = sourcepoint_fetch_script($script_type);
    // Build options.
    $options = _sourcepoint_get_script_options($script_type);
    $key = _sourcepoint_build_script_key($options);
    // Store.
    variable_set('sourcepoint_' . $script_type . '_script', [$key => $script]);
    drush_log(t('Fetched !script script.', array('!script' => $script_type)), 'ok');
  } catch (Exception $e) {
    return drush_set_error(t('Could not fetch !script script. !exception', array(
      '!script' => $script_type,
      '!exception' => $e->getMessage(),
    )));
  }
}