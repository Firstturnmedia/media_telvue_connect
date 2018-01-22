<?php

/**
 * @file media_telvue_connect/includes/themes/media_telvue_connect.theme.inc
 *
 * Theme and preprocess functions for Media: Telvue Connect.
 */

/**
 * Preprocess function for theme('media_telvue_connect_video').
 */
function media_telvue_connect_preprocess_media_telvue_connect_video(&$variables) {
  // Build the URI.
  $wrapper = file_stream_wrapper_get_instance_by_uri($variables['uri']);
  $parts = $wrapper->get_parameters();

  $variables['client_id'] = check_plain($parts['id']);
  $variables['video_id'] = check_plain($parts['video']);

  // Make the file object available.
  //$file_object = file_uri_to_object($variables['uri']);

  // Parse options and build the query string. Only add the option to the query
  // array if the option value is not default. Be careful, depending on the
  // wording in media_youtube.formatters.inc, TRUE may be query=0.
  // @see https://developers.google.com/youtube/player_parameters#Parameters
  $query = array();

  // Hardcore fullscreen to true
  $query['fullscreen'] = 'true';

  // Hardcore autostart to false
  $query['autostart'] = 'false';

  // Protocol
  if ($variables['options']['protocol_specify']) {
    $protocol = $variables['options']['protocol'];
  }
  else {
    $protocol = '';
  }

  $url = $protocol . '//videoplayer.telvue.com/player/' . $variables['client_id'] . '/media/' . $variables['video_id'];
  $variables['url'] = url($url, array('query' => $query, 'external' => TRUE));
}
