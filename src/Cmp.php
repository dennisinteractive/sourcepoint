<?php
namespace Drupal\sourcepoint;
use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Url;

/**
 * Class Cmp
 */
class Cmp implements CmpInterface {
  /**
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $configFactory;

  /**
   * Cmp constructor.
   * @param \Drupal\Core\Config\ConfigFactoryInterface $config_factory
   */
  public function __construct(ConfigFactoryInterface $config_factory) {
    $this->configFactory = $config_factory;
  }

  /**
   * {@inheritdoc}
   */
  public function enabled() {
    return (bool) $this->getConfig()->get('cmp_enabled');
  }

  /**
   * {@inheritdoc}
   */
  public function getUrl() {
    $mms_domain = $this->getConfig()->get('mms_domain');
    $privacy_manager_id = $this->getConfig()->get('cmp_privacy_manager_id');
    $site_id = $this->getConfig()->get('cmp_site_id');

    if (!empty($mms_domain) && !empty($site_id) && !empty($privacy_manager_id)) {
      return Url::fromUri('//' . $mms_domain . '/cmp/privacy_manager', array(
        'query' => array(
          'privacy_manager_id' => $privacy_manager_id,
          'site_id' => $site_id,
        )
      ));
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getOverlay() {
    $overlay = array(
      '#theme' => 'sourcepoint_cmp_overlay',
      '#url' => $this->getUrl(),
      '#height' => $this->getConfig()->get('cmp_overlay_height', '600px'),
      '#width' => $this->getConfig()->get('cmp_overlay_width', '600px'),
      '#attached' => [
        'library' => [
          'sourcepoint/cmp'
        ]
      ]
    );
    return $overlay;
  }

  /**
   * @return \Drupal\Core\Config\ImmutableConfig
   */
  protected function getConfig() {
    return $this->configFactory->get('sourcepoint.settings');
  }
}
