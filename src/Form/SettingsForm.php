<?php

namespace Drupal\sourcepoint\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Provide settings for Sourcepoint.
 */
class SettingsForm extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'sourcepoint_settings';
  }

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return [
      'sourcepoint.settings',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state, $field_type = NULL) {
    $config = $this->config('sourcepoint.settings');

    $form['account_id'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Account ID'),
      '#default_value' => $config->get('account_id'),
      '#required' => TRUE,
    ];

    $form['enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => $config->get('enabled'),
    );

    // Content Control.
    $form['content_control'] = array(
      '#type' => 'fieldset',
      '#title' => t('Content Control'),
    );
    $form['content_control']['rid_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Recovery Interference Detection'),
      '#default_value' => $config->get('rid_enabled'),
    );
    $form['content_control']['content_control_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Content Control landing page'),
      '#default_value' => $config->get('content_control_url'),
      '#size' => 50,
      '#description' => t('The Url of the landing page. i.e. http://www.example.com/page'),
    );

    // Consent Management Platform.
    $form['cmp'] = array(
      '#type' => 'fieldset',
      '#title' => t('Consent Management Platform'),
    );
    $form['cmp']['cmp_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Consent Management Platform'),
      '#default_value' => $config->get('cmp_enabled'),
    );
    $form['cmp']['cmp_site_id'] = array(
      '#type' => 'number',
      '#title' => t('Site ID'),
      '#default_value' => $config->get('cmp_site_id'),
      '#size' => 50,
      '#description' => t('Site ID for privacy manager.'),
      '#min' => 0,
    );
    $form['cmp']['cmp_privacy_manager_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Privacy Manager ID'),
      '#default_value' => $config->get('cmp_privacy_manager_id'),
      '#size' => 50,
      '#description' => t('Privacy Manager ID.'),
    );
    $form['cmp']['cmp_overlay_height'] = array(
      '#type' => 'textfield',
      '#title' => t('Overlay Height'),
      '#default_value' => $config->get('cmp_overlay_height'),
      '#size' => 50,
      '#description' => t('Height of the overlay iframe.'),
    );
    $form['cmp']['cmp_overlay_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Overlay Width'),
      '#default_value' => $config->get('cmp_overlay_width'),
      '#size' => 50,
      '#description' => t('Width of the overlay iframe.'),
    );

    // Overlay iframe.
    if ($url = sourcepoint_get_privacy_manager_url()) {
      $messages = array();
      $messages[] = t('<h2>Privacy Manager</h2>URL: !url', array(
        '!url' => l($url, $url, array('attributes' => array('class' => 'sourcepoint-cmp-overlay'))),
      ));
      $messages[] = t('Add :class class to links to open overlay.', array(':class' => '.sourcepoint-cmp-overlay'));
      $messages[] = t('Use :menu_link to open the overlay from menu items.', array(':menu_link' => '<sourcepoint_cmp_overlay>'));

      $form['cmp']['cmp_overlay_demo_url'] = array(
        '#theme' => 'item_list',
        '#items' => $messages,
      );
      $form['cmp']['cmp_overlay_demo'] = sourcepoint_get_privacy_manager_overlay();
    }

    // Detection Timeout Management.
    $form['detection'] = array(
      '#type' => 'fieldset',
      '#title' => t('Detection Timeout Management'),
    );

    $form['detection']['dtm_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable detection timeout'),
      '#default_value' => $config->get('dtm_enabled'),
    );
    $form['detection']['dtm_timeout'] = array(
      '#type' => 'number',
      '#title' => t('Timeout'),
      '#default_value' => $config->get('dtm_timeout'),
      '#description' => t('Detection timeout in milliseconds.'),
      '#min' => 0,
    );

    // Style Manager
    $form['style_manager'] = array(
      '#type' => 'fieldset',
      '#title' => t('Style Manager'),
    );
    $form['style_manager']['style_manager'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Style Manager'),
      '#default_value' => $config->get('style_manager'),
    );

    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $config = $this->config('sourcepoint.settings');

    $keys = [
      'account_id',
      'enabled',
      'rid_enabled',
      'content_control_url',
      'cmp_enabled',
      'cmp_site_id',
      'cmp_privacy_manager_id',
      'cmp_overlay_height',
      'cmp_overlay_width',
      'dtm_enabled',
      'dtm_timeout',
      'style_manager',
    ];
    foreach ($keys as $key) {
      $config->set($key, $form_state->getValue($key));
    }
    $config->save();

    parent::submitForm($form, $form_state);
  }

}
