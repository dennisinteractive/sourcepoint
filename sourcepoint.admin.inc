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
    '#description' => t('Global kill switch.'),
  );
  $form['sourcepoint_account_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Sourcepoint Account ID'),
    '#default_value' => variable_get('sourcepoint_account_id', ''),
    '#size' => 20,
    '#required' => TRUE,
  );
  $form['sourcepoint_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Sourcepoint Api Key'),
    '#default_value' => variable_get('sourcepoint_api_key', ''),
    '#size' => 50,
    '#required' => TRUE,
  );
  $form['sourcepoint_content_control_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Content Control landing page'),
    '#default_value' => variable_get('sourcepoint_content_control_url', ''),
    '#size' => 50,
    '#description' => t('The Url of the landing page. i.e. http://www.example.com/page'),
    '#required' => TRUE,
  );

  // Bootstrap script options.
  $bootstrap_script_options = _sourcepoint_get_script_options('bootstrap', $form_state);
  $form['bootstrap_script'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bootstrap script'),
  );
  $form['bootstrap_script']['sourcepoint_bootstrap_script_version'] = array(
    '#title' => t('Version'),
    '#type' => 'textfield',
    '#default_value' => $bootstrap_script_options['v'],
    '#size' => 10,
    '#description' => t('Bootstrap script version.'),
    '#required' => TRUE,
  );
  $form['bootstrap_script']['bootstrap_script_status'] = array(
    '#type' => 'fieldset',
    '#title' => _sourcepoint_get_script_status('bootstrap', $form_state),
    '#prefix' => '<div id="bootstrap-script">',
    '#suffix' => '</div>',
  );
  $form['bootstrap_script']['bootstrap_script_status']['update_bootstrap_script'] = array(
    '#type' => 'submit',
    '#name' => 'update_bootstrap_script',
    '#value' => t('Update'),
    '#description' => t('When you click update, the script is retrieved from Sourcepoint servers and stored locally.'),
    '#ajax' => array(
      'callback' => 'sourcepoint_update_bootstrap_script_callback',
      'wrapper' => 'bootstrap-script',
      'method' => 'html',
    ),
  );

  // Recovery script options.
  $recovery_script_options = _sourcepoint_get_script_options('recovery', $form_state);
  $form['recovery_script'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recovery script'),
  );
  $form['recovery_script']['sourcepoint_pub_base'] = array(
    '#title' => t('Pub url'),
    '#type' => 'textfield',
    '#default_value' => $recovery_script_options['pub_base'],
    '#size' => 50,
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
    '#type' => 'fieldset',
    '#title' => _sourcepoint_get_script_status('recovery', $form_state),
    '#prefix' => '<div id="recovery-script">',
    '#suffix' => '</div>',
  );
  $form['recovery_script']['recovery_script_status']['update_recovery_script'] = array(
    '#type' => 'submit',
    '#name' => 'update_recovery_script',
    '#value' => t('Update'),
    '#description' => t('When you click update, the script is retrieved from Sourcepoint servers and stored locally.'),
    '#ajax' => array(
      'callback' => 'sourcepoint_update_recovery_script_callback',
      'wrapper' => 'recovery-script',
      'method' => 'html',
    ),
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form_validate().
 */
function sourcepoint_admin_settings_form_validate($form, &$form_state) {
  // If the script hasn't been retrieved using Update button, do it now.
  if (!sourcepoint_get_script('recovery', $form_state)) {
    _sourcepoint_update_script('recovery', $form_state);
  }
  // If we still don't have a script, display error message.
  if (!sourcepoint_get_script('recovery', $form_state)) {
    form_set_error('recovery_script_status', t('Recovery script not available.'));
  }

  // If the script hasn't been retrieved using Update button, do it now.
  if (!sourcepoint_get_script('bootstrap', $form_state)) {
    _sourcepoint_update_script('bootstrap', $form_state);
  }
  // If we still don't have a script, display error message.
  if (!sourcepoint_get_script('bootstrap', $form_state)) {
    form_set_error('bootstrap_script_status', t('Bootstrap script not available.'));
  }
}

/**
 * Callback for Bootstrap script update button.
 *
 * @param $form
 * @param $form_state
 *
 * @return string
 */
function sourcepoint_update_bootstrap_script_callback($form, $form_state) {
  $script_type = 'bootstrap';
  $message = _sourcepoint_update_script($script_type, $form_state);
  $form['bootstrap_script']['bootstrap_script_status']['#title'] = $message;

  return $form['bootstrap_script']['bootstrap_script_status'];
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
  $script_type = 'recovery';
  $message = _sourcepoint_update_script($script_type, $form_state);
  $form['recovery_script']['recovery_script_status']['#title'] = $message;

  return $form['recovery_script']['recovery_script_status'];
}

/**
 * Helper to do the actual call to the server and store the scripts.
 *
 * @param $form_state
 * @param $script_type
 *
 * @return string
 */
function _sourcepoint_update_script($script_type, $form_state) {
  $options = _sourcepoint_get_script_options($script_type, $form_state);
  $api_key = $form_state['input']['sourcepoint_api_key'];
  variable_del('sourcepoint_' . $script_type . '_script');

  try {
    $script = sourcepoint_fetch_script($script_type, $api_key, $options);
    if (isset($script)) {
      // Create key to store in settings.
      $key = _sourcepoint_build_script_key($options);
      variable_set('sourcepoint_' . $script_type . '_script', [$key => $script]);

      // Update status.
      return _sourcepoint_get_script_status($script_type, $form_state);
    }
  } catch (Exception $e) {
    // Update status with a custom message;
    return _sourcepoint_set_message($e->getMessage(), 'error');
  }

  // Update status with a custom message;
  return _sourcepoint_set_message(t('Error fetching script.'), 'error');
}

/**
 * Helper to create the Html message status.
 *
 * @param array $form_state
 * @param null  $script_type
 *
 * @return string
 */
function _sourcepoint_get_script_status($script_type, $form_state) {
  if (sourcepoint_get_script($script_type, $form_state)) {
    return _sourcepoint_set_message('Script is stored locally.', 'ok');
  }
  else {
    return _sourcepoint_set_message('No script available for the selected options, click Update.', 'error');
  }
}
