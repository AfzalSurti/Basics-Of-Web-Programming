-- MySQL schema for To-Do app
CREATE DATABASE IF NOT EXISTS `todo_app` CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
USE `todo_app`;

CREATE TABLE IF NOT EXISTS `tasks` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `title` VARCHAR(255) NOT NULL,
  `is_completed` TINYINT(1) NOT NULL DEFAULT 0,
  `created_at` DATETIME NOT NULL,
  `updated_at` DATETIME NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

