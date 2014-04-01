<?php
/*
  dvdetect DVD detection, analysis & DVDETECT lookup library

  Copyright (C) 2013-2014 Norbert Schlia <nschlia@dvdetect.de>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*! \file functions.buildxml.inc.php
 *
 *  \brief PHP function collection
 */

if (!defined('PHP_VERSION_ID'))
{
    $version = explode('.', PHP_VERSION);

    define('PHP_VERSION_ID', ($version[0] * 10000 + $version[1] * 100 + $version[2]));
}

// Defines
define("LIBDVDETECT_COPYRIGHT",             "Copyright (C) 2013-2014 Norbert Schlia <nschlia@dvdetect.de>");
define("LIBDVDETECT_XML_COMMENT",           "Created with libdvdetect DVD analyser and lookup library");

define("DEFSUBMITTER",                      "***ChangeMe!***");     // Don't change this here, meant to be in XML exports :)

define("LIBDVDETECT_PROTOCOL_VERSION",      0);
define("LIBDVDETECT_VERSION",               "0.40 PHP " . PHP_VERSION);
define("LIBDVDETECT_LIBRARYNAME",           "libdvdetect library PHP");
define("LIBDVDETECT_CLIENTNAME",            "libDVDetect PHP server");

define("XMLMODE_INVALID",                   0);
define("XMLMODE_QUERY",                     1);
define("XMLMODE_QUERY_RESPONSE",            2);
define("XMLMODE_SEARCH",                    3);
define("XMLMODE_SEARCH_RESPONSE",           4);
define("XMLMODE_SUBMIT",                    5);
define("XMLMODE_SUBMIT_RESPONSE",           6);
define("XMLMODE_EXPORT",                    7);

define("XMLRESULT_SUCCESS",                 0);
define("XMLRESULT_NOT_FOUND",                1);
define("XMLRESULT_NOT_IMPLEMENTED",         2);
define("XMLRESULT_SQL_ERROR",               3);
define("XMLRESULT_DUPLICATE_SUBMISSION",    4);
define("XMLRESULT_XML_ERROR",               5);
define("XMLRESULT_UNSUPPORTED_VERSION",     6);

function checkVersion($ProtocolVersion, $MaxProtocolVersion)
{
    if ($ProtocolVersion == "")
    {
        $ResponseText = "Your client software does not support the current proctocol version. Please go to http://www.dvdetect.de/ and update.";
        throw new Exception($ResponseText, XMLRESULT_UNSUPPORTED_VERSION);
    }

    if ($ProtocolVersion > $MaxProtocolVersion)
    {
        $ResponseText = "Unsupported protocol version " . $ProtocolVersion . ", maximum supported is " . $MaxProtocolVersion . ". The server software is probably not up-to-date, please inform " . getenv("SERVER_ADMIN");
        throw new Exception($ResponseText, XMLRESULT_UNSUPPORTED_VERSION);
    }
}

function createXmlDocument(&$domtree, &$xmlRoot, $XmlMode)
{
    // create a dom document with encoding utf8
    $domtree = new DOMDocument('1.0', 'UTF-8');

    $domtree->formatOutput = true;

    // create the root element of the xml tree
    $xmlRoot = $domtree->createElement("xml");

    $comment = $domtree->createComment(LIBDVDETECT_COPYRIGHT);
    $xmlRoot->appendChild($comment);
    $comment = $domtree->createComment(LIBDVDETECT_XML_COMMENT);
    $xmlRoot->appendChild($comment);
    $comment = $domtree->createComment("libdvdetect data exchange");
    $xmlRoot->appendChild($comment);

    // append it to the document created
    $domtree->appendChild($xmlRoot);

    $xmlRoot->setAttribute("XmlMode", $XmlMode);
    $xmlRoot->setAttribute("ProtocolVersion", LIBDVDETECT_PROTOCOL_VERSION);
    $xmlRoot->setAttribute("LibraryVersion", LIBDVDETECT_VERSION);
    $xmlRoot->setAttribute("LibraryName", LIBDVDETECT_LIBRARYNAME);
    $xmlRoot->setAttribute("ClientName", LIBDVDETECT_CLIENTNAME);
}

function get_xml_error($xmlstr)
{
        if (is_array($xmlstr))
        {
                $xml = $xmlstr;
        }
        else
        {
                $xml = explode("\n", $xmlstr);
        }

        $xmlerrortext = "";

        $errors = libxml_get_errors();

        foreach ($errors as $error)
        {
                $xmlerrortext = eval_xml_error($error, $xml);
        }

        libxml_clear_errors();

        return $xmlerrortext;
}

function eval_xml_error($error, $xml)
{
        $return  = $xml[$error->line - 1] . "\n";
        $return .= str_repeat('-', $error->column) . "^\n";

        switch ($error->level)
        {
        case LIBXML_ERR_WARNING:
                $return .= "Warning $error->code: ";
                break;
        case LIBXML_ERR_ERROR:
                $return .= "Error $error->code: ";
                break;
        case LIBXML_ERR_FATAL:
                $return .= "Fatal Error $error->code: ";
                break;
        }

        $return .= trim($error->message) .
                        "\n  Line: $error->line" .
                        "\n  Column: $error->column";

        if ($error->file)
        {
                $return .= "\n  File: $error->file";
        }

    return $return;
}

function safeSetAttribute($Tag, $field, $value)
{
    if (isset($value))
    {
        $Tag->setAttribute($field, str_replace("\r\n", "\n", $value));
    }
}

function safeSetText($domtree, $Tag, $field, $value)
{
    if (isset($value))
    {
        $name = $Tag->appendChild($domtree->createElement($field));
        $name->appendChild($domtree->createCDATASection(str_replace("\r\n", "\n", $value)));
    }
}

function buildVideoStream($domtree, $tagOwner, $mysqli, $ProtocolVersion, $idDVDVMGM, $idDVDVTS)
{
    $strSQL = "SELECT `idDVDVIDEOSTREAM`, `Type`, `ID`, `VideoCodingMode`, `VideoStandard`, `VideoAspect`, `AutomaticPanScanDisallowed`, `CCForLine21Field1InGOP`, `CCForLine21Field2InGOP`, `CBR`, `Resolution`, `LetterBoxed`, `SourceFilm` FROM `DVDVIDEOSTREAM` ";

    if ($idDVDVMGM != null)
    {
        $strSQL .= "WHERE `DVDVMGMKey` = $idDVDVMGM AND `DVDVTSKey` IS NULL;";
    }
    else
    {
        $strSQL .= "WHERE DVDVMGMKey IS NULL AND DVDVTSKey = $idDVDVTS;";
    }

    $rs = query_server($mysqli, $strSQL);

    while (is_array($Cols = $rs->fetch_row()))
    {
        $Type                           = $Cols[1];
        $ID                             = $Cols[2];
        $VideoCodingMode                = $Cols[3];
        $VideoStandard                  = $Cols[4];
        $VideoAspect                    = $Cols[5];
        $AutomaticPanScanDisallowed     = $Cols[6];
        $CCForLine21Field1InGOP         = $Cols[7];
        $CCForLine21Field2InGOP         = $Cols[8];
        $CBR                            = $Cols[9];
        $Resolution                     = $Cols[10];
        $LetterBoxed                    = $Cols[11];
        $SourceFilm                     = $Cols[12];

        $tagDVDVIDEOSTREAM = $domtree->createElement("DVDVIDEOSTREAM");

        safeSetAttribute($tagDVDVIDEOSTREAM, "Type", $Type);
        safeSetAttribute($tagDVDVIDEOSTREAM, "ID", $ID);
        safeSetAttribute($tagDVDVIDEOSTREAM, "VideoCodingMode", $VideoCodingMode);
        safeSetAttribute($tagDVDVIDEOSTREAM, "VideoStandard", $VideoStandard);
        safeSetAttribute($tagDVDVIDEOSTREAM, "VideoAspect", $VideoAspect);
        safeSetAttribute($tagDVDVIDEOSTREAM, "AutomaticPanScanDisallowed", $AutomaticPanScanDisallowed);
        safeSetAttribute($tagDVDVIDEOSTREAM, "CCForLine21Field1InGOP", $CCForLine21Field1InGOP);
        safeSetAttribute($tagDVDVIDEOSTREAM, "CCForLine21Field2InGOP", $CCForLine21Field2InGOP);
        safeSetAttribute($tagDVDVIDEOSTREAM, "CBR", $CBR);
        safeSetAttribute($tagDVDVIDEOSTREAM, "Resolution", $Resolution);
        safeSetAttribute($tagDVDVIDEOSTREAM, "LetterBoxed", $LetterBoxed);
        safeSetAttribute($tagDVDVIDEOSTREAM, "SourceFilm", $SourceFilm);

        $tagOwner->appendChild($tagDVDVIDEOSTREAM);
    }

    $rs->close();
    $rs = null;
}

function buildAudioStream($domtree, $tagOwner, $mysqli, $ProtocolVersion, $idDVDVMGM, $idDVDVTS)
{
    $strSQL = "SELECT idDVDAUDIOSTREAM, Number, Type, ID, CodingMode, MultichannelExtPresent, Quantisation, SampleRate, Channels FROM DVDAUDIOSTREAM ";

    if ($idDVDVMGM != null)
    {
        $strSQL .= "WHERE `DVDVMGMKey` = $idDVDVMGM AND `DVDVTSKey` IS NULL;";
    }
    else
    {
        $strSQL .= "WHERE DVDVMGMKey IS NULL AND DVDVTSKey = $idDVDVTS;";
    }

    $rs = query_server($mysqli, $strSQL);

    while (is_array($Cols = $rs->fetch_row()))
    {
        $Number                         = $Cols[1];
        $Type                           = $Cols[2];
        $ID                             = $Cols[3];
        $CodingMode                     = $Cols[4];
        $MultichannelExtPresent         = $Cols[5];
        $Quantisation                   = $Cols[6];
        $SampleRate                     = $Cols[7];
        $Channels                       = $Cols[8];

        $tagDVDAUDIOSTREAM = $domtree->createElement("DVDAUDIOSTREAM");

        safeSetAttribute($tagDVDAUDIOSTREAM, "Number", $Number);
        safeSetAttribute($tagDVDAUDIOSTREAM, "Type", $Type);
        safeSetAttribute($tagDVDAUDIOSTREAM, "ID", $ID);
        safeSetAttribute($tagDVDAUDIOSTREAM, "CodingMode", $CodingMode);
        safeSetAttribute($tagDVDAUDIOSTREAM, "MultichannelExtPresent", $MultichannelExtPresent);
        safeSetAttribute($tagDVDAUDIOSTREAM, "Quantisation", $Quantisation);
        safeSetAttribute($tagDVDAUDIOSTREAM, "SampleRate", $SampleRate);
        safeSetAttribute($tagDVDAUDIOSTREAM, "Channels", $Channels);

        $tagOwner->appendChild($tagDVDAUDIOSTREAM);
    }

    // free result set
    $rs->close();
    $rs = null;
}

function buildAudioStreamEx($domtree, $tagOwner, $mysqli, $ProtocolVersion, $idDVDVTS)
{
    $rs = query_server($mysqli, "SELECT idDVDAUDIOSTREAMEX, Number, LanguageCodePresent, LanguageCode, CodeExt, CodeExtPresent, MCIntroPresent, ApplicationMode, KaraokeVersion, SuitableForDolbySurroundDecoding FROM DVDAUDIOSTREAMEX WHERE DVDVTSKey = $idDVDVTS");

    while (is_array($Cols = $rs->fetch_row()))
    {
        $Number                             = $Cols[1];
        $LanguageCodePresent                = $Cols[2];
        $LanguageCode                       = $Cols[3];
        $CodeExt                            = $Cols[4];
        $CodeExtPresent                     = $Cols[5];
        $MCIntroPresent                     = $Cols[6];
        $ApplicationMode                    = $Cols[7];
        $KaraokeVersion                     = $Cols[8];
        $SuitableForDolbySurroundDecoding   = $Cols[9];

        $tagDVDAUDIOSTREAMEX = $domtree->createElement("DVDAUDIOSTREAMEX");

        safeSetAttribute($tagDVDAUDIOSTREAMEX, "Number", $Number);
        safeSetAttribute($tagDVDAUDIOSTREAMEX, "Type", "VTS");
        safeSetAttribute($tagDVDAUDIOSTREAMEX, "LanguageCodePresent", $LanguageCodePresent);
        safeSetAttribute($tagDVDAUDIOSTREAMEX, "LanguageCode", $LanguageCode);
        safeSetAttribute($tagDVDAUDIOSTREAMEX, "CodeExtPresent", $CodeExtPresent);
        safeSetAttribute($tagDVDAUDIOSTREAMEX, "CodeExt", $CodeExt);
        safeSetAttribute($tagDVDAUDIOSTREAMEX, "MCIntroPresent", $MCIntroPresent);
        safeSetAttribute($tagDVDAUDIOSTREAMEX, "ApplicationMode", $ApplicationMode);
        safeSetAttribute($tagDVDAUDIOSTREAMEX, "KaraokeVersion", $KaraokeVersion);
        safeSetAttribute($tagDVDAUDIOSTREAMEX, "SuitableForDolbySurroundDecoding", $SuitableForDolbySurroundDecoding);

        $tagOwner->appendChild($tagDVDAUDIOSTREAMEX);
    }

    // free result set
    $rs->close();
    $rs = null;
}

function buildSubpicStream($domtree, $tagOwner, $mysqli, $ProtocolVersion, $idDVDVMGM, $idDVDVTS)
{
    $strSQL = "SELECT idDVDSUBPICSTREAM, Number, Type, ID, CodingMode, LanguageCodePresent, LanguageCode, CodeExtPresent, CodeExt FROM DVDSUBPICSTREAM ";

    if ($idDVDVMGM != null)
    {
        $strSQL .= "WHERE `DVDVMGMKey` = $idDVDVMGM AND `DVDVTSKey` IS NULL;";
    }
    else
    {
        $strSQL .= "WHERE DVDVMGMKey IS NULL AND DVDVTSKey = $idDVDVTS;";
    }

    $rs = query_server($mysqli, $strSQL);

    while (is_array($Cols = $rs->fetch_row()))
    {
        $Number                 = $Cols[1];
        $Type                   = $Cols[2];
        $ID                     = $Cols[3];
        $CodingModeMode         = $Cols[4];
        $LanguageCodePresent    = $Cols[5];
        $LanguageCode           = $Cols[6];
        $CodeExtPresent         = $Cols[7];
        $CodeExt                = $Cols[8];

        $tagDVDSUBPICSTREAM = $domtree->createElement("DVDSUBPICSTREAM");

        safeSetAttribute($tagDVDSUBPICSTREAM, "Number", $Number);
        safeSetAttribute($tagDVDSUBPICSTREAM, "Type", $Type);
        safeSetAttribute($tagDVDSUBPICSTREAM, "ID", $ID);
        safeSetAttribute($tagDVDSUBPICSTREAM, "CodingMode", $CodingModeMode);
        safeSetAttribute($tagDVDSUBPICSTREAM, "LanguageCodePresent", $LanguageCodePresent);
        safeSetAttribute($tagDVDSUBPICSTREAM, "LanguageCode", $LanguageCode);
        safeSetAttribute($tagDVDSUBPICSTREAM, "CodeExtPresent", $CodeExtPresent);
        safeSetAttribute($tagDVDSUBPICSTREAM, "CodeExt", $CodeExt);

        $tagOwner->appendChild($tagDVDSUBPICSTREAM);
    }

    // free result set
    $rs->close();
    $rs = null;
}

function buildFileSet($domtree, $tagOwner, $mysqli, $ProtocolVersion, $idDVDVMGM, $idDVDVTS)
{
    $strSQL = "SELECT idDVDFILE, FileSetNo, Type, VobNo, Size, Date FROM DVDFILE ";

    if ($idDVDVMGM != null)
    {
        $strSQL .= "WHERE `DVDVMGMKey` = $idDVDVMGM AND `DVDVTSKey` IS NULL;";
    }
    else
    {
        $strSQL .= "WHERE DVDVMGMKey IS NULL AND DVDVTSKey = $idDVDVTS;";
    }

    $rs = query_server($mysqli, $strSQL);

    while (is_array($Cols = $rs->fetch_row()))
    {
        $FileSetNo      = $Cols[1];
        $Type           = $Cols[2];
        $VobNo          = $Cols[3];
        $Size           = $Cols[4];
        $Date           = $Cols[5];

        $tagDVDFILESET = $domtree->createElement("DVDFILE");

        safeSetAttribute($tagDVDFILESET, "Number", $FileSetNo);
        safeSetAttribute($tagDVDFILESET,"Type", $Type);
        safeSetAttribute($tagDVDFILESET,"VobNo", $VobNo);
        safeSetAttribute($tagDVDFILESET,"Size", $Size);
        safeSetAttribute($tagDVDFILESET,"Date", $Date);

        $tagOwner->appendChild($tagDVDFILESET);
    }

    // free result set
    $rs->close();
    $rs = null;
}

function buildResults($domtree, $mysqli, $xmlRoot, $Hash, $rsDVDVMGM, $ProtocolVersion)
{
    $found = FALSE;

    // $rsDVDVMGM should be opened with the fields from QUERY_SET
    while (is_array($Cols = $rsDVDVMGM->fetch_row()))
    {
        // Main data
        $found = TRUE;

        $idDVDVMGM          = $Cols[0];
        $Album              = $Cols[1];
        $AlbumArtist        = $Cols[2];
        $Revision           = $Cols[3];
        $RowCreationDate    = $Cols[4];
        $Genre              = $Cols[5];
        $Cast               = $Cols[6];
        $Crew               = $Cols[7];
        $Director           = $Cols[8];
        $Country            = $Cols[9];
        $ReleaseDate        = $Cols[10];
        $SpecialFeatures    = $Cols[11];
        $EAN_UPC            = $Cols[12];
        $Storyline          = $Cols[13];
        $Submitter          = $Cols[14];
        $SubmitterIP        = $Cols[15];
        $Client             = $Cols[16];
        $Remarks            = $Cols[17];
        $RowLastChanged     = $Cols[18];
        $Keywords           = $Cols[19];
        if ($Hash == null)
        {
            $Hash           = $Cols[20];
        }
        $RegionProhibited1  = $Cols[21];
        $RegionProhibited2  = $Cols[22];
        $RegionProhibited3  = $Cols[23];
        $RegionProhibited4  = $Cols[24];
        $RegionProhibited5  = $Cols[25];
        $RegionProhibited6  = $Cols[26];;
        $RegionProhibited7  = $Cols[27];
        $RegionProhibited8  = $Cols[28];
        $VersionNumberMajor = $Cols[29];
        $VersionNumberMinor = $Cols[30];
        $NumberOfVolumes    = $Cols[31];
        $VolumeNumber       = $Cols[32];
        $SideID             = $Cols[33];
        $OriginalAlbum      = $Cols[34];
        $Screenplay         = $Cols[35];
        $Producer           = $Cols[36];
        $Editing            = $Cols[37];
        $Cinematography     = $Cols[38];
        $OriginalLanguage   = $Cols[39];

        $dvdTag = $domtree->createElement("DVD");

        $dvdTag->setAttribute("Hash", $Hash);

        safeSetAttribute($dvdTag, "Revision", $Revision);
        safeSetText($domtree, $dvdTag, "Album", $Album);
        safeSetText($domtree, $dvdTag, "AlbumArtist", $AlbumArtist);
        safeSetText($domtree, $dvdTag, "OriginalAlbum", $OriginalAlbum);
        safeSetText($domtree, $dvdTag, "DateCreated", $RowCreationDate);
        safeSetText($domtree, $dvdTag, "Genre", $Genre);
        safeSetText($domtree, $dvdTag, "Cast", $Cast);
        safeSetText($domtree, $dvdTag, "Crew", $Crew);
        safeSetText($domtree, $dvdTag, "Director", $Director);
        safeSetText($domtree, $dvdTag, "Screenplay", $Screenplay);
        safeSetText($domtree, $dvdTag, "Producer", $Producer);
        safeSetText($domtree, $dvdTag, "Editing", $Editing);
        safeSetText($domtree, $dvdTag, "Cinematography", $Cinematography);
        safeSetText($domtree, $dvdTag, "Country", $Country);
        safeSetText($domtree, $dvdTag, "OriginalLanguage", $OriginalLanguage);
        safeSetText($domtree, $dvdTag, "ReleaseDate", $ReleaseDate);
        safeSetText($domtree, $dvdTag, "DateLastChanged", $RowLastChanged);
        safeSetText($domtree, $dvdTag, "SpecialFeatures", $SpecialFeatures);
        safeSetText($domtree, $dvdTag, "EAN_UPC", $EAN_UPC);
        safeSetText($domtree, $dvdTag, "Storyline", $Storyline);
        safeSetText($domtree, $dvdTag, "Submitter", $Submitter);
        safeSetText($domtree, $dvdTag, "SubmitterIP", $SubmitterIP);
        safeSetText($domtree, $dvdTag, "Client", $Client);
        safeSetText($domtree, $dvdTag, "Remarks", $Remarks);
        safeSetText($domtree, $dvdTag, "Keywords", $Keywords);
        safeSetAttribute($dvdTag, "RegionProhibited1", $RegionProhibited1);
        safeSetAttribute($dvdTag, "RegionProhibited2", $RegionProhibited2);
        safeSetAttribute($dvdTag, "RegionProhibited3", $RegionProhibited3);
        safeSetAttribute($dvdTag, "RegionProhibited4", $RegionProhibited4);
        safeSetAttribute($dvdTag, "RegionProhibited5", $RegionProhibited5);
        safeSetAttribute($dvdTag, "RegionProhibited6", $RegionProhibited6);
        safeSetAttribute($dvdTag, "RegionProhibited7", $RegionProhibited7);
        safeSetAttribute($dvdTag, "RegionProhibited8", $RegionProhibited8);
        safeSetAttribute($dvdTag, "VersionNumberMajor", $VersionNumberMajor);
        safeSetAttribute($dvdTag, "VersionNumberMinor", $VersionNumberMinor);
        safeSetAttribute($dvdTag, "NumberOfVolumes", $NumberOfVolumes);
        safeSetAttribute($dvdTag, "VolumeNumber", $VolumeNumber);
        safeSetAttribute($dvdTag, "SideID", $SideID);

        $xmlRoot->appendChild($dvdTag);

        $tagPhysical = $domtree->createElement("physical");

        // Add title sets
        $rsDVDVTS = query_server($mysqli, "SELECT `idDVDVTS`, `TitleSetNo`, `VersionNumberMajor`, `VersionNumberMinor` FROM `DVDVTS` WHERE `DVDVMGMKey` = $idDVDVMGM");

        while (is_array($Cols = $rsDVDVTS->fetch_row()))
        {
            $idDVDVTS               = $Cols[0];
            $TitleSetNo             = $Cols[1];
            $VersionNumberMajor     = $Cols[2];
            $VersionNumberMinor     = $Cols[3];

            $tagDVDVTS = $domtree->createElement("DVDVTS");
            safeSetAttribute($tagDVDVTS, "TitleSetNo", $TitleSetNo);
            safeSetAttribute($tagDVDVTS, "VersionNumberMajor", $VersionNumberMajor);
            safeSetAttribute($tagDVDVTS, "VersionNumberMinor", $VersionNumberMinor);

            $tagPhysical->appendChild($tagDVDVTS);

            // Add program chains
            $rsDVDPGC = query_server($mysqli, "SELECT `idDVDPGC`, `ProgramChainNo`, `EntryPGC` FROM `DVDPGC` WHERE `DVDVTSKey` = $idDVDVTS");

            while (is_array($Cols = $rsDVDPGC->fetch_row()))
            {
                $idDVDPGC       = $Cols[0];
                $Number         = $Cols[1];
                $EntryPGC       = $Cols[2];

                $tagDVDPGC = $domtree->createElement("DVDPGC");
                safeSetAttribute($tagDVDPGC, "Number", $Number);
                safeSetAttribute($tagDVDPGC, "EntryPGC", $EntryPGC);

                $tagDVDVTS->appendChild($tagDVDPGC);

                // Add programs
                $rsDVDPROGRAM = query_server($mysqli, "SELECT `idDVDPROGRAM`, `ProgramNo` FROM `DVDPROGRAM` WHERE `DVDPGCKey` = $idDVDPGC");

                while (is_array($Cols = $rsDVDPROGRAM->fetch_row()))
                {
                    $idDVDPROGRAM      = $Cols[0];
                    $Number         = $Cols[1];

                    $tagDVDPROGRAM = $domtree->createElement("DVDPROGRAM");
                    safeSetAttribute($tagDVDPROGRAM, "Number", $Number);

                    $tagDVDPGC->appendChild($tagDVDPROGRAM);

                    // Add cells
                    $rsDVDCELL = query_server($mysqli, "SELECT `idDVDCELL`, `CellNo`, `CellType`, `BlockType`, `SeamlessMultiplex`, `Interleaved`, `SCRdiscontinuity`, `SeamlessAngleLinkedInDSI`, `VOBStillMode`, `StopsTrickPlay`, `CellStillTime`, `CellCommand`, `PlayTime`, `FrameRate`, `FirstVOBUStartSector`, `FirstILVUEndSector`, `LastVOBUStartSector`, `LastVOBUEndSector`, `VOBidn`, `CELLidn`, `NumberOfVOBIds` FROM `DVDCELL` WHERE `DVDPROGRAMKey` = $idDVDPROGRAM");

                    while (is_array($Cols = $rsDVDCELL->fetch_row()))
                    {
                        $idDVDCELL                  = $Cols[0];
                        $Number                     = $Cols[1];
                        $CellType                   = $Cols[2];
                        $BlockType                  = $Cols[3];
                        $SeamlessMultiplex          = $Cols[4];
                        $Interleaved                = $Cols[5];
                        $SCRdiscontinuity           = $Cols[6];
                        $SeamlessAngleLinkedInDSI   = $Cols[7];
                        $VOBStillMode               = $Cols[8];
                        $StopsTrickPlay             = $Cols[9];
                        $CellStillTime              = $Cols[10];
                        $CellCommand                = $Cols[11];
                        $PlayTime                   = $Cols[12];
                        $FrameRate                  = $Cols[13];
                        $FirstVOBUStartSector       = $Cols[14];
                        $FirstILVUEndSector         = $Cols[15];
                        $LastVOBUStartSector        = $Cols[16];
                        $LastVOBUEndSector          = $Cols[17];
                        $VOBidn                     = $Cols[18];
                        $CELLidn                    = $Cols[19];
                        $NumberOfVOBIds             = $Cols[20];

                        $tagDVDCELL = $domtree->createElement("DVDCELL");
                        safeSetAttribute($tagDVDCELL, "Number", $Number);
                        safeSetAttribute($tagDVDCELL, "CellType", $CellType);
                        safeSetAttribute($tagDVDCELL, "BlockType", $BlockType);
                        safeSetAttribute($tagDVDCELL, "SeamlessMultiplex", $SeamlessMultiplex);
                        safeSetAttribute($tagDVDCELL, "Interleaved", $Interleaved);
                        safeSetAttribute($tagDVDCELL, "SCRdiscontinuity", $SCRdiscontinuity);
                        safeSetAttribute($tagDVDCELL, "SeamlessAngleLinkedInDSI", $SeamlessAngleLinkedInDSI);
                        safeSetAttribute($tagDVDCELL, "VOBStillMode", $VOBStillMode);
                        safeSetAttribute($tagDVDCELL, "StopsTrickPlay", $StopsTrickPlay);
                        safeSetAttribute($tagDVDCELL, "CellStillTime", $CellStillTime);
                        safeSetAttribute($tagDVDCELL, "CellCommand", $CellCommand);
                        safeSetAttribute($tagDVDCELL, "PlayTime", $PlayTime);
                        safeSetAttribute($tagDVDCELL, "FrameRate", $FrameRate);
                        safeSetAttribute($tagDVDCELL, "FirstVOBUStartSector", $FirstVOBUStartSector);
                        safeSetAttribute($tagDVDCELL, "FirstILVUEndSector", $FirstILVUEndSector);
                        safeSetAttribute($tagDVDCELL, "LastVOBUStartSector", $LastVOBUStartSector);
                        safeSetAttribute($tagDVDCELL, "LastVOBUEndSector", $LastVOBUEndSector);
                        safeSetAttribute($tagDVDCELL, "VOBidn", $VOBidn);
                        safeSetAttribute($tagDVDCELL, "CELLidn", $CELLidn);
                        safeSetAttribute($tagDVDCELL, "NumberOfVOBIds", $NumberOfVOBIds);

                        $tagDVDPROGRAM->appendChild($tagDVDCELL);

                        // Add units
                        $rsDVDUNIT = query_server($mysqli, "SELECT `idDVDUNIT`, `UnitNo`, `StartSector`, `EndSector` FROM `DVDUNIT` WHERE `DVDCELLKey` = $idDVDCELL");

                        while (is_array($Cols = $rsDVDUNIT->fetch_row()))
                        {
                            $idDVDUNIT      = $Cols[0];
                            $Number         = $Cols[1];
                            $StartSector    = $Cols[2];
                            $EndSector      = $Cols[3];

                            $tagDVDUNIT = $domtree->createElement("DVDUNIT");
                            safeSetAttribute($tagDVDUNIT, "Number", $Number);
                            safeSetAttribute($tagDVDUNIT, "StartSector", $StartSector);
                            safeSetAttribute($tagDVDUNIT, "EndSector", $EndSector);

                            $tagDVDCELL->appendChild($tagDVDUNIT);
                        }

                        // free result set
                        $rsDVDUNIT->close();
                        $rsDVDUNIT = null;
                    }

                    // free result set
                    $rsDVDCELL->close();
                    $rsDVDCELL = null;
                }

                // free result set
                $rsDVDPROGRAM->close();
                $rsDVDPROGRAM = null;
            }

            // free result set
            $rsDVDPGC->close();
            $rsDVDPGC = null;

            // Add the streams
            buildVideoStream($domtree, $tagDVDVTS, $mysqli,  $ProtocolVersion, null, $idDVDVTS);
            buildAudioStream($domtree, $tagDVDVTS, $mysqli, $ProtocolVersion, null, $idDVDVTS);
            buildAudioStreamEx($domtree, $tagDVDVTS, $mysqli, $ProtocolVersion, $idDVDVTS);
            buildSubpicStream($domtree, $tagDVDVTS, $mysqli, $ProtocolVersion, null, $idDVDVTS);
            buildFileset($domtree, $tagDVDVTS, $mysqli, $ProtocolVersion, null, $idDVDVTS);
        }

        // free result set
        $rsDVDVTS->close();
        $rsDVDVTS = null;

        $dvdTag->appendChild($tagPhysical);

        $tagVirtual = $domtree->createElement("virtual");

        // Add title sets
        $rsDVDPTTVMG = query_server($mysqli, "SELECT `idDVDPTTVMG`, `TitleSetNo`, `PlaybackType`, `NumberOfVideoAngles`, `VideoTitleSetNo`, `TitleNo`, `Title` FROM `DVDPTTVMG` WHERE `DVDVMGMKey` = $idDVDVMGM");

        while (is_array($Cols = $rsDVDPTTVMG->fetch_row()))
        {
            $idDVDPTTVMG            = $Cols[0];
            $TitleSetNo             = $Cols[1];
            $PlaybackType           = $Cols[2];
            $NumberOfVideoAngles    = $Cols[3];
            $VideoTitleSetNo        = $Cols[4];
            $TitleNo                = $Cols[5];
            $Title                  = $Cols[6];

            $tagDVDPTTVMG = $domtree->createElement("DVDPTTVMG");
            safeSetAttribute($tagDVDPTTVMG, "TitleSetNo", $TitleSetNo);
            safeSetAttribute($tagDVDPTTVMG, "PlaybackType", $PlaybackType);
            safeSetAttribute($tagDVDPTTVMG, "NumberOfVideoAngles", $NumberOfVideoAngles);
            safeSetAttribute($tagDVDPTTVMG, "VideoTitleSetNo", $VideoTitleSetNo);
            safeSetAttribute($tagDVDPTTVMG, "TitleNo", $TitleNo);
            safeSetText($domtree, $tagDVDPTTVMG, "Title", $Title);

            $tagVirtual->appendChild($tagDVDPTTVMG);

            // Add program chains
            $rsDVDPTTVTS = query_server($mysqli, "SELECT `idDVDPTTVTS`, `PttChapterNo`, `ProgramChainNo`, `ProgramNo`, `PttTitleSetNo`, `TitleSetNo`, `Artist`, `Title`  FROM `DVDPTTVTS` WHERE `DVDPTTVMGKey` = $idDVDPTTVMG");

            while (is_array($Cols = $rsDVDPTTVTS->fetch_row()))
            {
                $Number         = $Cols[1];
                $ProgramChainNo = $Cols[2];
                $ProgramNo      = $Cols[3];
                $PttTitleSetNo  = $Cols[4];
                $TitleSetNo     = $Cols[5];
                $Artist         = $Cols[6];
                $Title          = $Cols[7];

                $tagDVDPTTVTS = $domtree->createElement("DVDPTTVTS");
                safeSetText($domtree, $tagDVDPTTVTS, "Artist", $Artist);
                safeSetText($domtree, $tagDVDPTTVTS, "Title", $Title);

                safeSetAttribute($tagDVDPTTVTS, "Number", $Number);
                safeSetAttribute($tagDVDPTTVTS, "TitleSetNo", $TitleSetNo);
                safeSetAttribute($tagDVDPTTVTS, "ProgramChainNo", $ProgramChainNo);
                safeSetAttribute($tagDVDPTTVTS, "ProgramNo", $ProgramNo);
                safeSetAttribute($tagDVDPTTVTS, "PttTitleSetNo", $PttTitleSetNo);

                $tagDVDPTTVMG->appendChild($tagDVDPTTVTS);
            }

            $rsDVDPTTVTS->close();
            $rsDVDPTTVTS = null;
        }

        $rsDVDPTTVMG->close();
        $rsDVDPTTVMG = null;

        $dvdTag->appendChild($tagVirtual);

        // Add the streams
        buildVideoStream($domtree, $dvdTag, $mysqli, $ProtocolVersion, $idDVDVMGM, null, null);
        buildAudioStream($domtree, $dvdTag, $mysqli, $ProtocolVersion, $idDVDVMGM, null, null);
        buildSubpicStream($domtree, $dvdTag, $mysqli, $ProtocolVersion, $idDVDVMGM, null, null);
        buildFileset($domtree, $dvdTag, $mysqli, $ProtocolVersion, $idDVDVMGM, null, null);

        break; // Only one DVD for now
    }
    return $found;
}
?>
