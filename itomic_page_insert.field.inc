<?php

/**
 * @file
 * Implement a video field.
 */

/**
 * Implements hook_field_info().
 */
function itomic_page_insert_field_info() {
  return array(
    'itomic_page_insert' => array(
      'label' => 'Page insert',
      'description' => 'Embed custom page insert',
      'settings' => array(),
      'instance_settings' => array(
        'description_field' => 0,
        'description_length' => 128,
        'allowed_providers' => array_keys(video_embed_get_handlers()),
      ),
      'default_widget' => 'itomic_page_insert_video',
      'default_formatter' => 'itomic_page_insert',
      'property_type' => 'itomic_page_insert',
      'property_callbacks' => array('itomic_page_insert_property_info_callback'),
    ),
  );
}

/**
 * Property callback for the Entity Metadata framework.
 */
function itomic_page_insert_property_info_callback(&$info, $entity_type, $field, $instance, $field_type) {
  // Apply the default.
  entity_metadata_field_default_property_callback($info, $entity_type, $field, $instance, $field_type);

  // Finally add in instance specific property info.
  $name = $field['field_name'];
  $property = &$info[$entity_type]['bundles'][$instance['bundle']]['properties'][$name];
  $property['type'] = ($field['cardinality'] != 1) ? 'list<itomic_page_insert>' : 'itomic_page_insert';
  $property['property info'] = itomic_page_insert_data_property_info('Page insert');
  $property['getter callback'] = 'entity_metadata_field_verbatim_get';
  $property['setter callback'] = 'entity_metadata_field_verbatim_set';
}

/**
 * Defines info for the properties of the itomic_page_insert data structure.
 */
function itomic_page_insert_data_property_info($name = NULL) {
  // Build an array of basic property information for itomic_page_insert.
  $properties = array(
    'page_insert' => array(
      'label' => 'Selected Insert',
      'type' => 'text',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'description' => array(
      'label' => 'Description',
      'type' => 'text',
      'setter callback' => 'entity_property_verbatim_set',
    ),
  );

  // Add the default values for each of the itomic_page_insert properties.
  foreach ($properties as $key => &$value) {
    $value += array(
      'description' => !empty($name) ? t('!label of field %name', array('!label' => $value['label'], '%name' => $name)) : '',
    );
  }

  return $properties;
}


/**
 * Implements hook_field_instance_settings_form().
 */
function itomic_page_insert_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];

  $form['description_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Max description length'),
    '#default_value' => isset($settings['description_length']) ? $settings['description_length'] : 128,
    '#weight' => 12,
    '#size' => 5,
    '#maxlength' => 5,
  );

  return $form;
}

/**
 * Implements hook_field_widget_info().
 */
function itomic_page_insert_field_widget_info() {
  return array(
    'itomic_page_insert_video' => array(
      'label' => 'Page insert',
      'description' => 'Provides a video embed field',
      'field types' => array('itomic_page_insert'),
      'settings' => array(),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function itomic_page_insert_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  // Don't need to check the type right now because we're only defining one.
  $inserts = itomic_page_insert_get_inserts();
  $options = array('' => '-- Please select --');
  if(!empty($inserts)){
    foreach($inserts as $key => $value){
      $options[$key] = $value['title'];
    }
  }

  $element += array(
    '#type' => 'fieldset',
  );

  $element['page_insert'] = array(
    '#type' => 'select',
    '#title' => t('Page insert'),
    '#attributes' => array('class' => array('page_insert_name')),
    '#options' => $options,
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'itomic_page_insert') . '/itomic_page_insert.form.css',
      ),
    ),
    '#default_value' => isset($items[$delta]['page_insert']) ? $items[$delta]['page_insert'] : '',
    '#required' => $element['#required'],
  );


  return $element;
}

/**
 * Validates insert validate.
 */
function itomic_page_insert_field_validat($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    if (!empty($item['page_insert'])) {
      $item[$delta]['page_insert'] = trim($item['page_insert']);
    }
  }
}

/**
 * Implements hook_field_presave().
 *
 * Download and save the thumbnail if it hasn't already been stored.
 * Get video data.
 */
function itomic_page_insert_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  foreach ($items as $delta => $item) {
    // Trim whitespace from the video URL.
    $items[$delta]['page_insert'] = trim($item['page_insert']);

  }
}

/**
 * Implements hook_field_insert().
 *
 * Download and save the thumbnail if it hasn't already been stored.
 * Get video data.
 */
function itomic_page_insert_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  foreach ($items as $delta => $item) {
    // Trim whitespace from the video URL.
    $items[$delta]['page_insert'] = trim($item['page_insert']);
  }
}


/**
 * Implements hook_field_is_empty().
 */
function itomic_page_insert_field_is_empty($item, $field) {
  return empty($item) || empty($item['page_insert']) || $item['page_insert'] === '';
}

/**
 * Implements hook_field_formatter_info().
 */
function itomic_page_insert_field_formatter_info() {
  $info = array(
    'itomic_page_insert' => array(
      'label' => t('Page insert'),
      'field types' => array('itomic_page_insert'),
      'settings' => array(
        'page_insert' => '',
      ),
    ),
  );

  return $info;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function itomic_page_insert_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();

  return $element;
}
/**
 * Implements hook_field_formatter_settings_summary().
 */
function itomic_page_insert_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();
  if ($display['type'] == 'itomic_page_insert'){
  	$summary[] = t('Display insert');
  }

  if (isset($settings['description'])) {
    if ($settings['description'] && $instance['settings']['description_field']) {
      $summary[] = t('Show description');
    }
    elseif ($instance['settings']['description_field']) {
      $summary[] = t('Hide description');
    }
  }

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function itomic_page_insert_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $settings = $display['settings'];

  $inserts = itomic_page_insert_get_inserts();

  foreach ($items as $delta => $item) {
    // Create the render array for the description.
    if (isset($item['description']) && $item['description'] && $settings['description'] && $instance['settings']['description_field']) {
      $description = array(
        '#prefix' => '<div class="page-insert-description">',
        '#markup' => check_plain($item['description']),
        '#suffix' => '</div>',
      );
      $alt = $item['description'];
    }
    else {
      $description = array();
      $alt = '';
    }

    // Render the field.
    if ($display['type'] == 'itomic_page_insert') {
    	$key = $item['page_insert'];
    	if(isset($inserts[$key]) && !empty($inserts[$key]['callback'])){
    		$callback = $inserts[$key]['callback'];
    		if(function_exists($callback)){
    			$processed = $callback();
    		} else {
    			$processed = '';
    		}
    	}

    	if(is_string($processed)){
    		$processed = array('#markup' => $processed);
    	}

    	/*
      $element[$delta] = array(
        array(
          $processed
        ),
      );
      */

      $element[$delta] = array(
        $processed,
      );

    }

    // Get the HTML instead of the array, because we append it to the suffix.
    // This way, the thumbnail link doesn't make the description a link as well.
    $description_html = drupal_render($description);

    $pos = isset($settings['description_position']) ? $settings['description_position'] : 'bottom';
    if ($pos == 'top') {
      $element[$delta]['#prefix'] = isset($element[$delta]['#prefix']) ? $element[$delta]['#prefix'] : '';
      $element[$delta]['#prefix'] = $description_html . $element[$delta]['#prefix'];
    }
    else {
      $element[$delta]['#suffix'] = isset($element[$delta]['#suffix']) ? $element[$delta]['#suffix'] : '';
      $element[$delta]['#suffix'] .= $description_html;
    }
  }

  return $element;
}






