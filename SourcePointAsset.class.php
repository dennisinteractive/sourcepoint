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
   * The endpoint.
   */
  protected $endpoint;
  /**
   * Stores the options.
   */
  protected $options;
  /**
   * The script.
   */
  public $script = NULL;

  /**
   * Initial setup.
   */
  public function __construct($endpoint, $api_key) {
    if (empty($endpoint) || empty($api_key)) {
      throw new Exception('Missing arguments in SourcePointAsset.');
    }

    $this->endpoint = trim($endpoint, '/');
    $this->apiKey = $api_key;
  }

  public function fetch() {
    if (!function_exists('curl_init')) {
      throw new Exception('php-curl is required.');
    }

    $header = array(
      'Content-type: text/xml',
      "Authorization: Token token=$this->apiKey",
    );

    $options = drupal_http_build_query($this->options);
    $url = trim(self::SERVICE_URL_BASE, '/') . '/' . $this->endpoint . '?' . $options;

    // Curl init.
    $ch = curl_init($url);
    curl_setopt_array($ch, array(
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLINFO_HEADER_OUT => TRUE,
      CURLOPT_HTTPHEADER => $header,
    ));

    // Curl exec.
    $data = curl_exec($ch);
    $info = curl_getinfo($ch);

    // Check for Auth error (returned as JSON).
    $data_json = drupal_json_decode($data);
    if (!empty($data_json)) {
      throw new Exception(implode(', ', $data_json));
    }

    // Check for Curl errors.
    if (curl_errno($ch) || $info['http_code'] != 200) {
      throw new Exception(t('Curl Request Error') . curl_error($ch));
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
          return preg_match('~function\(~', $data);
          break;

        case 'cdn':
          return preg_match('~cdn\.~', $data);
          break;
      }
    }
  }

  /**
   * Setter for options.
   *
   * @param $options
   */
  public function setOptions($options) {
    $this->options = $options;
  }
}
