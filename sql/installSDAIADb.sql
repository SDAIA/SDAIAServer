-- MySQL Script generated by MySQL Workbench
-- mar 09 feb 2016 08:34:40 CET
-- Model: New Model    Version: 1.0
-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema sdaia
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema sdaia
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `sdaia` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `sdaia` ;

-- -----------------------------------------------------
-- Table `sdaia`.`groups`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `sdaia`.`groups` ;

CREATE TABLE IF NOT EXISTS `sdaia`.`groups` (
  `groupid` INT NOT NULL AUTO_INCREMENT,
  `groupname` VARCHAR(32) NOT NULL,
  `root` BIT(1) NOT NULL DEFAULT 0,
  `description` TEXT(1000) NULL,
  `private` BIT(1) NOT NULL DEFAULT 0,
  `closed` BIT(1) NOT NULL DEFAULT 0,
  PRIMARY KEY (`groupid`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `sdaia`.`users`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `sdaia`.`users` ;

CREATE TABLE IF NOT EXISTS `sdaia`.`users` (
  `userid` INT NOT NULL AUTO_INCREMENT,
  `username` VARCHAR(32) NOT NULL,
  `email` VARCHAR(255) NOT NULL,
  `password` VARCHAR(32) NOT NULL,
  `create_time` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,
  `groups_groupid` INT NOT NULL,
  PRIMARY KEY (`userid`, `groups_groupid`),
  INDEX `fk_users_groups_idx` (`groups_groupid` ASC),
  CONSTRAINT `fk_users_groups`
    FOREIGN KEY (`groups_groupid`)
    REFERENCES `sdaia`.`groups` (`groupid`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `sdaia`.`permissions`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `sdaia`.`permissions` ;

CREATE TABLE IF NOT EXISTS `sdaia`.`permissions` (
  `idpermissions` INT NOT NULL AUTO_INCREMENT,
  `permitname` VARCHAR(48) NOT NULL,
  `description` TEXT(1000) NULL,
  PRIMARY KEY (`idpermissions`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `sdaia`.`groups_has_permissions`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `sdaia`.`groups_has_permissions` ;

CREATE TABLE IF NOT EXISTS `sdaia`.`groups_has_permissions` (
  `groups_groupid` INT NOT NULL,
  `permissions_idpermissions` INT NOT NULL,
  PRIMARY KEY (`groups_groupid`, `permissions_idpermissions`),
  INDEX `fk_groups_has_permissions_permissions1_idx` (`permissions_idpermissions` ASC),
  INDEX `fk_groups_has_permissions_groups1_idx` (`groups_groupid` ASC),
  CONSTRAINT `fk_groups_has_permissions_groups1`
    FOREIGN KEY (`groups_groupid`)
    REFERENCES `sdaia`.`groups` (`groupid`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_groups_has_permissions_permissions1`
    FOREIGN KEY (`permissions_idpermissions`)
    REFERENCES `sdaia`.`permissions` (`idpermissions`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `sdaia`.`users_has_permissions`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `sdaia`.`users_has_permissions` ;

CREATE TABLE IF NOT EXISTS `sdaia`.`users_has_permissions` (
  `users_userid` INT NOT NULL,
  `permissions_idpermissions` INT NOT NULL,
  PRIMARY KEY (`users_userid`, `permissions_idpermissions`),
  INDEX `fk_users_has_permissions_permissions1_idx` (`permissions_idpermissions` ASC),
  INDEX `fk_users_has_permissions_users1_idx` (`users_userid` ASC),
  CONSTRAINT `fk_users_has_permissions_users1`
    FOREIGN KEY (`users_userid`)
    REFERENCES `sdaia`.`users` (`userid`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_users_has_permissions_permissions1`
    FOREIGN KEY (`permissions_idpermissions`)
    REFERENCES `sdaia`.`permissions` (`idpermissions`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `sdaia`.`applications`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `sdaia`.`applications` ;

CREATE TABLE IF NOT EXISTS `sdaia`.`applications` (
  `applicationid` INT NOT NULL AUTO_INCREMENT,
  `applicationname` VARCHAR(48) NOT NULL,
  `description` TEXT(2500) NULL,
  `create_time` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP,
  `last_edit_time` TIMESTAMP NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `framework` VARCHAR(48) NOT NULL,
  `exec_path` VARCHAR(255) NOT NULL,
  `data_path` VARCHAR(255) NOT NULL,
  `users_userid` INT NOT NULL,
  `groups_groupid` INT NOT NULL,
  PRIMARY KEY (`applicationid`, `users_userid`, `groups_groupid`),
  INDEX `fk_applications_users1_idx` (`users_userid` ASC),
  INDEX `fk_applications_groups1_idx` (`groups_groupid` ASC),
  CONSTRAINT `fk_applications_users1`
    FOREIGN KEY (`users_userid`)
    REFERENCES `sdaia`.`users` (`userid`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_applications_groups1`
    FOREIGN KEY (`groups_groupid`)
    REFERENCES `sdaia`.`groups` (`groupid`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
