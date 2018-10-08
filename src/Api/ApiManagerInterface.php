<?php
namespace Drupal\sourcepoint\Api;

/**
 * Interface ApiManagerInterface
 * @package Drupal\sourcepoint\Api
 */
interface ApiManagerInterface {
  /**
   * @param $endpoint_name
   * @return ApiManagerInterface
   */
  public function fetch($endpoint_name);

  /**
   * @param $api_key
   * @return ApiManagerInterface
   */
  public function setApiKey($api_key);

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
