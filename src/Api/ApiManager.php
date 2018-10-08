<?php
namespace Drupal\sourcepoint\Api;

/**
 * Class ApiManager
 * @package Drupal\sourcepoint\Api
 */
class ApiManager implements ApiManagerInterface {

  /**
   * @var \Drupal\sourcepoint\Api\EndpointInterface[]
   */
  protected $endpoints = [];

  /**
   * @var \Drupal\sourcepoint\Api\HttpClientInterface
   */
  protected $httpClient;

  /**
   * ApiManager constructor.
   * @param \Drupal\sourcepoint\Api\HttpClientInterface $http_client
   */
  public function __construct(HttpClientInterface $http_client) {
    $this->httpClient = $http_client;
  }

  /**
   * {@inheritdoc}
   */
  public function fetch($endpoint_name) {
    return $this->getEndpoint($endpoint_name)->fetch();
  }

  /**
   * {@inheritdoc}
   */
  public function setApiKey($api_key) {
    $this->httpClient->setApiKey($api_key);
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function addEndpoint(EndpointInterface $endpoint) {
    // Set HTTP Client.
    $endpoint->setHttpClient($this->httpClient);
    // Register endpoint.
    $this->endpoints[$endpoint->getName()] = $endpoint;
  }

  /**
   * {@inheritdoc}
   */
  public function getEndpoint($endpoint_name) {
    if (isset($this->endpoints[$endpoint_name])) {
      return $this->endpoints[$endpoint_name];
    }
    throw new \Exception('Unknown endpoint ' . $endpoint_name . '.
      Available endpoints: ' . implode(', ', array_keys($this->endpoints)));
  }

  /**
   * {@inheritdoc}
   */
  public function getEndpoints() {
    return $this->endpoints;
  }
}
