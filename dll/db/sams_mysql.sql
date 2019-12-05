/*
Navicat MySQL Data Transfer

Source Server         : mysql1
Source Server Version : 50553
Source Host           : localhost:3306
Source Database       : sams

Target Server Type    : MYSQL
Target Server Version : 50553
File Encoding         : 65001

Date: 2019-06-24 13:46:50
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for alarmlink
-- ----------------------------
DROP TABLE IF EXISTS `alarmlink`;
CREATE TABLE `alarmlink` (
  `PositionID` varchar(10) NOT NULL,
  `ComName` varchar(5) NOT NULL,
  `BaudRate` int(6) NOT NULL,
  `ModelAddr` int(3) NOT NULL,
  `LinkAddr` varchar(10) NOT NULL,
  `LinkEnable` varchar(2) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of alarmlink
-- ----------------------------

-- ----------------------------
-- Table structure for alarmlog
-- ----------------------------
DROP TABLE IF EXISTS `alarmlog`;
CREATE TABLE `alarmlog` (
  `LogID` int(11) NOT NULL AUTO_INCREMENT,
  `PositionID` varchar(10) DEFAULT NULL,
  `DeviceName` varchar(20) DEFAULT NULL,
  `NodeName` varchar(20) DEFAULT NULL,
  `NodeValue` varchar(5) DEFAULT NULL,
  `NodeSign` varchar(5) DEFAULT NULL,
  `Content` varchar(20) DEFAULT NULL,
  `StartTime` varchar(19) DEFAULT NULL,
  `EndTime` varchar(19) DEFAULT NULL,
  `ConfirmUser` varchar(10) DEFAULT NULL,
  `ConfirmTime` varchar(19) DEFAULT NULL,
  `ConfirmContent` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`LogID`),
  KEY `AlarmLogIndex` (`DeviceName`,`Content`,`StartTime`,`EndTime`,`NodeName`,`LogID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of alarmlog
-- ----------------------------

-- ----------------------------
-- Table structure for deviceinfo
-- ----------------------------
DROP TABLE IF EXISTS `deviceinfo`;
CREATE TABLE `deviceinfo` (
  `DeviceID` int(11) NOT NULL,
  `PortName` varchar(20) NOT NULL,
  `DeviceName` varchar(20) NOT NULL,
  `DeviceAddr` int(3) NOT NULL,
  `DeviceType` varchar(10) NOT NULL,
  `NodeNumber` int(3) NOT NULL,
  PRIMARY KEY (`DeviceID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of deviceinfo
-- ----------------------------

-- ----------------------------
-- Table structure for nodedata
-- ----------------------------
DROP TABLE IF EXISTS `nodedata`;
CREATE TABLE `nodedata` (
  `PositionID` varchar(10) NOT NULL,
  `NodeValue` varchar(5) NOT NULL,
  `NodeStatus` int(1) NOT NULL,
  `SaveTime` varchar(19) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of nodedata
-- ----------------------------
INSERT INTO `nodedata` VALUES ('AT-10001', '55', '1', '2019-06-23 12:25:26');
INSERT INTO `nodedata` VALUES ('AT-10002', '58', '0', '2019-06-23 12:25:26');
INSERT INTO `nodedata` VALUES ('AT-10004', '88', '2', '2019-06-23 12:25:26');
INSERT INTO `nodedata` VALUES ('AT-10003', '63', '3', '2019-06-23 12:25:26');
INSERT INTO `nodedata` VALUES ('AT-10005', '67', '4', '2019-06-23 12:25:26');

-- ----------------------------
-- Table structure for nodeinfo
-- ----------------------------
DROP TABLE IF EXISTS `nodeinfo`;
CREATE TABLE `nodeinfo` (
  `NodeID` int(3) DEFAULT NULL,
  `PositionID` varchar(10) NOT NULL,
  `DeviceName` varchar(20) NOT NULL,
  `NodeName` varchar(20) NOT NULL,
  `NodeAddr` int(3) NOT NULL,
  `NodeType` varchar(10) NOT NULL,
  `NodeClass` varchar(5) NOT NULL,
  `NodeSign` varchar(5) NOT NULL,
  `NodeUpper` varchar(5) NOT NULL,
  `NodeLimit` varchar(5) NOT NULL,
  `NodeMax` varchar(5) NOT NULL,
  `NodeMin` varchar(5) NOT NULL,
  `NodeRange` varchar(5) NOT NULL,
  `NodeEnable` varchar(2) NOT NULL,
  `NodeSound` varchar(10) NOT NULL,
  `NodeImage` varchar(10) NOT NULL,
  `SaveInterval` int(3) NOT NULL,
  `DotCount` int(1) NOT NULL,
  `AlarmDelay` int(3) NOT NULL,
  `AlarmType` varchar(2) DEFAULT NULL,
  `NodeX` int(4) NOT NULL,
  `NodeY` int(4) NOT NULL,
  PRIMARY KEY (`PositionID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of nodeinfo
-- ----------------------------

-- ----------------------------
-- Table structure for nodelog
-- ----------------------------
DROP TABLE IF EXISTS `nodelog`;
CREATE TABLE `nodelog` (
  `LogID` int(11) NOT NULL AUTO_INCREMENT,
  `PositionID` varchar(10) DEFAULT NULL,
  `DeviceName` varchar(20) DEFAULT NULL,
  `NodeName` varchar(20) DEFAULT NULL,
  `NodeValue` varchar(5) DEFAULT NULL,
  `NodeSign` varchar(5) DEFAULT NULL,
  `SaveTime` varchar(19) DEFAULT NULL,
  PRIMARY KEY (`LogID`),
  KEY `NodeLogIndex` (`DeviceName`,`SaveTime`,`NodeName`,`LogID`,`PositionID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of nodelog
-- ----------------------------

-- ----------------------------
-- Table structure for portinfo
-- ----------------------------
DROP TABLE IF EXISTS `portinfo`;
CREATE TABLE `portinfo` (
  `PortID` int(11) NOT NULL,
  `PortName` varchar(30) NOT NULL,
  `ComName` varchar(4) DEFAULT NULL,
  `BaudRate` int(6) DEFAULT NULL,
  `TcpIP` varchar(16) DEFAULT NULL,
  `TcpPort` int(6) DEFAULT NULL,
  `ReadInterval` int(4) NOT NULL,
  `ReadTimeout` int(4) NOT NULL,
  `ReadMaxtime` int(4) NOT NULL,
  PRIMARY KEY (`PortID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of portinfo
-- ----------------------------

-- ----------------------------
-- Table structure for typeinfo
-- ----------------------------
DROP TABLE IF EXISTS `typeinfo`;
CREATE TABLE `typeinfo` (
  `TypeID` int(11) NOT NULL,
  `DeviceType` varchar(20) DEFAULT NULL,
  `NodeNumber` int(3) DEFAULT NULL,
  `NodeType` varchar(20) DEFAULT NULL,
  `NodeClass` varchar(10) DEFAULT NULL,
  `NodeSign` varchar(10) DEFAULT NULL,  
  PRIMARY KEY (`TypeID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of typeinfo
-- ----------------------------
INSERT INTO `typeinfo` VALUES (1, 'FC-1003-1', 1, 'FE-1102', 'GAS', 'PPM');
INSERT INTO `typeinfo` VALUES (2, 'FC-1003-8', 8, 'FE-2102', 'O2', '%LEL');
INSERT INTO `typeinfo` VALUES (3, 'FC-1003-16', 16, 'FT-1104', 'CO', '%VOL');
INSERT INTO `typeinfo` VALUES (4, 'FC-1201显示板', 256, 'FT-2104', 'H2S', 'umol/mol');
INSERT INTO `typeinfo` VALUES (5, 'FC-1201采样板', 4, 'FT-2104PLUS', 'C6H6', 'mg/m3');
INSERT INTO `typeinfo` VALUES (6, 'FC-1201输出板', 256, 'FT-2104泵吸式', 'NH3', 'mA');
INSERT INTO `typeinfo` VALUES (7, 'FC-1201通信板', 256, 'FT-2104拓展型', 'SO2', 'mV');
INSERT INTO `typeinfo` VALUES (8, 'FT-2104P', 1, 'FT-2104单点型', 'NO', '℃');
INSERT INTO `typeinfo` VALUES (9, 'FT-2104P传感器组件', 64, 'EA-08', 'NO2', NULL);
INSERT INTO `typeinfo` VALUES (10, 'SAMS-4128', 8, 'PT-1700', 'CL2', NULL);
INSERT INTO `typeinfo` VALUES (11, 'SAMS-423C', 12, 'PE-1100', 'HCLHCN', NULL);
INSERT INTO `typeinfo` VALUES (12, 'PT-5000', 5, 'PT-4100', 'CLO2', NULL);
INSERT INTO `typeinfo` VALUES (13, NULL, NULL, NULL, 'COCL2', NULL);
INSERT INTO `typeinfo` VALUES (14, NULL, NULL, NULL, 'PH3', NULL);
INSERT INTO `typeinfo` VALUES (15, NULL, NULL, NULL, 'ETO', NULL);
INSERT INTO `typeinfo` VALUES (16, NULL, NULL, NULL, 'H2', NULL);
INSERT INTO `typeinfo` VALUES (17, NULL, NULL, NULL, 'O3', NULL);
INSERT INTO `typeinfo` VALUES (18, NULL, NULL, NULL, 'CH', NULL);
INSERT INTO `typeinfo` VALUES (19, NULL, NULL, NULL, 'CH4', NULL);
INSERT INTO `typeinfo` VALUES (20, NULL, NULL, NULL, 'HF', NULL);
INSERT INTO `typeinfo` VALUES (21, NULL, NULL, NULL, 'OTHER', NULL);

-- ----------------------------
-- Table structure for userinfo
-- ----------------------------
DROP TABLE IF EXISTS `userinfo`;
CREATE TABLE `userinfo` (
  `UserName` varchar(10) NOT NULL DEFAULT '',
  `UserPwd` varchar(20) DEFAULT NULL,
  `UserType` varchar(10) DEFAULT NULL,
  `UserAdmin` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`UserName`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of userinfo
-- ----------------------------
INSERT INTO `userinfo` VALUES ('admin', 'admin', '管理员', '');
INSERT INTO `userinfo` VALUES ('feiyang', '123456', '操作员', '');

-- ----------------------------
-- Table structure for userlog
-- ----------------------------
DROP TABLE IF EXISTS `userlog`;
CREATE TABLE `userlog` (
  `LogID` int(11) NOT NULL AUTO_INCREMENT,
  `TriggerTime` varchar(19) DEFAULT NULL,
  `UserName` varchar(10) DEFAULT NULL,
  `UserType` varchar(6) DEFAULT NULL,
  `TriggerType` varchar(10) DEFAULT NULL,
  `TriggerContent` varchar(15) DEFAULT NULL,
  PRIMARY KEY (`LogID`),
  KEY `UserLogIndex` (`TriggerTime`,`UserName`,`LogID`,`TriggerType`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of userlog
-- ----------------------------
