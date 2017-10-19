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

  // Style Manager
  $form['style_manager'] = array(
    '#type' => 'fieldset',
    '#title' => t('Style Manager'),
  );
  $form['style_manager']['sourcepoint_style_manager'] = array(
    '#type' => 'checkbox',
    '#title' => t('Style Manager Enabled'),
    '#default_value' => variable_get('sourcepoint_style_manager', 1),
    '#description' => t('Enable Style Manager'),
  );

  return system_settings_form($form);
}
