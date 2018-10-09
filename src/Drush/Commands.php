<?php
namespace Drupal\sourcepoint\Drush;

use Drupal\Core\Executable\ExecutableException;
use Drupal\sourcepoint\Api\ApiManagerInterface;
use Drush\Commands\DrushCommands;
use Symfony\Component\Console\Input\InputOption;

/**
 * Class Fetch
 * @package Drupal\sourcepoint\Drush
 */
class Commands extends DrushCommands {
  /**
   * @var \Drupal\sourcepoint\Api\ApiManagerInterface
   */
  protected $apiManager;

  /**
   * Commands constructor.
   * @param \Drupal\sourcepoint\Api\ApiManagerInterface $api_manager
   */
  public function __construct(ApiManagerInterface $api_manager) {
    $this->apiManager = $api_manager;
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
    foreach (['name', 'path', 'apikey'] as $key) {
      if (empty($options[$key]) || is_bool($options[$key])) {
        throw new \Exception('--' . $key . ' is required.');
      }
    }
    // Fetch script.
    $this->apiManager
      ->setApiKey($options['apikey'])
      ->getEndpoint($options['name'])
      ->fetch($options['path']);
  }

}
