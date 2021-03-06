-- phpMyAdmin SQL Dump
-- version 3.4.10.1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Erstellungszeit: 07. Aug 2013 um 15:27
-- Server Version: 5.1.66
-- PHP-Version: 5.3.3-7+squeeze15

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Clean old stuff first
--

DROP TABLE IF EXISTS `DVDAUDIOSTREAM`;
DROP TABLE IF EXISTS `DVDAUDIOSTREAMEX`;
DROP TABLE IF EXISTS `DVDSUBPICSTREAM`;
DROP TABLE IF EXISTS `DVDVIDEOSTREAM`;
DROP TABLE IF EXISTS `DVDFILE`;
DROP TABLE IF EXISTS `DVDPTTVTS`;
DROP TABLE IF EXISTS `DVDPTTVMG`;
DROP TABLE IF EXISTS `DVDUNIT`;
DROP TABLE IF EXISTS `DVDCELL`;
DROP TABLE IF EXISTS `DVDPROGRAM`;
DROP TABLE IF EXISTS `DVDPGC`;
DROP TABLE IF EXISTS `DVDVTS`;
DROP TABLE IF EXISTS `DVDVMGM`;
DROP VIEW IF EXISTS `vDVDCHAPTERLIST`;
DROP VIEW IF EXISTS `vDVDSTATS`;


--
-- Datenbank: `dbdvdetect`
--

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `DVDAUDIOSTREAM`
--

CREATE TABLE IF NOT EXISTS `DVDAUDIOSTREAM` (
  `idDVDAUDIOSTREAM` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary key',
  `DVDVMGMKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDVMGM',
  `DVDVTSKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDVTS',
  `Number` int(11) DEFAULT NULL COMMENT 'Audio Stream Number',
  `Type` char(10) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Type of stream VMG (Video Manager), VTSM (Video Title Manager), VTS (Video Title)',
  `ID` int(11) DEFAULT NULL COMMENT 'Stream ID',
  `CodingMode` varchar(10) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Audio coding mode (AC3, MPEG1, MPEG2, LPCM, DTS)',
  `MultichannelExtPresent` bit(1) DEFAULT b'0' COMMENT 'True if multichannel extension present',
  `Quantisation` char(10) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Quantisation: 16/20/24 bits per sample or DRC',
  `SampleRate` int(11) DEFAULT NULL COMMENT 'Sample rate: 48 Kbps or 96 Kbps',
  `Channels` int(11) DEFAULT NULL COMMENT 'Number of channels (1...6)',
  `RowLastChanged` datetime DEFAULT NULL COMMENT 'Date and time row was last changed',
  `RowCreationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Date and time row was created',
  PRIMARY KEY (`idDVDAUDIOSTREAM`),
  KEY `FK_DVDAUDIOSTREAM_DVDKEY` (`DVDVMGMKey`),
  KEY `FK_DVDAUDIOSTREAM_DVDVTS` (`DVDVTSKey`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Audio tracks on DVD (basic information)' AUTO_INCREMENT=1 ;

--
-- Trigger `DVDAUDIOSTREAM`
--
DROP TRIGGER IF EXISTS `TR_DVDAUDIOSTREAM_UPDATE`;
DELIMITER //
CREATE TRIGGER `TR_DVDAUDIOSTREAM_UPDATE` BEFORE UPDATE ON `DVDAUDIOSTREAM`
 FOR EACH ROW BEGIN
    SET NEW.RowLastChanged = CURRENT_TIMESTAMP;
END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `DVDAUDIOSTREAMEX`
--

CREATE TABLE IF NOT EXISTS `DVDAUDIOSTREAMEX` (
  `idDVDAUDIOSTREAMEX` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary key',
  `DVDVTSKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDVTS',
  `Number` int(11) DEFAULT NULL COMMENT 'Audio Stream Number',
  `LanguageCodePresent` bit(1) DEFAULT b'0' COMMENT 'Language code is present',
  `LanguageCode` char(2) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '2 byte language code, e.g. de, en',
  `CodeExtPresent` bit(1) DEFAULT b'0' COMMENT 'Language code extention is present',
  `CodeExt` int(11) DEFAULT NULL COMMENT 'unspecified/normal/for visually impaired/director''s comments/alternate director''s comments',
  `ChannelAssignment` int(11) DEFAULT NULL COMMENT 'Channel Assignment',
  `MCIntroPresent` bit(1) DEFAULT b'0' COMMENT 'MC Intro Present',
  `KaraokeVersion` int(11) DEFAULT NULL COMMENT 'Karaoke version',
  `ApplicationMode` char(12) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Audio application mode: undefined, karaoke, surround',
  `KaraokeMode` int(11) DEFAULT NULL COMMENT 'Karaoke mode: solo or duet',
  `SuitableForDolbySurroundDecoding` bit(1) DEFAULT b'0' COMMENT 'Suitable for Dolby surround decoding',
  `RowLastChanged` datetime DEFAULT NULL COMMENT 'Date and time row was last changed',
  `RowCreationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Date and time row was created',
  PRIMARY KEY (`idDVDAUDIOSTREAMEX`),
  KEY `FK_DVDAUDIOSTREAMEX_DVDVTS` (`DVDVTSKey`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Audio tracks on DVD (extended information, VTS only)' AUTO_INCREMENT=1 ;

--
-- Trigger `DVDAUDIOSTREAMEX`
--
DROP TRIGGER IF EXISTS `TR_DVDAUDIOSTREAMEX_UPDATE`;
DELIMITER //
CREATE TRIGGER `TR_DVDAUDIOSTREAMEX_UPDATE` BEFORE UPDATE ON `DVDAUDIOSTREAMEX`
 FOR EACH ROW BEGIN
    SET NEW.RowLastChanged = CURRENT_TIMESTAMP;
END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `DVDCELL`
--

CREATE TABLE IF NOT EXISTS `DVDCELL` (
  `idDVDCELL` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary key',
  `DVDPROGRAMKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDVMGM (Owner program)',
  `CellNo` int(11) DEFAULT NULL COMMENT 'Cell number',
  `CellType` char(20) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Cell type: normal, first, middle, last',
  `BlockType` char(20) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'normal/angle block',
  `SeamlessMultiplex` bit(1) NOT NULL DEFAULT b'1' COMMENT 'seamless playback linked in PCI',
  `Interleaved` bit(1) NOT NULL DEFAULT b'1' COMMENT 'interleaved',
  `SCRdiscontinuity` bit(1) NOT NULL DEFAULT b'1' COMMENT 'STC discontinuity',
  `SeamlessAngleLinkedInDSI` bit(1) NOT NULL DEFAULT b'1' COMMENT 'eamless angle linked in DSI',
  `VOBStillMode` bit(1) NOT NULL DEFAULT b'1' COMMENT 'VOBStillMode',
  `StopsTrickPlay` bit(1) NOT NULL DEFAULT b'1' COMMENT 'restricted (stops trick play)',
  `CellStillTime` int(11) DEFAULT NULL COMMENT 'Cell still time',
  `CellCommand` int(11) DEFAULT NULL COMMENT 'Cell command #',
  `PlayTime` int(11) DEFAULT NULL COMMENT 'Cell playback time in ms',
  `FrameRate` int(11) DEFAULT NULL COMMENT 'Frame rate 30 fps/25 fps',
  `FirstVOBUStartSector` int(11) DEFAULT NULL COMMENT 'First VOBU Start Sector',
  `FirstILVUEndSector` int(11) DEFAULT NULL COMMENT 'First ILVU End Sector',
  `LastVOBUStartSector` int(11) DEFAULT NULL COMMENT 'Last VOBU Start Sector',
  `LastVOBUEndSector` int(11) DEFAULT NULL COMMENT 'Last VOBU End Sector',
  `VOBidn` int(11) DEFAULT NULL COMMENT 'VOB (Video Object Unit) id',
  `CELLidn` int(11) DEFAULT NULL COMMENT 'Cell id',
  `NumberOfVOBIds` int(11) DEFAULT NULL COMMENT 'number of VOB IDs',
  `RowLastChanged` datetime DEFAULT NULL COMMENT 'Date and time row was last changed',
  `RowCreationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Date and time row was created',
  PRIMARY KEY (`idDVDCELL`),
  KEY `FK_DVDCELL_DVDPROGRAM` (`DVDPROGRAMKey`),
  KEY `IX_DVDCELL_CELLTYPE` (`CellType`),
  KEY `IX_DVDCELL_BLOCKTYPE` (`BlockType`),
  KEY `IX_DVDCELL_CELLNO` (`CellNo`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

--
-- Trigger `DVDCELL`
--
DROP TRIGGER IF EXISTS `TR_DVDCELL_UPDATE`;
DELIMITER //
CREATE TRIGGER `TR_DVDCELL_UPDATE` BEFORE UPDATE ON `DVDCELL`
 FOR EACH ROW BEGIN
     SET NEW.RowLastChanged = CURRENT_TIMESTAMP;
 END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `DVDFILE`
--

CREATE TABLE IF NOT EXISTS `DVDFILE` (
  `idDVDFILE` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary key',
  `DVDVMGMKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDVMGM',
  `DVDVTSKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDVTS',
  `FileSetNo` int(11) DEFAULT NULL COMMENT 'Fileset no',
  `Type` char(10) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'File type (IFO, VOB, etc.)',
  `VobNo` int(11) DEFAULT NULL COMMENT 'For VOB files: number (0...9)',
  `Size` int(11) DEFAULT NULL COMMENT 'Size of file',
  `Date` datetime DEFAULT NULL COMMENT 'File date',
  `RowLastChanged` datetime DEFAULT NULL COMMENT 'Date and time row was last changed',
  `RowCreationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Date and time row was created',
  PRIMARY KEY (`idDVDFILE`),
  KEY `FK_DVDFILE_DVDVMGM` (`DVDVMGMKey`),
  KEY `FK_DVDFILE_DVDVTS` (`DVDVTSKey`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

--
-- Trigger `DVDFILE`
--
DROP TRIGGER IF EXISTS `TR_DVDFILE_UPDATE`;
DELIMITER //
CREATE TRIGGER `TR_DVDFILE_UPDATE` BEFORE UPDATE ON `DVDFILE`
 FOR EACH ROW BEGIN
    SET NEW.RowLastChanged = CURRENT_TIMESTAMP;
END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `DVDPGC`
--

CREATE TABLE IF NOT EXISTS `DVDPGC` (
  `idDVDPGC` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary key',
  `DVDVTSKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDVTS (Chapter/owner title number)',
  `ProgramChainNo` int(11) DEFAULT NULL COMMENT 'PGC number',
  `EntryPGC` bit(1) DEFAULT b'0' COMMENT 'entry PGC',
  `RowLastChanged` datetime DEFAULT NULL COMMENT 'Date and time row was last changed',
  `RowCreationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Date and time row was created',
  PRIMARY KEY (`idDVDPGC`),
  KEY `FK_DVDPGC_DVDVTS` (`DVDVTSKey`),
  KEY `IX_DVDPGC_PROGRAMCHAINNO` (`ProgramChainNo`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='DVD program chains (physical structure of DVD)' AUTO_INCREMENT=1 ;

--
-- Trigger `DVDPGC`
--
DROP TRIGGER IF EXISTS `TR_DVDPGC_UPDATE`;
DELIMITER //
CREATE TRIGGER `TR_DVDPGC_UPDATE` BEFORE UPDATE ON `DVDPGC`
 FOR EACH ROW BEGIN
    SET NEW.RowLastChanged = CURRENT_TIMESTAMP;
END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `DVDPROGRAM`
--

CREATE TABLE IF NOT EXISTS `DVDPROGRAM` (
  `idDVDPROGRAM` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary key',
  `DVDPGCKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDPGC (Owner program chain (PGC) number)',
  `ProgramNo` int(11) DEFAULT NULL COMMENT 'Program number',
  `RowLastChanged` datetime DEFAULT NULL COMMENT 'Date and time row was last changed',
  `RowCreationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Date and time row was created',
  PRIMARY KEY (`idDVDPROGRAM`),
  KEY `FK_DVDPROGRAM_DVDPGC` (`DVDPGCKey`),
  KEY `IX_DVDPROGRAM_PROGRAMNO` (`ProgramNo`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='DVD programs (physical structure of DVD)' AUTO_INCREMENT=1 ;

--
-- Trigger `DVDPROGRAM`
--
DROP TRIGGER IF EXISTS `TR_DVDPROGRAM_UPDATE`;
DELIMITER //
CREATE TRIGGER `TR_DVDPROGRAM_UPDATE` BEFORE UPDATE ON `DVDPROGRAM`
 FOR EACH ROW BEGIN
    SET NEW.RowLastChanged = CURRENT_TIMESTAMP;
END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `DVDPTTVMG`
--

CREATE TABLE IF NOT EXISTS `DVDPTTVMG` (
  `idDVDPTTVMG` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary key',
  `DVDVMGMKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDVMGM',
  `TitleSetNo` int(11) DEFAULT NULL COMMENT 'Number of this (virtual) title set',
  `Title` varchar(120) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Title',
  `PlaybackType` int(11) DEFAULT NULL COMMENT 'Playback type',
  `NumberOfVideoAngles` int(11) DEFAULT NULL COMMENT 'Number of video angles (1...9)',
  `ParentalMgmMaskVMG` int(11) DEFAULT NULL COMMENT 'Parental management mask for VMG',
  `ParentalMgmMaskVTS` int(11) DEFAULT NULL COMMENT 'Parental management mask for VTS',
  `VideoTitleSetNo` int(11) DEFAULT NULL COMMENT 'Video title set having this video title',
  `TitleNo` int(11) DEFAULT NULL COMMENT 'Title number within its video title set (VTS)',
  `RowLastChanged` datetime DEFAULT NULL COMMENT 'Date and time row was last changed',
  `RowCreationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Date and time row was created',
  PRIMARY KEY (`idDVDPTTVMG`),
  KEY `FK_DVDPTTVMG_DVDKEY` (`DVDVMGMKey`),
  KEY `IX_DVDPTTVMG_TITLE` (`Title`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Virtual title on DVD' AUTO_INCREMENT=1 ;

--
-- Trigger `DVDPTTVMG`
--
DROP TRIGGER IF EXISTS `TR_DVDPTTVMG_UPDATE`;
DELIMITER //
CREATE TRIGGER `TR_DVDPTTVMG_UPDATE` BEFORE UPDATE ON `DVDPTTVMG`
 FOR EACH ROW BEGIN
    SET NEW.RowLastChanged = CURRENT_TIMESTAMP;
END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `DVDPTTVTS`
--

CREATE TABLE IF NOT EXISTS `DVDPTTVTS` (
  `idDVDPTTVTS` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary key',
  `DVDPTTVMGKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDPTTVMG',
  `Artist` varchar(120) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Artist name',
  `Title` varchar(120) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Title name',
  `TitleSetNo` int(11) DEFAULT NULL COMMENT 'Owner title number',
  `ProgramChainNo` int(11) DEFAULT NULL COMMENT '(Real) Program Chain (PGCN)',
  `ProgramNo` int(11) DEFAULT NULL COMMENT '(Real) Program (PGN)',
  `PttTitleSetNo` int(11) DEFAULT NULL COMMENT 'PTT (virtual) title number',
  `PttChapterNo` int(11) DEFAULT NULL COMMENT 'PTT (virtual) chapter number',
  `RowLastChanged` datetime DEFAULT NULL COMMENT 'Date and time row was last changed',
  `RowCreationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Date and time row was created',
  PRIMARY KEY (`idDVDPTTVTS`),
  UNIQUE KEY `UQ_DVDPTTVTS_COMBINED` (`DVDPTTVMGKey`,`TitleSetNo`,`ProgramChainNo`,`ProgramNo`),
  KEY `FK_DVDPTTVTS_DVDPTTVMG` (`DVDPTTVMGKey`),
  KEY `IX_DVDPTTVTS_PTTCHAPTERNO` (`PttChapterNo`),
  KEY `IX_DVDPTTVTS_TITLE` (`Title`),
  KEY `IX_DVDPTTVTS_ARTIST` (`Artist`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='DVDPTTVTS' AUTO_INCREMENT=1 ;

--
-- Trigger `DVDPTTVTS`
--
DROP TRIGGER IF EXISTS `TR_DVDPTTVTS_UPDATE`;
DELIMITER //
CREATE TRIGGER `TR_DVDPTTVTS_UPDATE` BEFORE UPDATE ON `DVDPTTVTS`
 FOR EACH ROW BEGIN
    SET NEW.RowLastChanged = CURRENT_TIMESTAMP;
END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `DVDSUBPICSTREAM`
--

CREATE TABLE IF NOT EXISTS `DVDSUBPICSTREAM` (
  `idDVDSUBPICSTREAM` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary key',
  `DVDVMGMKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDVMGM',
  `DVDVTSKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDVTS',
  `Number` int(11) DEFAULT NULL COMMENT 'Number',
  `Type` char(10) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Type of stream VMG (Video Manager), VTSM (Video Title Manager), VTS (Video Title)',
  `ID` int(11) DEFAULT NULL COMMENT 'Subpicture steam ID, 0x20 + (Stream # - 1)',
  `CodingMode` int(11) DEFAULT NULL COMMENT 'Subtitle coding mode',
  `LanguageCodePresent` bit(1) DEFAULT b'0' COMMENT 'Language code is present',
  `LanguageCode` char(2) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '2 byte language code, e.g. de, en',
  `CodeExtPresent` bit(1) DEFAULT b'0' COMMENT 'Language code extention is present',
  `CodeExt` int(11) DEFAULT NULL COMMENT 'not specified/normal/large/children/normal captions/large captions/children’s captions/forced/director comments/large director comments/director comments for children',
  `RowLastChanged` datetime DEFAULT NULL COMMENT 'Date and time row was last changed',
  `RowCreationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Date and time row was created',
  PRIMARY KEY (`idDVDSUBPICSTREAM`),
  KEY `FK_DVDSUBPICSTREAM_DVDKEY` (`DVDVMGMKey`),
  KEY `FK_DVDSUBPICSTREAM_DVDVTS` (`DVDVTSKey`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Subtitles on DVD' AUTO_INCREMENT=1 ;

--
-- Trigger `DVDSUBPICSTREAM`
--
DROP TRIGGER IF EXISTS `TR_DVDSUBPICSTREAM_UPDATE`;
DELIMITER //
CREATE TRIGGER `TR_DVDSUBPICSTREAM_UPDATE` BEFORE UPDATE ON `DVDSUBPICSTREAM`
 FOR EACH ROW BEGIN
    SET NEW.RowLastChanged = CURRENT_TIMESTAMP;
END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `DVDUNIT`
--

CREATE TABLE IF NOT EXISTS `DVDUNIT` (
  `idDVDUNIT` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary key',
  `DVDCELLKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDCELL',
  `UnitNo` int(11) DEFAULT NULL COMMENT 'Cell number',
  `StartSector` int(11) DEFAULT NULL COMMENT 'Unit start sector',
  `EndSector` int(11) DEFAULT NULL COMMENT 'Unit end sector',
  `RowLastChanged` datetime DEFAULT NULL COMMENT 'Date and time row was last changed',
  `RowCreationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Date and time row was created',
  PRIMARY KEY (`idDVDUNIT`),
  KEY `FK_DVDUNIT_DVDCELL` (`DVDCELLKey`),
  KEY `IX_DVDUNIT_UNITNO` (`UnitNo`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

--
-- Trigger `DVDUNIT`
--
DROP TRIGGER IF EXISTS `TR_DVDUNIT_UPDATE`;
DELIMITER //
CREATE TRIGGER `TR_DVDUNIT_UPDATE` BEFORE UPDATE ON `DVDUNIT`
 FOR EACH ROW BEGIN
     SET NEW.RowLastChanged = CURRENT_TIMESTAMP;
 END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `DVDVIDEOSTREAM`
--

CREATE TABLE IF NOT EXISTS `DVDVIDEOSTREAM` (
  `idDVDVIDEOSTREAM` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary key',
  `DVDVMGMKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDVMGM',
  `DVDVTSKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDVTS',
  `Type` char(10) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Type of stream VMG (Video Manager), VTSM (Video Title Manager), VTS (Video Title)',
  `ID` int(11) DEFAULT NULL COMMENT 'Video stream ID, fixed to 0xE0',
  `VideoCodingMode` char(10) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Video coding mode (MPEG-1 or MPEG-2)',
  `VideoStandard` char(5) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Video standard (NTSC, PAL)',
  `VideoAspect` char(5) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Video aspect ratio (16:9, 4:3)',
  `AutomaticPanScanDisallowed` bit(1) DEFAULT NULL COMMENT 'Automatic pan scan allowed',
  `AutomaticLetterboxDisallowed` bit(1) DEFAULT NULL COMMENT 'Automatic Letterbox',
  `CCForLine21Field1InGOP` bit(1) DEFAULT NULL COMMENT 'NTSC only: Closed Caption for line 21 field 1 in GOP (Group of Pictures)',
  `CCForLine21Field2InGOP` bit(1) DEFAULT NULL COMMENT 'NTSC only: Closed Caption for line 21 field 2 in GOP (Group of Pictures)',
  `CBR` bit(1) DEFAULT NULL COMMENT 'Constant bitrate',
  `Resolution` char(8) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Resolution (width): possible values are NTSC (PAL) 720x480 (720x576), 704x480 (704x576), 352x480 (352x576), 352x240 (352x288)',
  `LetterBoxed` bit(1) DEFAULT NULL COMMENT 'false = Full Screen; true = top and bottom cropped (4:3 aspect only)',
  `SourceFilm` bit(1) DEFAULT NULL COMMENT 'Source: false = camera, true = film',
  `RowLastChanged` datetime DEFAULT NULL COMMENT 'Date and time row was last changed',
  `RowCreationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Date and time row was created',
  PRIMARY KEY (`idDVDVIDEOSTREAM`),
  KEY `FK_DVDVIDEOSTREAM_DVDKEY` (`DVDVMGMKey`),
  KEY `FK_DVDVIDEOSTREAM_DVDVTS` (`DVDVTSKey`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Subtitles on DVD' AUTO_INCREMENT=1 ;

--
-- Trigger `DVDVIDEOSTREAM`
--
DROP TRIGGER IF EXISTS `TR_DVDVIDEOSTREAM_UPDATE`;
DELIMITER //
CREATE TRIGGER `TR_DVDVIDEOSTREAM_UPDATE` BEFORE UPDATE ON `DVDVIDEOSTREAM`
 FOR EACH ROW BEGIN
    SET NEW.RowLastChanged = CURRENT_TIMESTAMP;
END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `DVDVMGM`
--

CREATE TABLE IF NOT EXISTS `DVDVMGM` (
  `idDVDVMGM` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary key',
  `Hash` char(48) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Identification hash',
  `Album` varchar(120) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Album name',
  `OriginalAlbum` varchar(120) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Original language title/album name',
  `AlbumArtist` varchar(120) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Album artist',
  `Genre` varchar(120) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Album genre',
  `Keywords` varchar(40) COLLATE utf8_unicode_ci DEFAULT NULL,
  `Cover` blob COMMENT 'Cover Art',
  `Cast` text COLLATE utf8_unicode_ci COMMENT 'Cast',
  `Crew` varchar(120) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Crew',
  `Director` varchar(120) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Director',
  `Screenplay` varchar(120) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Storyline writer',
  `Producer` varchar(120) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Produced by',
  `Editing` varchar(120) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Editing by',
  `Cinematography` varchar(120) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Cinematography/camera by',
  `Country` varchar(120) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Country',
  `OriginalLanguage` varchar(80) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Original Language',
  `ReleaseDate` date DEFAULT NULL COMMENT 'Release Date',
  `SpecialFeatures` varchar(120) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'Special Features',
  `EAN_UPC` varchar(13) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'DVD EAN/UPC',
  `Storyline` text COLLATE utf8_unicode_ci COMMENT 'Storyline/Synopsis',
  `Remarks` text COLLATE utf8_unicode_ci COMMENT 'Additional remarks',
  `Submitter` varchar(80) COLLATE utf8_unicode_ci DEFAULT NULL,
  `SubmitterIP` varchar(40) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'IP adress of submitter',
  `Client` varchar(80) COLLATE utf8_unicode_ci DEFAULT NULL,
  `VersionNumberMajor` int(11) DEFAULT NULL COMMENT 'Major version number',
  `VersionNumberMinor` int(11) DEFAULT NULL COMMENT 'Minor version number',
  `RegionProhibited1` bit(1) NOT NULL DEFAULT b'1' COMMENT 'Prohibited region: 1',
  `RegionProhibited2` bit(1) NOT NULL DEFAULT b'1' COMMENT 'Prohibited region: 2',
  `RegionProhibited3` bit(1) NOT NULL DEFAULT b'1' COMMENT 'Prohibited region: 3',
  `RegionProhibited4` bit(1) NOT NULL DEFAULT b'1' COMMENT 'Prohibited region: 4',
  `RegionProhibited5` bit(1) NOT NULL DEFAULT b'1' COMMENT 'Prohibited region: 5',
  `RegionProhibited6` bit(1) NOT NULL DEFAULT b'1' COMMENT 'Prohibited region: 6',
  `RegionProhibited7` bit(1) NOT NULL DEFAULT b'1' COMMENT 'Prohibited region: 7',
  `RegionProhibited8` bit(1) NOT NULL DEFAULT b'1' COMMENT 'Prohibited region: 8',
  `NumberOfVolumes` int(11) DEFAULT NULL COMMENT 'Number of volumes',
  `VolumeNumber` int(11) DEFAULT NULL COMMENT 'Volume number',
  `SideID` int(11) DEFAULT NULL COMMENT 'Side ID',
  `Active` bit(1) NOT NULL DEFAULT b'1' COMMENT 'Active revision',
  `Revision` int(11) NOT NULL DEFAULT '1' COMMENT 'Revision number',
  `RowLastChanged` datetime DEFAULT NULL COMMENT 'Date and time row was last changed',
  `RowCreationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Date and time row was created',
  PRIMARY KEY (`idDVDVMGM`),
  KEY `IX_DVDVMGM_HASH` (`Hash`),
  KEY `IX_DVDVMGM_ALBUM` (`Album`),
  KEY `IX_DVDVMGM_ALBUMARTIST` (`AlbumArtist`),
  KEY `IX_DVDVMGM_GENRE` (`Genre`),
  KEY `IX_DVDVMGM_KEYWORDS` (`Keywords`),
  KEY `IX_DVDVMGM_CREW` (`Crew`),
  KEY `IX_DVDVMGM_DIRECTOR` (`Director`),
  KEY `IX_DVDVMGM_COUNTRY` (`Country`),
  KEY `IX_DVDVMGM_RELEASEDATE` (`ReleaseDate`),
  KEY `IX_DVDVMGM_SPECIALFEATURES` (`SpecialFeatures`),
  KEY `IX_DVDVMGM_EAN_UPC` (`EAN_UPC`),
  KEY `IX_DVDVMGM_SUBMITTER` (`Submitter`),
  KEY `IX_DVDVMGM_CLIENT` (`Client`),
  KEY `IX_DVDVMGM_ORIGINALALBUM` (`OriginalAlbum`),
  KEY `IX_DVDVMGM_SCREENPLAY` (`Screenplay`),
  KEY `IX_DVDVMGM_PRODUCER` (`Producer`),
  KEY `IX_DVDVMGM_EDITING` (`Editing`),
  KEY `IX_DVDVMGM_ORIGINALLANGUAGE` (`OriginalLanguage`),
  KEY `IX_DVDVMGM_CINEMATOGRAPHY` (`Cinematography`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='DVD main list' AUTO_INCREMENT=1 ;

--
-- Trigger `DVDVMGM`
--
DROP TRIGGER IF EXISTS `TR_DVDVMGM_INSERT`;
DELIMITER //
CREATE TRIGGER `TR_DVDVMGM_INSERT` BEFORE INSERT ON `DVDVMGM`
 FOR EACH ROW BEGIN
    SET NEW.Revision = (SELECT COALESCE(max(Revision) + 1, 1) FROM `DVDVMGM` WHERE `Hash` = NEW.Hash);
END
//
DELIMITER ;
DROP TRIGGER IF EXISTS `TR_DVDVMGM_UPDATE`;
DELIMITER //
CREATE TRIGGER `TR_DVDVMGM_UPDATE` BEFORE UPDATE ON `DVDVMGM`
 FOR EACH ROW BEGIN
    SET NEW.RowLastChanged = CURRENT_TIMESTAMP;
    SET NEW.Revision = (SELECT COALESCE(max(Revision) + 1, 1) FROM `DVDVMGM` WHERE `Hash` = NEW.Hash);
END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `DVDVTS`
--

CREATE TABLE IF NOT EXISTS `DVDVTS` (
  `idDVDVTS` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary key',
  `DVDVMGMKey` int(11) DEFAULT NULL COMMENT 'Foreign key into DVDVMGM',
  `TitleSetNo` int(11) DEFAULT NULL COMMENT 'Number of title set',
  `VersionNumberMajor` int(11) DEFAULT NULL COMMENT 'Major version number',
  `VersionNumberMinor` int(11) DEFAULT NULL COMMENT 'Minor version number',
  `RowLastChanged` datetime DEFAULT NULL COMMENT 'Date and time row was last changed',
  `RowCreationDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Date and time row was created',
  PRIMARY KEY (`idDVDVTS`),
  KEY `FK_DVDVTS_DVDVMGM` (`DVDVMGMKey`),
  KEY `IX_DVDVTS_TITLESETNO` (`TitleSetNo`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

--
-- Trigger `DVDVTS`
--
DROP TRIGGER IF EXISTS `TR_DVDVTS_UPDATE`;
DELIMITER //
CREATE TRIGGER `TR_DVDVTS_UPDATE` BEFORE UPDATE ON `DVDVTS`
 FOR EACH ROW BEGIN
    SET NEW.RowLastChanged = CURRENT_TIMESTAMP;
END
//
DELIMITER ;

-- --------------------------------------------------------

--
-- Stellvertreter-Struktur des Views `vDVDCHAPTERLIST`
--
CREATE TABLE IF NOT EXISTS `vDVDCHAPTERLIST` (
`idDVDPTTVMG` int(11)
,`DVDVMGMKey` int(11)
,`PTTVMGTitleSetNo` int(11)
,`PTTVMGTitle` varchar(120)
,`PlaybackType` int(11)
,`NumberOfVideoAngles` int(11)
,`ParentalMgmMaskVMG` int(11)
,`ParentalMgmMaskVTS` int(11)
,`VideoTitleSetNo` int(11)
,`TitleNo` int(11)
,`PTTVMGRowLastChanged` datetime
,`PTTVMGRowCreationDate` timestamp
,`idDVDPTTVTS` int(11)
,`Artist` varchar(120)
,`Title` varchar(120)
,`TitleSetNo` int(11)
,`ProgramChainNo` int(11)
,`ProgramNo` int(11)
,`PttTitleSetNo` int(11)
,`PttChapterNo` int(11)
,`RowLastChanged` datetime
,`RowCreationDate` timestamp
,`PlayTime` decimal(32,0)
);
-- --------------------------------------------------------

--
-- Struktur des Views `vDVDCHAPTERLIST`
--
DROP TABLE IF EXISTS `vDVDCHAPTERLIST`;

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `vDVDCHAPTERLIST` AS
SELECT
    DVDPTTVMG.idDVDPTTVMG AS idDVDPTTVMG,
    DVDPTTVMG.DVDVMGMKey AS DVDVMGMKey,
    DVDPTTVMG.TitleSetNo AS PTTVMGTitleSetNo,
    DVDPTTVMG.Title AS PTTVMGTitle,
    DVDPTTVMG.PlaybackType AS PlaybackType,
    DVDPTTVMG.NumberOfVideoAngles AS NumberOfVideoAngles,
    DVDPTTVMG.ParentalMgmMaskVMG AS ParentalMgmMaskVMG,
    DVDPTTVMG.ParentalMgmMaskVTS AS ParentalMgmMaskVTS,
    DVDPTTVMG.VideoTitleSetNo AS VideoTitleSetNo,
    DVDPTTVMG.TitleNo AS TitleNo,
    DVDPTTVMG.RowLastChanged AS PTTVMGRowLastChanged,
    DVDPTTVMG.RowCreationDate AS PTTVMGRowCreationDate,
    DVDPTTVTS.idDVDPTTVTS AS idDVDPTTVTS,
    DVDPTTVTS.Artist AS Artist,
    DVDPTTVTS.Title AS Title,
    DVDPTTVTS.TitleSetNo AS TitleSetNo,
    DVDPTTVTS.ProgramChainNo AS ProgramChainNo,
    DVDPTTVTS.ProgramNo AS ProgramNo,
    DVDPTTVTS.PttTitleSetNo AS PttTitleSetNo,
    DVDPTTVTS.PttChapterNo AS PttChapterNo,
    DVDPTTVTS.RowLastChanged AS RowLastChanged,
    DVDPTTVTS.RowCreationDate AS RowCreationDate,
    (SELECT
        SUM(DVDCELL.PlayTime)
    FROM
        (((DVDCELL
        JOIN DVDPROGRAM ON DVDCELL.DVDPROGRAMKey = DVDPROGRAM.idDVDPROGRAM)
        JOIN DVDPGC ON DVDPROGRAM.DVDPGCKey = DVDPGC.idDVDPGC)
        JOIN DVDVTS ON DVDPGC.DVDVTSKey = DVDVTS.idDVDVTS)
    WHERE
        ((DVDVTS.TitleSetNo = DVDPTTVTS.TitleSetNo) AND (DVDPGC.ProgramChainNo = DVDPTTVTS.ProgramChainNo) AND (DVDPROGRAM.ProgramNo = DVDPTTVTS.ProgramNo) AND ((DVDCELL.CellType = 'normal') OR (DVDCELL.CellType = 'first')) AND (DVDPTTVMG.DVDVMGMKey = DVDVTS.DVDVMGMKey))) AS PlayTime
FROM
    (DVDPTTVTS
    JOIN DVDPTTVMG ON DVDPTTVTS.DVDPTTVMGKey = DVDPTTVMG.idDVDPTTVMG);

-- --------------------------------------------------------

--
-- Struktur des Views `vDVDSTATS`
--

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `vDVDSTATS` AS
SELECT
    COUNT(0) AS DVDCount,
    COUNT(DISTINCT DVDVMGM.Album) AS DVDAlbumCount,
    COUNT(DISTINCT DVDVMGM.OriginalAlbum) AS DVDOriginalAlbumCount,
    COUNT(DISTINCT DVDVMGM.AlbumArtist) AS DVDAlbumArtistCount,
    COUNT(DISTINCT DVDVMGM.Genre) AS DVDGenreCount,
    COUNT(DISTINCT DVDVMGM.Director) AS DVDDirectorCount,
    COUNT(DISTINCT DVDVMGM.Screenplay) AS DVDScreenplayCount,
    COUNT(DISTINCT DVDVMGM.Producer) AS DVDProducerCount,
    COUNT(DISTINCT DVDVMGM.Editing) AS DVDEditingCount,
    COUNT(DISTINCT DVDVMGM.Cinematography) AS DVDCinematographyCount,
    COUNT(DISTINCT DVDVMGM.Country) AS DVDCountryCount,
    COUNT(DISTINCT DVDVMGM.OriginalLanguage) AS DVDOriginalLanguageCount,
    COUNT(DISTINCT DVDVMGM.Submitter) AS DVDSubmitterCount,
    COUNT(DISTINCT DVDVMGM.SubmitterIP) AS DVDSubmitterIPCount,
    COUNT(DISTINCT DVDVMGM.Client) AS DVDClientCount,
    MAX(DVDVMGM.RowLastChanged) AS LastChanged,
    MAX(DVDVMGM.RowCreationDate) AS LastAdded
FROM
    `DVDVMGM`;

--
-- Constraints der exportierten Tabellen
--

--
-- Constraints der Tabelle `DVDAUDIOSTREAM`
--
ALTER TABLE `DVDAUDIOSTREAM`
  ADD CONSTRAINT `FK_DVDAUDIOSTREAM_DVDVMGM` FOREIGN KEY (`DVDVMGMKey`) REFERENCES `DVDVMGM` (`idDVDVMGM`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `FK_DVDAUDIOSTREAM_DVDVTS` FOREIGN KEY (`DVDVTSKey`) REFERENCES `DVDVTS` (`idDVDVTS`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints der Tabelle `DVDAUDIOSTREAMEX`
--
ALTER TABLE `DVDAUDIOSTREAMEX`
  ADD CONSTRAINT `FK_DVDAUDIOSTREAMEX_DVDVTS` FOREIGN KEY (`DVDVTSKey`) REFERENCES `DVDVTS` (`idDVDVTS`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints der Tabelle `DVDCELL`
--
ALTER TABLE `DVDCELL`
  ADD CONSTRAINT `FK_DVDCELL_DVDPROGRAM` FOREIGN KEY (`DVDPROGRAMKey`) REFERENCES `DVDPROGRAM` (`idDVDPROGRAM`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints der Tabelle `DVDFILE`
--
ALTER TABLE `DVDFILE`
  ADD CONSTRAINT `FK_DVDFILE_DVDVMGM` FOREIGN KEY (`DVDVMGMKey`) REFERENCES `DVDVMGM` (`idDVDVMGM`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `FK_DVDFILE_DVDVTS` FOREIGN KEY (`DVDVTSKey`) REFERENCES `DVDVTS` (`idDVDVTS`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints der Tabelle `DVDPGC`
--
ALTER TABLE `DVDPGC`
  ADD CONSTRAINT `FK_DVDPGC_DVDVTS` FOREIGN KEY (`DVDVTSKey`) REFERENCES `DVDVTS` (`idDVDVTS`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints der Tabelle `DVDPROGRAM`
--
ALTER TABLE `DVDPROGRAM`
  ADD CONSTRAINT `FK_DVDPROGRAM_DVDPGC` FOREIGN KEY (`DVDPGCKey`) REFERENCES `DVDPGC` (`idDVDPGC`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints der Tabelle `DVDPTTVTS`
--
ALTER TABLE `DVDPTTVTS`
  ADD CONSTRAINT `FK_DVDPTTVTS_DVDPTTVMG` FOREIGN KEY (`DVDPTTVMGKey`) REFERENCES `DVDPTTVMG` (`idDVDPTTVMG`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints der Tabelle `DVDSUBPICSTREAM`
--
ALTER TABLE `DVDSUBPICSTREAM`
  ADD CONSTRAINT `FK_DVDSUBPICSTREAM_DVDVMGM` FOREIGN KEY (`DVDVMGMKey`) REFERENCES `DVDVMGM` (`idDVDVMGM`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `FK_DVDSUBPICSTREAM_DVDVTS` FOREIGN KEY (`DVDVTSKey`) REFERENCES `DVDVTS` (`idDVDVTS`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints der Tabelle `DVDUNIT`
--
ALTER TABLE `DVDUNIT`
  ADD CONSTRAINT `FK_DVDUNIT_DVDCELL` FOREIGN KEY (`DVDCELLKey`) REFERENCES `DVDCELL` (`idDVDCELL`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints der Tabelle `DVDVIDEOSTREAM`
--
ALTER TABLE `DVDVIDEOSTREAM`
  ADD CONSTRAINT `FK_DVDVIDEOSTREAM_DVDVMGM` FOREIGN KEY (`DVDVMGMKey`) REFERENCES `DVDVMGM` (`idDVDVMGM`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `FK_DVDVIDEOSTREAM_DVDVTS` FOREIGN KEY (`DVDVTSKey`) REFERENCES `DVDVTS` (`idDVDVTS`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints der Tabelle `DVDVTS`
--
ALTER TABLE `DVDVTS`
  ADD CONSTRAINT `FK_DVDVTS_DVDVMGM` FOREIGN KEY (`DVDVMGMKey`) REFERENCES `DVDVMGM` (`idDVDVMGM`) ON DELETE CASCADE ON UPDATE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
