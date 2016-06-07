<?php
/**
 * @file
 * Administrative page callbacks for the Sourcepoint module.
 */

/**
 * Implements hook_admin_settings() for module settings configuration.
 */
function sourcepoint_admin_settings_form($form_state) {

  $form['sourcepoint_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => variable_get('sourcepoint_enabled', 1),
    '#description' => 'Global kill switch.',
  );

  $form['sourcepoint_api_key'] = array(
    '#title' => t('Sourcepoint Api Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('sourcepoint_api_key', ''),
    '#size' => 70,
    '#required' => TRUE,
  );

  $form['sourcepoint_recovery_script'] = array(
    '#type' => 'hidden',
  );
  $form['recovery_script'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recovery script'),
  );
  $form['recovery_script']['sourcepoint_pub_base'] = array(
    '#title' => t('Pub url'),
    '#type' => 'textfield',
    '#default_value' => _sourcepoint_get_pub_base(),
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
    '#default_value' => variable_get('sourcepoint_pub_adserver', 'dfp'),
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
    '#default_value' => variable_get('sourcepoint_env', 'prod'),
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
    '#default_value' => variable_get('sourcepoint_recovery_script_format', ''),
    '#required' => TRUE,
    '#description' => t('Format of returned script. JS is raw javascript and CDN is a CDN-based URL.'),
  );
  $form['recovery_script']['recovery_script_status'] = array(
    '#markup' => '<div id="recovery-script-status-wrapper"></div>',
  );
  $form['recovery_script']['update_recovery_script'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#submit' => array('sourcepoint_update_recovery_script_button'),
    // See the examples in ajax_example.module for more details on the
    // properties of #ajax.
    '#ajax' => array(
      'callback' => 'sourcepoint_update_recovery_script_callback',
      'wrapper' => 'recovery-script-status-wrapper',
    ),
  );

  $form['sourcepoint_bootstrap_script'] = array(
    '#type' => 'hidden',
  );
  $form['bootstrap_script'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bootstrap script'),
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
  /*
  $api_key = $form_state['input']['sourcepoint_api_key'];
  $method = $form_state['input']['sourcepoint_recovery_script_format'];

  try {
    $data = sourcepoint_request_sourcepoint_script($api_key, $method);
    form_set_value($form['sourcepoint_recovery_script'], $data->script, $form_state);
  } catch (Exception $e) {
    form_set_error('sourcepoint_api_key', t('Invalid sourcepoint data.'));
  }*/
}

function sourcepoint_update_recovery_script_button($form, $form_state) {
  //return 'updated';
}
function sourcepoint_update_recovery_script_callback($form, $form_state) {
  $webhook = '/cs_recovery';
  $api_key = $form_state['input']['sourcepoint_api_key'];
  $options = array(
    'fmt' => $form_state['input']['sourcepoint_recovery_script_format'],
    'pub_base' => $form_state['input']['sourcepoint_pub_base'],
    'pub_adserver' => $form_state['input']['sourcepoint_pub_adserver'],
    'env' => $form_state['input']['sourcepoint_env'],
  );

  try {
    $response = sourcepoint_request_sourcepoint_script($webhook, $api_key, $options);
    form_set_value($form['sourcepoint_recovery_script'], $response->script, $form_state);
    return '<div id="recovery-script-status-wrapper ok">Script updated.</div>';
  } catch (Exception $e) {
    return '<div class="recovery-script-status-wrapper error">' . $e->getMessage() . '</div>';
  }
}
