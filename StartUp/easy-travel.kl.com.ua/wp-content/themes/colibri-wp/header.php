<!DOCTYPE html>
<html <?php language_attributes(); ?>>
<head>
    <meta charset="<?php bloginfo( 'charset' ); ?>">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="profile" href="http://gmpg.org/xfn/11">
    <?php wp_head(); ?>
    <?php colibriwp_theme()->get( 'css' )->render(); ?>
	
	<link rel="icon" href="http://localhost/easy-travel.kl.com.ua/wp-content/uploads/2020/05/Logo1.png" type="image/png" />
    <link rel="shortcut icon" href="http://localhost/easy-travel.kl.com.ua/wp-content/uploads/2020/05/Logo1.png" type="image/png" />
</head>

<body id="colibri" <?php body_class(); ?>>
<?php
if ( function_exists( 'wp_body_open' ) ) {
    wp_body_open();
} else {
    do_action( 'wp_body_open' );
}
?>
<div class="site" id="page-top">
    <?php colibriwp_theme()->get( 'header' )->render(); ?>

