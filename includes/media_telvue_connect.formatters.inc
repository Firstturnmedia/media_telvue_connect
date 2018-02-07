<?php

/**
 * @file media_telvue_connect/includes/media_telvue_connect.formatters.inc
 *
 * Formatters for Media: Telvue Connect.
 */

/**
 * Implements hook_file_formatter_info().
 */
function media_telvue_connect_file_formatter_info() {
  // Telvue connect video
  $formatters['media_telvue_connect_video'] = array(
    'label' => t('Telvue Connect Video'),
    'file types' => array('video'),
    'default settings' => array(
      'width' => 640,
      'height' => 390,
      'fullscreen' => 'true',
    ),
    'view callback' => 'media_telvue_connect_file_formatter_video_view',
    'settings callback' => 'media_telvue_connect_file_formatter_video_settings',
    'mime types' => array('video/connect'),
  );

  // Telvue connect image
  $formatters['media_telvue_connect_image'] = array(
    'label' => t('Telvue Connect Preview Image'),
    'file types' => array('video'),
    'default settings' => array(
      'image_style' => '',
    ),
    'view callback' => 'media_telvue_connect_file_formatter_image_view',
    'settings callback' => 'media_telvue_connect_file_formatter_image_settings',
    'mime types' => array('video/connect'),
  );

  return $formatters;
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_telvue_connect_file_formatter_video_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);

  if ($scheme == 'connect') {
    $element = array(
      '#theme' => 'media_telvue_connect_video',
      '#uri' => $file->uri,
      '#options' => array(),
    );

    // Fake a default for attributes so the ternary doesn't choke.
    $display['settings']['attributes'] = array();

    // Fake a default for attributes so the ternary doesn't choke.
    /*$display['settings']['attributes'] = array();

    $settings = array_keys(media_telvue_connect_variable_default());

    foreach ($settings as $setting) {
      $element['#options'][$setting] = isset($file->override[$setting]) ? $file->override[$setting] : $display['settings'][$setting];
    }
    */

    return $element;
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_telvue_connect_file_formatter_video_settings($form, &$form_state, $settings) {
  $element = array();

  $element['width'] = array(
    '#title' => t('Width'),
    '#type' => 'textfield',
    '#default_value' => $settings['width'],
    '#element_validate' => array('_telvue_connect_validate_video_width_and_height'),
  );

  $element['height'] = array(
    '#title' => t('Height'),
    '#type' => 'textfield',
    '#default_value' => $settings['height'],
    '#element_validate' => array('_telvue_connect_validate_video_width_and_height'),
  );

  // @todo check if this is even offered by telvue
  /*$element['autoplay'] = array(
    '#title' => t('Autoplay video on load'),
    '#type' => 'checkbox',
    '#default_value' => $settings['autoplay'],
  );
  */

  $element['protocol_specify'] = array(
    '#title' => t('Specify an http protocol'),
    '#type' => 'checkbox',
    '#default_value' => $settings['protocol_specify'],
    '#description' => t('An explicit protocol may be neccesary for videos embedded in RSS feeds and emails. If no protocol is specified, iframes will be protocol relative.'),
  );

  $element['protocol'] = array(
    '#title' => t('Iframe protocol'),
    '#type' => 'radios',
    '#default_value' => $settings['protocol'],
    '#options' => array(
      'http:' => 'http://',
      'https:' => 'https://',
    ),
    '#states' => array(
      'invisible' => array(
        ':input[name="displays[media_telvue_connect_video][settings][protocol_specify]"]' => array('checked' => FALSE),
      ),
    ),
  );

  return $element;
}

/**
 * Validation for width and height.
 */
function _telvue_connect_validate_video_width_and_height($element, &$form_state, $form) {
  // Check if the value is a number with an optional decimal or percentage sign, or "auto".
  if (!empty($element['#value']) && !preg_match('/^(auto|([0-9]*(\.[0-9]+)?%?))$/', $element['#value'])) {
    form_error($element, t("The value entered for @dimension is invalid. Please insert a unitless integer for pixels, a percent, or \"auto\". Note that percent and auto may not function correctly depending on the browser and doctype.", array('@dimension' => $element['#title'])));
  }
}

/**
 * Validation for Js API Origin.
 */
function _telvue_connect_validate_jsapi_domain($element, &$form_state, $form) {
  // Check if the value is a url with http/s and no trailing directories.
  if (!empty($element['#value']) && !preg_match('/^https?\:\/\/[a-zA-Z0-9\-\.]+\.([a-zA-Z]{2,4}){1,2}$/', $element['#value'])) {
    form_error($element, t('Please insert a valid domain in the format http://www.yourdomain.com'));
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_telvue_connect_file_formatter_image_view($file, $display, $langcode) {
  $scheme = file_uri_scheme($file->uri);

  if ($scheme == 'connect') {
    $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);
    $image_style = $display['settings']['image_style'];
    $valid_image_styles = image_style_options(FALSE);

    // @TODO: If autosubmit is removed and we allow view modes that insert
    // images in the WYSIWYG, add file->overrides handling.
    if (empty($image_style) || !isset($valid_image_styles[$image_style])) {
      $element = array(
        '#theme' => 'image',
        '#path' => $wrapper->getOriginalThumbnailPath(),
        '#alt' => isset($file->override['attributes']['alt']) ? $file->override['attributes']['alt'] : $file->filename,
      );
    }
    else {
      $element = array(
        '#theme' => 'image_style',
        '#style_name' => $image_style,
        '#path' => $wrapper->getLocalThumbnailPath(),
        '#alt' => isset($file->override['attributes']['alt']) ? $file->override['attributes']['alt'] : $file->filename,
      );
    }

    return $element;
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_telvue_connect_file_formatter_image_settings($form, &$form_state, $settings) {
  $element = array();

  $element['image_style'] = array(
    '#title' => t('Image style'),
    '#type' => 'select',
    '#options' => image_style_options(FALSE),
    '#default_value' => $settings['image_style'],
    '#empty_option' => t('None (original image)'),
  );

  $element['image_style_edit_link'] = array(
    '#type' => 'link',
    '#title' => t('Edit image style'),
    '#href' => 'admin/config/media/image-styles/edit/' . key(image_style_options(FALSE)),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'media_telvue') . '/js/media_telvue_connect.imagestyle.js',
      ),
    ),
  );

  return $element;
}
