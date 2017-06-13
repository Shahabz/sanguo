<?php

/**
 * @desc 错误输出级别
 *
 */
error_reporting(E_ALL);

/**
 * @desc 时区
 *
 */
date_default_timezone_set("Etc/GMT");


/**
 * @desc 数据库配置
 *
 */
 
define( 'DB_WEB_HOST', "192.168.197.129" );
define( 'DB_WEB_DBNAME', "war_user" );
define( 'DB_WEB_USER', "eye" );
define( 'DB_WEB_PWD', "eye" );

/**
 * @desc GET串的加密秘钥
 *
 */
define( 'SECRET_KEY', "eye^20170517" );

/**
 * @desc 根目录
 *
 */
define( 'ROOT_PATH', dirname(__FILE__) );

?>
