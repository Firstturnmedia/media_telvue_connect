<?php

/**
 *  @file media_telvue_connect/includes/MediaTelvueConnectStreamWrapper.inc
 *
 *  Create a Telvue Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $telvue = new MediaTelvueConnectStreamWrapper('telvue://v/[video-code]');
 */
class MediaTelvueConnectStreamWrapper extends MediaReadOnlyStreamWrapper {
  // Overrides $base_url defined in MediaReadOnlyStreamWrapper.
  protected $base_url = 'http://videoplayer.telvue.com';

  /**
   * Returns a url in the format "https://videoplayer.telvue.com/player/BYZxzRoRv7IoaYynQYVU7BGYzrtXvUiD/media/286963".
   *
   * Overrides interpolateUrl() defined in MediaReadOnlyStreamWrapper.
   * This is an exact copy of the function in MediaReadOnlyStreamWrapper,
   * here in case that example is redefined or removed.
   */
  function interpolateUrl() {
    if ($parameters = $this->get_parameters()) {
      return $this->base_url . '/player/' . $parameters['id'] . '/media/' . $parameters['video'];
    }
  }

  static function getMimeType($uri, $mapping = NULL) {
    return 'video/connect';
  }

  function getTarget($f) {
    return FALSE;
  }

  function getOriginalThumbnailPath() {
    $parts = $this->get_parameters();
    //http://videoplayer.telvue.com/thumbnails/134038.jpg
    return 'http://origin.peg.tv/thumbnails/'. check_plain($parts['video']) .'.jpg';
  }

  function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    $local_path = 'public://media-telvue-connect/' . check_plain($parts['video']) . '.jpg';

    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      $response = drupal_http_request($this->getOriginalThumbnailPath());

      if (!isset($response->error)) {
        file_save_data($response->data, $local_path, TRUE);
      }
      else {
        @copy($this->getOriginalThumbnailPath(), $local_path);
      }
    }

    return $local_path;
  }
}
