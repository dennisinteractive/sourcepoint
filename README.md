## Sourcepoint
The SCRP is a
combination of software and services that enables a publisher to deliver client
messaging and recover advertising impressions free of interference
by current adblocker technology.

This Drupal module provides the integration with Sourcepoint scripts.

## Updating the local javascripts programmatically:

### Global api key:
```php
variable_set('sourcepoint_api_key', 's4f64sd6f4sd64fssdfsdfsdfsdsdfsfdd4');
```

### Retrieving and storing Bootstrap script:
```php
// Set the version.
variable_set('sourcepoint_bootstrap_script_version', '1');

// Fetch the script.
$script = sourcepoint_fetch_script('bootstrap');

// Build options.
$options = _sourcepoint_get_script_options('bootstrap');
$key = _sourcepoint_build_script_key($options);

// Store.
variable_set('sourcepoint_bootstrap_script', [$key => $script]);
```

### Retrieving and storing Recovery script:
```php
// Set options.
variable_set('sourcepoint_recovery_script_env', 'prod');
variable_set('sourcepoint_recovery_script_pub_adserver', 'dfp');
variable_set('sourcepoint_recovery_script_format', 'cdn');
variable_set('sourcepoint_recovery_script_pub_base', 'http://www.example.com');

// Fetch the script.
$script = sourcepoint_fetch_script('recovery');

// Build options.
$options = _sourcepoint_get_script_options('recovery');
$key = _sourcepoint_build_script_key($options);

// Store.
variable_set('sourcepoint_recovery_script', [$key => $script]);
```

## Fetch script via drush command:
Help and examples

`drush sourcepoint-fs --help`

Update script by type:

`drush sourcepoint-fs --sp-type="bootstrap"`

