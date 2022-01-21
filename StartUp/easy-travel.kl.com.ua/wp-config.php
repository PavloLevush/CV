<?php

/**

 * The base configuration for WordPress

 *

 * The wp-config.php creation script uses this file during the

 * installation. You don't have to use the web site, you can

 * copy this file to "wp-config.php" and fill in the values.

 *

 * This file contains the following configurations:

 *

 * * MySQL settings

 * * Secret keys

 * * Database table prefix

 * * ABSPATH

 *

 * @link https://codex.wordpress.org/Editing_wp-config.php

 *

 * @package WordPress

 */



// ** MySQL settings - You can get this info from your web host ** //

/** The name of the database for WordPress */

define( 'DB_NAME', 'pavlo_levush' );



/** MySQL database username */

define( 'DB_USER', 'root' );



/** MySQL database password */

define( 'DB_PASSWORD', '' );



/** MySQL hostname */

define( 'DB_HOST', 'localhost' );



/** Database Charset to use in creating database tables. */

define( 'DB_CHARSET', 'utf8mb4' );



/** The Database Collate type. Don't change this if in doubt. */

define( 'DB_COLLATE', '' );



/**#@+

 * Authentication Unique Keys and Salts.

 *

 * Change these to different unique phrases!

 * You can generate these using the {@link https://api.wordpress.org/secret-key/1.1/salt/ WordPress.org secret-key service}

 * You can change these at any point in time to invalidate all existing cookies. This will force all users to have to log in again.

 *

 * @since 2.6.0

 */

define( 'AUTH_KEY',         '#`IFLMO|P6sF}{F<<Arn8GnK:@$A<<}wmMbb4aqw$uSh0-(*+u}]*<:l;{GzFaq)' );

define( 'SECURE_AUTH_KEY',  '_u~Ca}5C;@=h>uSs5x&5e3>gEf;eKH#<Xo^^ JNTc|17%1i)=>:*ma ,LalvnEUE' );

define( 'LOGGED_IN_KEY',    'j.QR@B!cwJkx2p|kk(YDrmw1@qr6I+XAU`Jba-[#w9F^i(x@g&j!J|4j<e{J:fz6' );

define( 'NONCE_KEY',        'N&fGE<?JN0>T/7FLy:xY;J.rjwc}pQ$yPGFsYgZpg:5Qs$%-<4c./cXUBD#XylO-' );

define( 'AUTH_SALT',        'wr$<gC>Z3?M!,e`aSJwf%%AV%(g q=_#uF@q>t`Kw_kg-~pI4P!H;o OV&fC/H2*' );

define( 'SECURE_AUTH_SALT', '7z%L|fD0Z^Uj.go56~(=A[l9P!sVD`BJL(FI`E^zUnu^|myF(Ry;g&sG=P%%K!Q4' );

define( 'LOGGED_IN_SALT',   'b6~9,_qZ^lYkm|1<$CiYU<KDTR7mQk){ht_Ha-GP&L4B2zrstr+>Q.g[l_5ARH(m' );

define( 'NONCE_SALT',       'WV$+sq5L9X+u^g+`%Cy4M69-JlN^aAv7HvV;@+]jN*4lp9*P?3#x]rZezr=/ J3)' );



/**#@-*/



/**

 * WordPress Database Table prefix.

 *

 * You can have multiple installations in one database if you give each

 * a unique prefix. Only numbers, letters, and underscores please!

 */

$table_prefix = 'wp_';



/**

 * For developers: WordPress debugging mode.

 *

 * Change this to true to enable the display of notices during development.

 * It is strongly recommended that plugin and theme developers use WP_DEBUG

 * in their development environments.

 *

 * For information on other constants that can be used for debugging,

 * visit the Codex.

 *

 * @link https://codex.wordpress.org/Debugging_in_WordPress

 */

define( 'WP_DEBUG', false);


define('FS_METHOD', 'direct');
$GLOBALS['_wp_filesystem_direct_method'] = 'relaxed_ownership';
define('DISABLE_WP_CRON', true);
define('WP_MEMORY_LIMIT', '64M');



/* That's all, stop editing! Happy publishing. */



/** Absolute path to the WordPress directory. */

if ( ! defined( 'ABSPATH' ) ) {

	define( 'ABSPATH', dirname( __FILE__ ) . '/' );

}



/** Sets up WordPress vars and included files. */

require_once( ABSPATH . 'wp-settings.php' );

