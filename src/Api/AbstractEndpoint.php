<?php
namespace Drupal\sourcepoint\Api;

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
   * @var \Drupal\sourcepoint\Api\HttpClientInterface
   */
  protected $httpClient;

  /**
   * {@inheritdoc}
   */
  public function setHttpClient(HttpClientInterface $http_client) {
    $this->httpClient = $http_client;
  }

  /**
   * {@inheritdoc}
   */
  public function getHttpClient() {
    if (isset($this->httpClient)) {
      return $this->httpClient;
    }
    throw new \Exception('HTTP client has not been set.');
  }

  /**
   * {@inheritdoc}
   */
  public function fetch($path) {
    return file_unmanaged_save_data($this->request(), $path, FILE_EXISTS_REPLACE);
  }

  /**
   * Perform API request.
   * @return string
   * @throws \Exception
   */
  protected function request() {
    return $this->getHttpClient()->request(self::SERVICE_URL_BASE . '/' . $this->getName());
  }
}
