<?php
namespace Drupal\sourcepoint;

/**
 * Interface CmpInterface
 */
interface CmpInterface {
  /**
   * Check if CMP is enabled.
   * @return bool
   */
  public function enabled();

  /**
   * Gets the privacy URL.
   * @return \Drupal\Core\Url
   */
  public function getUrl();

  /**
   * Gets the privacy URL.
   * @return array
   */
  public function getOverlay();

}
