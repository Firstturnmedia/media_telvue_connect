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
    $access_key = variable_get('media_telvue_connect_access_key', FALSE);
    $secret_passphrase = variable_get('media_telvue_connect_secret_passphrase', FALSE);

    // Get parts
    $parts = $this->get_parameters();

    $connect_url = 'https://connect.telvue.com/api/v2/media/' . check_plain($parts['video']) . '.json';

    $options = array(
      'headers' => array(
        'CONNECT-ACCESS-KEY' => $access_key,
        'CONNECT-SECRET-PASSPHRASE' => $secret_passphrase,
      ),
    );

    $response = drupal_http_request($connect_url, $options);

    if (!isset($response->error)) {
      $data = drupal_json_decode($response->data);
      return $data['medium']['thumbnail_large'];
    }
    else {
      throw new Exception("Error Processing Request. (Error: {$response->code}, {$response->error})");
      return;
    }
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
