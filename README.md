# README #

The module provides a field that can output defined callbacks that will be pushed though drupal_render.

This works well with the [paragraphs](https://www.drupal.org/project/paragraphs) module.

### How to use this module ###

To define the callbacks, create a custom module and define the following hook. 
Then include you custom callback function. 

```
#!php

/**
 * hook_itomic_page_insert
 */
function my_module_itomic_page_inserts(&$vars) {

  $vars['my_page_insert'] = array(
    'callback'  => 'my_module_loader',
    'title'     => t('My Page Insert'), 
  );
  return $vars;
}

/**
 * Callback
 */
function my_module_loader() {
    $base = drupal_get_path('module', 'my_module');

    $content = array();
    $content['#attached'] = array(    // Add custom CSS.
        'css' => array(
            $base . '/color.css' => array(),
        ),
        // Add custom JavaScript.
        'js' => array(
            $base . '/color.js',
        )
    );
    $content['#markup'] = '<h1>Hello world</h1>';

    return $content;
}
```
