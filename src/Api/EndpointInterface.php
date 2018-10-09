<?php
namespace Drupal\sourcepoint\Api;
use Drupal\Core\Config\ConfigFactoryInterface;

/**
 * Interface EndpointInterface
 * @package Drupal\sourcepoint\Api
 */
interface EndpointInterface {
  /**
   * @param $api_key
   * @return EndpointInterface
   */
  public function setApiKey($api_key);

  /**
   * Fetches/stores the endpoint script.
   * @return EndpointInterface
   */
  public function fetch();

  /**
   * Sets the path where the script will be saved.
   * @return EndpointInterface
   */
  public function setPath($path);

  /**
   * Gets the path where the script will be saved.
   * @return string
   */
  public function getPath();

  /**
   * Script to fetch from the API.
   * @return string
   */
  public function getName();

  /**
   * Saves the endpoint configuration.
   * @return EndpointInterface
   */
  public function saveConfig();
}
