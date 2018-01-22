<?php

/**
 * @file
 * Default display configuration for the default file types.
 */

/**
 * Implements hook_file_default_displays().
 */
function media_telvue_connect_file_default_displays() {
  $file_displays = array();

  //
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__default__media_telvue_connect_video';
  $file_display->weight = 0;
  $file_display->status = TRUE;

  $file_display->settings = array(
    'width' => '640',
    'height' => '390',
    'autoplay' => 0,
    'protocol_specify' => 0,
    'protocol' => 'https:',
  );

  $file_displays['video__default__media_telvue_connect_video'] = $file_display;

  // Add image default displays here

  return $file_displays;
}
