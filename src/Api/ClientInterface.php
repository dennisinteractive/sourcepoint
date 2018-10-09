<?php
namespace Drupal\sourcepoint\Api;

/**
 * Interface ClientInterface
 * @package Drupal\sourcepoint\Api
 */
interface ClientInterface {
  /**
   * @param $api_key
   * @return ClientInterface
   */
  public function setApiKey($api_key);

  /**
   * @param $url
   * @return string
   */
  public function request($url);
}
