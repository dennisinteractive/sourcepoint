<?php
namespace Drupal\sourcepoint\Api;

/**
 * Interface EndpointInterface
 * @package Drupal\sourcepoint\Api
 */
interface EndpointInterface {
  /**
   * Stores the endpoint script.
   * @return EndpointInterface
   */
  public function fetch($path);

  /**
   * Script to fetch from the API.
   * @return string
   */
  public function getName();

  /**
   * @param \Drupal\sourcepoint\Api\HttpClientInterface $http_client
   * @return EndpointInterface
   */
  public function setHttpClient(HttpClientInterface $http_client);
}
