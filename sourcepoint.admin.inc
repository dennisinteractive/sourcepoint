<?php
/**
 * @file
 * Administrative page callbacks for the Sourcepoint module.
 */

/**
 * Implements hook_admin_settings() for module settings configuration.
 */
function sourcepoint_admin_settings_form($form, &$form_state) {

  // Global options.
  $form['sourcepoint_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => variable_get('sourcepoint_enabled', 1),
    '#description' => 'Global kill switch.',
  );
  $form['sourcepoint_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Sourcepoint Api Key'),
    '#default_value' => variable_get('sourcepoint_api_key', ''),
    '#size' => 70,
    '#required' => TRUE,
  );

  // Recovery script options.
  $recovery_script_options = _sourcepoint_get_recovery_script_options($form_state);
  $form['recovery_script'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recovery script'),
  );
  $form['recovery_script']['sourcepoint_pub_base'] = array(
    '#title' => t('Pub url'),
    '#type' => 'textfield',
    '#default_value' => $recovery_script_options['pub_base'],
    '#size' => 70,
    '#description' => t('Full URL for the pubbase to be included in the returned CS-URL.'),
    '#required' => TRUE,
  );
  $form['recovery_script']['sourcepoint_pub_adserver'] = array(
    '#title' => t('Pub adserver'),
    '#type' => 'select',
    '#options' => array(
      'dfp' => t('DFP'),
      'smart' => t('Smart'),
    ),
    '#default_value' => $recovery_script_options['pub_adserver'],
    '#description' => t('Specifies the publisher adserver support required.'),
    '#required' => TRUE,
  );
  $form['recovery_script']['sourcepoint_env'] = array(
    '#title' => t('Environment'),
    '#type' => 'select',
    '#options' => array(
      'prod' => t('Production'),
      'stage' => t('Staging'),
    ),
    '#default_value' => $recovery_script_options['env'],
    '#description' => t('Production or Staging.'),
    '#required' => TRUE,
  );
  $form['recovery_script']['sourcepoint_recovery_script_format'] = array(
    '#type' => 'select',
    '#title' => t('Script format'),
    '#options' => array(
      'js' => t('Inline'),
      'cdn' => t('CDN'),
    ),
    '#default_value' => $recovery_script_options['fmt'],
    '#required' => TRUE,
    '#description' => t('Format of returned script. JS is raw javascript and CDN is a CDN-based URL.'),
  );
  $form['recovery_script']['recovery_script_status'] = array(
    '#markup' => _sourcepoint_get_recovery_script_status($form_state),
    '#prefix' => '<div id="recovery-script-status-wrapper">',
    '#suffix' => '</div>',
  );
  $form['recovery_script']['update_recovery_script'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#description' => t('When you click update, the script is retrieved from Sourcepoint servers and stored locally.'),
    '#ajax' => array(
      'callback' => 'sourcepoint_update_recovery_script_callback',
      'wrapper' => 'recovery-script-status-wrapper',
      'method' => 'replace',
    ),
  );

  // Bootstrap script options.
  $form['sourcepoint_bootstrap_script'] = array(
    '#type' => 'hidden',
  );
  $form['bootstrap_script']['sourcepoint_bootstrap_version'] = array(
    '#title' => t('Version'),
    '#type' => 'textfield',
    '#default_value' => variable_get('sourcepoint_bootstrap_version', ''),
    '#size' => 6,
    '#description' => t('Bootstrap script version.'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form_validate().
 */
function sourcepoint_admin_settings_form_validate($form, &$form_state) {
  // If the script hasn't been retrieved using Update button, do it now.
  if (!_sourcepoint_recovery_script_available($form_state)) {
    sourcepoint_update_recovery_script_callback($form, $form_state);
  }
  // If we still don't have a script, display error message.
  if (!_sourcepoint_recovery_script_available($form_state)) {
    form_set_error('recovery_script_status', t('Recovery script not available.'));
  }
}

/**
 * Callback for Recovery script update button.
 *
 * @param $form
 * @param $form_state
 *
 * @return string
 */
function sourcepoint_update_recovery_script_callback($form, $form_state) {
  $endpoint = '/cs_recovery';
  $api_key = $form_state['input']['sourcepoint_api_key'];
  $options = _sourcepoint_get_recovery_script_options($form_state);

  try {
    $sourcePoint = new SourcePointAsset($endpoint, $api_key);
    $sourcePoint->setOptions($options);
    $response = $sourcePoint->fetch();

    if ($script = $response->script) {
      // Create key to store in settings.
      $key = _sourcepoint_get_recovery_script_key($options);
      variable_set('sourcepoint_recovery_script', [$key => $script]);

      // Update status.
      return _sourcepoint_get_recovery_script_status($form_state);
    }
  } catch (Exception $e) {
    // Update status with a custom message;
    return _sourcepoint_get_recovery_script_status($form_state, $e->getMessage(), 'error');
  }

  // Update status with a custom message;
  return _sourcepoint_get_recovery_script_status($form_state, t('Error fetching script.'), 'error');
}

/**
 * Helper to build the key used to store the script in the variables.
 *
 * @param $options
 *
 * @return string
 */
function _sourcepoint_get_recovery_script_key($options) {
  return implode('_', array_values($options));
}

/**
 * Helper to return the array of options and values for the Recovery script.
 *
 * @param array $form_state If this argument is passed, the values will be
 * retrieved from it. Default is to retrieve from settings.
 *
 * @return array
 */
function _sourcepoint_get_recovery_script_options($form_state = []) {
  if (isset($form_state['input']['sourcepoint_pub_base'])) {
    $pub_base = $form_state['input']['sourcepoint_pub_base'];
  }
  else {
    global $base_url;
    $pub_base = trim(variable_get('sourcepoint_pub_base', $base_url . '/pub/base'), '/');
  }

  if (isset($form_state['input']['sourcepoint_pub_adserver'])) {
    $pub_adserver = $form_state['input']['sourcepoint_pub_adserver'];
  }
  else {
    $pub_adserver = variable_get('sourcepoint_pub_adserver', 'dfp');
  }

  if (isset($form_state['input']['sourcepoint_env'])) {
    $env = $form_state['input']['sourcepoint_env'];
  }
  else {
    $env = variable_get('sourcepoint_env', 'prod');
  }

  if (isset($form_state['input']['sourcepoint_recovery_script_format'])) {
    $fmt = $form_state['input']['sourcepoint_recovery_script_format'];
  }
  else {
    $fmt = variable_get('sourcepoint_recovery_script_format', 'cdn');
  }

  return [
    'pub_base' => $pub_base,
    'pub_adserver' => $pub_adserver,
    'env' => $env,
    'fmt' => $fmt,
  ];
}

/**
 * Helper to check if the script for the selected options is available locally.
 *
 * @param array $form_state If this argument is passed, the values will be retrieved
 * from it. Default is to retrieve from settings.
 *
 * @return bool
 */
function _sourcepoint_recovery_script_available($form_state = []) {
  $script_value = variable_get('sourcepoint_recovery_script');
  $options = _sourcepoint_get_recovery_script_options($form_state);
  $key = _sourcepoint_get_recovery_script_key($options);
  if (isset($script_value[$key])) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Helper to create the Html message status.
 *
 * @param array  $form_state
 * @param null   $custom_message
 * @param string $class
 *
 * @return string
 */
function _sourcepoint_get_recovery_script_status($form_state = [], $custom_message = NULL, $class = 'error') {
  if (!is_null($custom_message)) {
    $message = $custom_message;
  }
  else {
    if (_sourcepoint_recovery_script_available($form_state)) {
      $class = 'ok';
      $message = t('Script is stored locally.');
    }
    else {
      $class = 'error';
      $message = t('No script available for the selected options, click Update.');
    }
  }

  return '<div class="' . $class . '">' . $message . '</div>';
}
