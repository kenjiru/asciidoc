<?php
/**
 * Form (items) for filter settings.
 */
function _asciidoc_filter_settings() {
  $settings['info'] = array(
    '#markup' => '<p>'. t('AsciiDoc filter can be configured through 
        <a href="!asciidoc_admin_url">general AsciiDoc filter settings</a>.', 
      array('!asciidoc_admin_url' => 
        url('admin/config/content/formats/asciidoc'))) .'</p>'
  );

  return $settings;
}

/**
 * Create and display the AsciiDoc configuration form.
 */
function asciidoc_config_form($form, &$form_state) {
  // Text field for the asciidoc executable path.
  $form['asciidoc_asciidoc_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Asciidoc executable path'),
    '#description' => t('The path to the asciidoc executable. Enter "asciidoc" 
      if the executable is installed system wide.'),
    '#default_value' => variable_get('asciidoc_asciidoc_path', 'asciidoc'),
    '#size' => 60,
    '#maxlength' => 200,
    '#required' => TRUE,
  );
  
  $highlight_options = drupal_map_assoc(
    array("source-highlight", "Pygments"), 
    'highlight_options'
  );

  // Select field for the highlight library
  $form['asciidoc_highlight_library'] = array(
    '#type' => 'select',
    '#title' => t('Highlight library'),
    '#description' => t('Select the highlight library to be used with 
      asciidoc.'),
    '#default_value' => variable_get('asciidoc_highlight_library',
      'source-highlight'),
    '#options' => $highlight_options,
  );

  // Text field for the Pygments library path.
  $form['asciidoc_pygments_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Pygments library path'),
    '#description' => t('The path to the Pygments library. Let the field blank 
      if the library is in the system path.'),
    '#default_value' => variable_get('asciidoc_pygments_path'),
    '#states' => array(
      // Only show this field when the 'asciidoc_highlight_library' select has
      // the value Pygments.
      'visible' => array(
        ':input[name="asciidoc_highlight_library"]' => 
          array('value' => 'Pygments'),
      ),
    ),
    '#size' => 60,
    '#maxlength' => 200,
  );

  return system_settings_form($form);
}

/**
 * Handles validating of the AsciiDoc configuration form.
 */
function asciidoc_config_form_validate($form, &$form_state) {
  $asciidoc_highlight_library = $form_state['values']['asciidoc_highlight_library'];
  $asciidoc_pygments_path = $form_state['values']['asciidoc_pygments_path'];
  $asciidoc_asciidoc_path = $form_state['values']['asciidoc_asciidoc_path'];
  
  exec(escapeshellcmd($asciidoc_asciidoc_path . ' --version'), $output, $ret);
  if ($ret !== 0) {
    form_set_error('asciidoc_asciidoc_path', 
      t('Could not find the Asciidoc executable!'));
  }
  
  // If we used the Pygments library for highlighting, check for it's existence
  if ($asciidoc_highlight_library === 'Pygments') {
    exec(escapeshellcmd($asciidoc_pygments_path . '/pygmentize -V'), 
      $output, $ret);
    if ($ret !== 0) {
      form_set_error('asciidoc_pygments_path', 
        t('Could not find the Pygments library!'));
    }
  }
}
