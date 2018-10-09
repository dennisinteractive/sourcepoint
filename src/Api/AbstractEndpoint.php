<?php
namespace Drupal\sourcepoint\Api;
use Drupal\Core\Config\ConfigFactoryInterface;

/**
 * Class AbstractEndpoint
 * @package Drupal\sourcepoint\Api
 */
abstract class AbstractEndpoint implements EndpointInterface {
  /**
   * Sourcepoint Service URL.
   */
  const SERVICE_URL_BASE = 'https://api.sourcepoint.com/script';

  /**
   * The config name for endpoint settings.
   */
  const CONFIG_BASE_NAME = 'sourcepoint.endpoints';

  /**
   * @var \Drupal\sourcepoint\Api\ClientInterface
   */
  protected $client;

  /**
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $configFactory;

  /**
   * @var string
   */
  protected $path;

  /**
   * AbstractEndpoint constructor.
   * @param \Drupal\sourcepoint\Api\ClientInterface $http_client
   * @param \Drupal\Core\Config\ConfigFactoryInterface $config_factory
   */
  public function __construct(ClientInterface $client, ConfigFactoryInterface $config_factory) {
    $this->client = $client;
    $this->configFactory = $config_factory;
  }

  /**
   * {@inheritdoc}
   */
  public function setPath($path) {
    $this->path = $path;
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function getPath() {
    if (isset($this->path)) {
      return $this->path;
    }
    if ($path = $this->getConfig()->get('path')) {
      return $this->path = $path;
    }
    throw new \Exception('Path has not been set');
  }

  /**
   * {@inheritdoc}
   */
  public function fetch() {
    file_unmanaged_save_data($this->request(), $this->getPath(), FILE_EXISTS_REPLACE);
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function setApiKey($api_key) {
    $this->client->setApiKey($api_key);
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function saveConfig() {
    $config = $this->getEditableConfig();
    $config->set('path', $this->getPath());
    $config->save();
    return $this;
  }

  /**
   * @return string
   */
  protected function getConfigName() {
    return self::CONFIG_BASE_NAME . '.' . $this->getName();
  }

  /**
   * @return \Drupal\Core\Config\Config
   */
  protected function getEditableConfig() {
    return $this->configFactory->getEditable($this->getConfigName());
  }

  /**
   * @return \Drupal\Core\Config\ImmutableConfig
   */
  protected function getConfig() {
    return $this->configFactory->get($this->getConfigName());
  }

  /**
   * Perform API request.
   * @return string
   * @throws \Exception
   */
  protected function request() {
    return $this->client->request(self::SERVICE_URL_BASE . '/' . $this->getName());
  }
}
