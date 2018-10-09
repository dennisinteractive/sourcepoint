<?php
namespace Drupal\sourcepoint\Drush;

use Drupal\sourcepoint\Api\EndpointManagerInterface;
use Drush\Commands\DrushCommands;

/**
 * Class Fetch
 * @package Drupal\sourcepoint\Drush
 */
class Commands extends DrushCommands {
  /**
   * @var \Drupal\sourcepoint\Api\EndpointManagerInterface
   */
  protected $endpointManager;

  /**
   * Commands constructor.
   * @param \Drupal\sourcepoint\Api\EndpointManagerInterface $endpoint_manager
   */
  public function __construct(EndpointManagerInterface $endpoint_manager) {
    $this->endpointManager = $endpoint_manager;
  }

  /**
   * @command sourcepoint:fetch
   * @param array $options
   * @options name Script name
   * @options path Location to save the script
   * @options apikey API key to authenticate
   * @aliases sp:fetch
   * @throws \Exception
   */
  public function fetch($options = [
    'name' => '',
    'path' => '',
    'apikey' => '',
  ]) {
    // Validate required options.
    foreach (['name', 'apikey'] as $key) {
      if (empty($options[$key]) || is_bool($options[$key])) {
        throw new \Exception('--' . $key . ' is required.');
      }
    }
    // Get endpoint.
    $endpoint = $this->endpointManager
      ->getEndpoint($options['name'])
      ->setApiKey($options['apikey']);

    // Set path.
    if (!empty($options['path']) && is_string($options['path'])) {
      $endpoint->setPath($options['path']);
    }

    // Fetch script and save config.
    $endpoint
      ->fetch()
      ->saveConfig();
  }

}
