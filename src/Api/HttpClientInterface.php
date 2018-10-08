<?php
namespace Drupal\sourcepoint\Api;

/**
 * Interface HttpClientInterface
 * @package Drupal\sourcepoint\Api
 */
interface HttpClientInterface {
  /**
   * @param $api_key
   * @return HttpClientInterface
   */
  public function setApiKey($api_key);

  /**
   * @param $url
   * @return string
   */
  public function request($url);
}
