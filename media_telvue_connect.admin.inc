<?php

/**
 * Callback for module settings page.
 */
function media_telvue_connect_settings_form($form, &$form_state) {
  $form = array();

  $form['media_telvue_connect_access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Telvue Connect Access Key'),
    '#default_value' => variable_get('media_telvue_connect_access_key', ''),
    '#size' => 150,
  );

  $form['media_telvue_connect_secret_passphrase'] = array(
    '#type' => 'textfield',
    '#title' => t('Telvue Connect Passphrase'),
    '#default_value' => variable_get('media_telvue_connect_secret_passphrase', ''),
    '#size' => 150,
  );

  return system_settings_form($form);
}
