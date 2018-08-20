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

  // Consent Management Platform.
  $form['cmp'] = array(
    '#type' => 'fieldset',
    '#title' => t('Consent Management Platform'),
  );
  $form['cmp']['sourcepoint_cmp_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('CMP Enabled'),
    '#default_value' => variable_get('sourcepoint_cmp_enabled', 1),
    '#description' => t('Enable Consent Management Platform.'),
  );
  $form['cmp']['sourcepoint_cmp_site_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Site ID'),
    '#default_value' => variable_get('sourcepoint_cmp_site_id', ''),
    '#size' => 50,
    '#description' => t('Site ID for privacy manager.'),
    '#element_validate' => array('element_validate_integer_positive')
  );
  $form['cmp']['sourcepoint_cmp_privacy_manager_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Privacy Manager ID'),
    '#default_value' => variable_get('sourcepoint_cmp_privacy_manager_id', ''),
    '#size' => 50,
    '#description' => t('Privacy Manager ID.'),
  );
  $form['cmp']['sourcepoint_cmp_overlay_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Overlay Height'),
    '#default_value' => variable_get('sourcepoint_cmp_overlay_height', '600px'),
    '#size' => 50,
    '#description' => t('Height of the overlay iframe.'),
  );
  $form['cmp']['sourcepoint_cmp_overlay_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Overlay Width'),
    '#default_value' => variable_get('sourcepoint_cmp_overlay_width', '600px'),
    '#size' => 50,
    '#description' => t('Width of the overlay iframe.'),
  );

  // Privacy Preferences Link
  $form['cmp']['sourcepoint_cmp_link_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('CMP Link Enabled'),
    '#default_value' => variable_get('sourcepoint_cmp_link_enabled', 0),
    '#description' => t('Enable a menu link for the Consent Management Platform pop-up.'),
  );
  $form['cmp']['sourcepoint_cmp_link_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Link Title'),
    '#default_value' => variable_get('sourcepoint_cmp_link_title', ''),
    '#size' => 50,
    '#description' => t('Define the title of the link.'),
  );
  $form['cmp']['sourcepoint_cmp_link_menu_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu'),
    '#default_value' => variable_get('sourcepoint_cmp_link_menu_name', ''),
    '#size' => 50,
    '#description' => t('The menu to add the link to.'),
  );
  $form['cmp']['sourcepoint_cmp_link_weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu Link Weight'),
    '#default_value' => variable_get('sourcepoint_cmp_link_weight', 0),
    '#size' => 50,
    '#description' => t('Weight to determine position of the link.'),
  );

  // Overlay iframe.
  if ($url = sourcepoint_get_privacy_manager_url()) {
    $form['cmp']['sourcepoint_cmp_overlay_demo_url'] = array(
      '#markup' => t('<h2>Privacy Manager</h2>URL: !url <br />Add <em>.sourcepoint-cmp-overlay</em> class to links to open overlay.', array(
        '!url' => l($url, $url, array('attributes' => array('class' => 'sourcepoint-cmp-overlay'))),
      )),
    );
    $form['cmp']['sourcepoint_cmp_overlay_demo'] = sourcepoint_get_privacy_manager_overlay();
  }

  // Detection Timeout Management.
  $form['detection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Detection Timeout Management'),
  );

  $form['detection']['sourcepoint_dtm_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => variable_get('sourcepoint_dtm_enabled', 1),
    '#description' => t('Enable detection timeout.'),
  );
  $form['detection']['sourcepoint_dtm_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout'),
    '#default_value' => variable_get('sourcepoint_dtm_timeout', 500),
    '#description' => t('Detection timeout in milliseconds.'),
    '#element_validate' => array('element_validate_integer_positive')
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
