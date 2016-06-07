<?php

/**
 * Class SourcePointAsset.
 */
class SourcePointAsset {
  /**
   * The Sourcepoint Service URL.
   */
  const SERVICE_URL_BASE = 'https://api.sourcepoint.com/script';

  /**
   * The API Key.
   */
  protected $api_key;

  /**
   * The Service Url.
   */
  protected $serviceUrl;

  /**
   * The script.
   */
  public $script;

  /**
   * Stores the options.
   */
  protected $options;

  /**
   * Initial setup.
   */
  public function __construct($webhook, $api_key, $options) {
    if (empty($webhook) || empty($api_key) || empty($options)) {
      throw new Exception('Missing arguments in SourcePointAsset.');
    }

    $webhook = trim($webhook, '/');
    $this->apiKey = $api_key;
    $this->options = $options;
    $options = drupal_http_build_query($this->options);
    $this->serviceUrl = trim(self::SERVICE_URL_BASE, '/') . "/$webhook?$options";
  }

  public function determine_asset() {
    if (!function_exists('curl_init')) {
      throw new Exception('php-curl is required.');
    }

    $header = array(
      'Content-type: text/xml',
      "Authorization: Token token=$this->apiKey",
    );

    // Curl init.
    $ch = curl_init($this->serviceUrl);
    curl_setopt_array($ch, array(
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLINFO_HEADER_OUT => TRUE,
      CURLOPT_HTTPHEADER => $header,
    ));

    // Curl exec.
    $data = curl_exec($ch);

    // Check for Auth error (returned as JSON).
    $data_json = drupal_json_decode($data);
    if (!empty($data_json)) {
      throw new Exception(t('Error(s):') . ' ' . PHP_EOL . implode(', ', $data_json));
    }

    // Check for Curl errors.
    if (curl_errno($ch)) {
      throw new Exception(t('Curl Request Error:') . curl_error($ch));
    }

    // Validate retrieved data.
    if (!$this->isValidScript($data)) {
      throw new Exception(t('Retrieved script doesn\'t seem to be valid.'));
    }

    $this->script = $data;

    return $this;
  }

  /**
   * Validate retrieved script.
   */
  private function isValidScript($data) {
    if (isset($this->options['fmt'])) {
      switch ($this->options['fmt']) {
        case 'js':
          return preg_match('~{function~', $data);
        break;

        case 'cdn':
          return preg_match('~cdn\.~', $data);
        break;
      }
    }
  }

}
