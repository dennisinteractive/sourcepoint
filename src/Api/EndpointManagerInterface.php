<?php
namespace Drupal\sourcepoint\Api;

/**
 * Interface EndpointManagerInterface
 * @package Drupal\sourcepoint\Api
 */
interface EndpointManagerInterface {
  /**
   * @return mixed
   */
  public function addEndpoint(EndpointInterface $endpoint);

  /**
   * @param $endpoint_name
   * @return \Drupal\sourcepoint\Api\EndpointInterface
   */
  public function getEndpoint($endpoint_name);

  /**
   * @return \Drupal\sourcepoint\Api\EndpointInterface[]
   */
  public function getEndpoints();
}
