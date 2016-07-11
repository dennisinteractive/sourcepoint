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

  // Content Control.
  $form['content_control'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content Control'),
  );
  $form['content_control']['sourcepoint_rid_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('RID Enabled'),
    '#default_value' => variable_get('sourcepoint_rid_enabled', 1),
    '#description' => t('Enable Recovery Interference Detection.'),
  );
  $form['content_control']['sourcepoint_content_control_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Content Control landing page'),
    '#default_value' => variable_get('sourcepoint_content_control_url', ''),
    '#size' => 50,
    '#description' => t('The Url of the landing page. i.e. http://www.example.com/page'),
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
      'callback' => 'sourcepoint_update_script_callback',
      'wrapper' => 'bootstrap-script',
      'method' => 'html',
    ),
  );

  // msg script options.
  $msg_script_options = _sourcepoint_get_script_options('msg', $form_state);
  $form['msg_script'] = array(
    '#type' => 'fieldset',
    '#title' => t('msg script'),
  );
  $form['msg_script']['sourcepoint_msg_script_version'] = array(
    '#title' => t('Version'),
    '#type' => 'textfield',
    '#default_value' => $msg_script_options['v'],
    '#size' => 10,
    '#description' => t('msg script version.'),
    '#required' => TRUE,
  );
  $form['msg_script']['sourcepoint_msg_script_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Script path.'),
    '#default_value' => variable_get('sourcepoint_msg_script_path', substr(md5(time()), 0, 10) . '.js'),
    '#size' => 20,
    '#description' => t('The raw content of the msg javascript will be returned from this path.'),
    '#required' => TRUE,
    '#element_validate' => array('sourcepoint_validate_src_path'),
  );
  $form['msg_script']['msg_script_status'] = array(
    '#type' => 'fieldset',
    '#title' => _sourcepoint_get_script_status('msg', $form_state),
    '#prefix' => '<div id="msg-script">',
    '#suffix' => '</div>',
  );
  $form['msg_script']['msg_script_status']['update_msg_script'] = array(
    '#type' => 'submit',
    '#name' => 'update_msg_script',
    '#value' => t('Update'),
    '#description' => t('When you click update, the script is retrieved from Sourcepoint servers and stored locally.'),
    '#ajax' => array(
      'callback' => 'sourcepoint_update_script_callback',
      'wrapper' => 'msg-script',
      'method' => 'html',
    ),
  );

  // mms_client script options.
  $mms_client_script_options = _sourcepoint_get_script_options('mms_client', $form_state);
  $form['mms_client_script'] = array(
    '#type' => 'fieldset',
    '#title' => t('mms_client script'),
  );
  $form['mms_client_script']['sourcepoint_mms_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('MMS Domain'),
    '#default_value' => variable_get('sourcepoint_mms_domain', ''),
    '#size' => 50,
    '#description' => t('The Url of the MMS domain'),
  );
  $form['mms_client_script']['sourcepoint_mms_client_script_version'] = array(
    '#title' => t('Version'),
    '#type' => 'textfield',
    '#default_value' => $mms_client_script_options['v'],
    '#size' => 10,
    '#description' => t('mms_client script version.'),
    '#required' => TRUE,
  );
  $form['mms_client_script']['sourcepoint_mms_client_script_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Script path.'),
    '#default_value' => variable_get('sourcepoint_mms_client_script_path', substr(md5(time()), 0, 10) . '.js'),
    '#size' => 20,
    '#description' => t('The raw content of the mms_client javascript will be returned from this path.'),
    '#required' => TRUE,
    '#element_validate' => array('sourcepoint_form_validate_script', 'sourcepoint_validate_src_path'),
  );
  $form['mms_client_script']['mms_client_script_status'] = array(
    '#type' => 'fieldset',
    '#title' => _sourcepoint_get_script_status('mms_client', $form_state),
    '#prefix' => '<div id="mms_client-script">',
    '#suffix' => '</div>',
  );
  $form['mms_client_script']['mms_client_script_status']['update_mms_client_script'] = array(
    '#type' => 'submit',
    '#name' => 'update_mms_client_script',
    '#value' => t('Update'),
    '#description' => t('When you click update, the script is retrieved from Sourcepoint servers and stored locally.'),
    '#ajax' => array(
      'callback' => 'sourcepoint_update_script_callback',
      'wrapper' => 'mms_client-script',
      'method' => 'html',
    ),
  );

  // Add recovery fields to form.
  $form = sourcepoint_admin_recovery_settings_form($form, $form_state);

  return system_settings_form($form);
}

/**
 * Helper to ad recovery fields to form.
 */
function sourcepoint_admin_recovery_settings_form($form, &$form_state) {
  // Only expose recovery options if an ad server is supported.
  $ad_servers = sourcepoint_get_ad_servers();
  if (empty($ad_servers)) {
    return $form;
  }

  // Recovery script options.
  $recovery_script_options = _sourcepoint_get_script_options('recovery', $form_state);
  $form['recovery_script'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recovery script'),
  );
  $form['recovery_script']['sourcepoint_recovery_script_pub_base'] = array(
    '#title' => t('Pub url'),
    '#type' => 'textfield',
    '#default_value' => $recovery_script_options['pub_base'],
    '#size' => 50,
    '#description' => t('Full URL for the pubbase to be included in the returned CS-URL.'),
    '#required' => TRUE,
  );
  $form['recovery_script']['sourcepoint_recovery_script_pub_adserver'] = array(
    '#title' => t('Pub adserver'),
    '#type' => 'select',
    '#options' => $ad_servers,
    '#default_value' => $recovery_script_options['pub_adserver'],
    '#description' => t('Specifies the publisher adserver support required.'),
    '#required' => TRUE,
  );
  $form['recovery_script']['sourcepoint_recovery_script_env'] = array(
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
      /*'cdn' => t('CDN'), deprecated */
    ),
    '#default_value' => $recovery_script_options['fmt'],
    '#required' => TRUE,
    '#description' => t('Format of returned script. JS is raw javascript and CDN is a CDN-based URL.'),
  );
  $form['recovery_script']['sourcepoint_recovery_script_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Script path.'),
    '#default_value' => variable_get('sourcepoint_recovery_script_path', substr(md5(time()), 0, 10) . '.js'),
    '#size' => 20,
    '#description' => t('The raw content of the recovery javascript will be returned from this path.'),
    '#required' => TRUE,
    '#element_validate' => array('sourcepoint_validate_src_path'),
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
      'callback' => 'sourcepoint_update_script_callback',
      'wrapper' => 'recovery-script',
      'method' => 'html',
    ),
  );
  return $form;
}

/**
 * Helper to validate scripts.
 */
function sourcepoint_form_validate_script($element, &$form_state) {
  $script_types = array(
    'sourcepoint_recvovery_script_path' => 'recovery',
    'sourcepoint_msg_script_path' => 'msg',
    'sourcepoint_mms_client_script_path' => 'mms_client',
  );
  if (!isset($script_types[$element['#name']])) {
    return;
  }

  $script_type = $script_types[$element['#name']];

  // If the script hasn't been retrieved using Update button, do it now.
  if (!sourcepoint_get_script($script_type, $form_state)) {
    _sourcepoint_update_script($script_type, $form_state);
  }
  // If we still don't have a script, display error message.
  if (!sourcepoint_get_script($script_type, $form_state)) {
    form_set_error($script_type . '_script_status', t(':type not available.', array(':type' => $script_type)));
  }
}

/**
 * Helper to validate and sanitize the script path.
 *
 * @param $form
 * @param $form_state
 */
function sourcepoint_validate_src_path($element, &$form_state) {
  // Construct variable name.
  $variable_name = $element['#name'];

  // Sanitize and normalize path.
  $src_path = $element['#value'];
  $src_path = trim($src_path, '/');
  if (strpos($src_path, '.js') !== FALSE) {
    $src_path = str_replace('.js', '', $src_path);
  }
  if (module_exists('pathauto')) {
    module_load_include('inc', 'pathauto');
    if (function_exists('pathauto_cleanstring')) {
      $src_path = pathauto_cleanstring($src_path);
    }
  }
  $src_path = $src_path . '.js';

  // Check that the src path changed and it doesn't exist as a menu item.
  if ($current_src_path = variable_get($variable_name)) {
    if ($current_src_path != $src_path) {
      $src_path_changed = $src_path;
    }
    else {
      $src_path_changed = FALSE;
    }
  }
  else {
    $src_path_changed = $src_path;
  }
  if ($src_path_changed) {
    if (drupal_valid_path($src_path_changed)) {
      // Path already exists.
      form_set_error($variable_name, t('Path already exists.'));
    }
    else {
      // Update form.
      $form_state['values'][$variable_name] = $src_path_changed;

      // Store new path.
      variable_set($variable_name, $src_path_changed);

      // Clear menu cache.
      menu_rebuild();
    }
  }
}

/**
 * Callback for script update button.
 *
 * @param $form
 * @param $form_state
 *
 * @return string
 */
function sourcepoint_update_script_callback($form, $form_state) {
  $script_types = array(
    'update_bootstrap_script' => 'bootstrap',
    'update_recovery_script' => 'recovery',
    'update_msg_script' => 'msg',
    'update_mms_client_script' => 'mms_client',
  );
  if (isset($script_types[$form_state['triggering_element']['#name']])) {
    // Update script and return update message.
    $script_type = $script_types[$form_state['triggering_element']['#name']];
    $status = $form[$script_type . '_script'][$script_type . '_script_status'];
    $status['#title'] = _sourcepoint_update_script($script_type, $form_state);
    return $status;
  }
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
