<?php
/*
  dvdetect DVD detection, analysis & DVDETECT lookup library

  Copyright (C) 2013 Norbert Schlia <nschlia@dvdetect.de>

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

/*! \file functions.search.inc.php
 *
 *  \brief PHP function collection
 */

function addVideoStream($mysqli, $tagDVDVIDEOSTREAM, $idDVDVMGM, $idDVDVTS)
{
    $attributes                 = $tagDVDVIDEOSTREAM->attributes();

    $Type                       = value_or_null($attributes["Type"]);
    $ID                         = value_or_null($attributes["ID"]);
    $VideoCodingMode            = value_or_null($attributes["VideoCodingMode"]);
    $VideoStandard              = value_or_null($attributes["VideoStandard"]);
    $VideoAspect                = value_or_null($attributes["VideoAspect"]);
    $AutomaticPanScanDisallowed = value_or_null($attributes["AutomaticPanScanDisallowed"]);
    $CCForLine21Field1InGOP     = value_or_null($attributes["CCForLine21Field1InGOP"]);
    $CCForLine21Field2InGOP     = value_or_null($attributes["CCForLine21Field2InGOP"]);
    $CBR                        = value_or_null($attributes["CBR"]);
    $Resolution                 = value_or_null($attributes["Resolution"]);
    $LetterBoxed                = value_or_null($attributes["LetterBoxed"]);
    $SourceFilm                 = value_or_null($attributes["SourceFilm"]);

    if ($idDVDVTS == null)
    {
        $Type = "VMG";
    }

    $strSQL = "INSERT INTO DVDVIDEOSTREAM (DVDVMGMKey, DVDVTSKey, Type, ID, VideoCodingMode, VideoStandard, VideoAspect, AutomaticPanScanDisallowed, CCForLine21Field1InGOP, CCForLine21Field2InGOP, CBR, Resolution, LetterBoxed, SourceFilm) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    $stmt = $mysqli->prepare($strSQL);

    if (!$stmt)
    {
        $ResponseText = "Error preparing insert statement for DVDVIDEOSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if (!$stmt->bind_param("iisisssiiiisii", $idDVDVMGM, $idDVDVTS, $Type, $ID, $VideoCodingMode, $VideoStandard, $VideoAspect, $AutomaticPanScanDisallowed, $CCForLine21Field1InGOP, $CCForLine21Field2InGOP, $CBR, $Resolution, $LetterBoxed, $SourceFilm))
    {
        $ResponseText = "Error binding parameters for DVDVIDEOSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if (!$stmt->execute())
    {
        $ResponseText = "Error executing statement on DVDVIDEOSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    $stmt->close();
}

function updateVideoStream($mysqli, $tagDVDVIDEOSTREAM, $idDVDVMGM, $idDVDVTS)
{
    $attributes                 = $tagDVDVIDEOSTREAM->attributes();

    $Type                       = value_or_null($attributes["Type"]);
    $ID                         = value_or_null($attributes["ID"]);
    $VideoCodingMode            = value_or_null($attributes["VideoCodingMode"]);
    $VideoStandard              = value_or_null($attributes["VideoStandard"]);
    $VideoAspect                = value_or_null($attributes["VideoAspect"]);
    $AutomaticPanScanDisallowed = value_or_null($attributes["AutomaticPanScanDisallowed"]);
    $CCForLine21Field1InGOP     = value_or_null($attributes["CCForLine21Field1InGOP"]);
    $CCForLine21Field2InGOP     = value_or_null($attributes["CCForLine21Field2InGOP"]);
    $CBR                        = value_or_null($attributes["CBR"]);
    $Resolution                 = value_or_null($attributes["Resolution"]);
    $LetterBoxed                = value_or_null($attributes["LetterBoxed"]);
    $SourceFilm                 = value_or_null($attributes["SourceFilm"]);

    if ($idDVDVMGM == null)
    {
        $Type = "VMG";
    }

    $strSQL = "UPDATE DVDVIDEOSTREAM SET ID = ?, VideoCodingMode = ?, VideoStandard = ?, VideoAspect = ?, AutomaticPanScanDisallowed = ?, CCForLine21Field1InGOP = ?, CCForLine21Field2InGOP = ?, CBR = ?, Resolution = ?, LetterBoxed = ?, SourceFilm = ? ";

    if ($idDVDVMGM != null)
    {
        $strSQL .= "WHERE DVDVMGMKey = ? AND DVDVTSKey IS NULL AND Type = ?;";
    }
    else
    {
        $strSQL .= "WHERE DVDVMGMKey IS NULL AND DVDVTSKey = ? AND Type = ?;";
    }

    $stmt = $mysqli->prepare($strSQL);

    if (!$stmt)
    {
        $ResponseText = "Error preparing update statement for DVDVIDEOSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if ($idDVDVMGM != null)
    {
        if (!$stmt->bind_param("isssiiiisiiis", $ID, $VideoCodingMode, $VideoStandard, $VideoAspect, $AutomaticPanScanDisallowed, $CCForLine21Field1InGOP, $CCForLine21Field2InGOP, $CBR, $Resolution, $LetterBoxed, $SourceFilm, $idDVDVMGM, $Type))
        {
            $ResponseText = "Error binding parameters for DVDVIDEOSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
            $stmt->close();
            throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
        }
    }
    else
    {
        if (!$stmt->bind_param("isssiiiisiiis", $ID, $VideoCodingMode, $VideoStandard, $VideoAspect, $AutomaticPanScanDisallowed, $CCForLine21Field1InGOP, $CCForLine21Field2InGOP, $CBR, $Resolution, $LetterBoxed, $SourceFilm, $idDVDVTS, $Type))
        {
            $ResponseText = "Error binding parameters for DVDVIDEOSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
            $stmt->close();
            throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
        }
    }

    if (!$stmt->execute())
    {
        $ResponseText = "Error executing statement on DVDVIDEOSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    $stmt->close();
}

function addAudioStream($mysqli, $tagDVDAUDIOSTREAM, $idDVDVMGM, $idDVDVTS)
{
    $attributes             = $tagDVDAUDIOSTREAM->attributes();

    $Number                 = value_or_null($attributes["Number"]);
    $Type                   = value_or_null($attributes["Type"]);
    $ID                     = value_or_null($attributes["ID"]);
    $Channels               = value_or_null($attributes["Channels"]);
    $SampleRate             = value_or_null($attributes["SampleRate"]);
    $Quantisation           = value_or_null($attributes["Quantisation"]);
    $MultichannelExtPresent = value_or_null($attributes["MultichannelExtPresent"]);
    $CodingMode             = value_or_null($attributes["CodingMode"]);

    if ($idDVDVTS == null)
    {
        $Type = "VMG";
    }

    $strSQL = "INSERT INTO DVDAUDIOSTREAM (DVDVMGMKey, DVDVTSKey, Number, Type, ID, SampleRate, Channels, Quantisation, CodingMode) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

    $stmt = $mysqli->prepare($strSQL);

    if (!$stmt)
    {
        $ResponseText = "Error preparing insert statement for DVDAUDIOSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if (!$stmt->bind_param("iiisiiiss", $idDVDVMGM, $idDVDVTS, $Number, $Type, $ID, $SampleRate, $Channels, $Quantisation, $CodingMode))
    {
        $ResponseText = "Error binding parameters for DVDAUDIOSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if (!$stmt->execute())
    {
        $ResponseText = "Error executing statement on DVDAUDIOSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    $stmt->close();
}

function updateAudioStream($mysqli, $tagDVDAUDIOSTREAM, $idDVDVMGM, $idDVDVTS)
{
    $attributes             = $tagDVDAUDIOSTREAM->attributes();

    $Number                 = value_or_null($attributes["Number"]);
    $Type                   = value_or_null($attributes["Type"]);
    $ID                     = value_or_null($attributes["ID"]);
    $Channels               = value_or_null($attributes["Channels"]);
    $SampleRate             = value_or_null($attributes["SampleRate"]);
    $Quantisation           = value_or_null($attributes["Quantisation"]);
    $MultichannelExtPresent = value_or_null($attributes["MultichannelExtPresent"]);
    $CodingMode             = value_or_null($attributes["CodingMode"]);

    if ($idDVDVTS == null)
    {
        $Type = "VMG";
    }

    $strSQL = "UPDATE DVDAUDIOSTREAM SET ID = ?, SampleRate = ?, Channels = ?, Quantisation = ?, CodingMode = ? ";

    if ($idDVDVMGM != null)
    {
        $strSQL .= "WHERE DVDVMGMKey = ? AND DVDVTSKey IS NULL AND Number = ? AND Type = ?;";
    }
    else
    {
        $strSQL .= "WHERE DVDVMGMKey IS NULL AND DVDVTSKey = ? AND Number = ? AND Type = ?;";
    }

    $stmt = $mysqli->prepare($strSQL);

    if (!$stmt)
    {
        $ResponseText = "Error preparing update statement for DVDAUDIOSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if ($idDVDVMGM != null)
    {
        if (!$stmt->bind_param("iiissiis", $ID, $SampleRate, $Channels, $Quantisation, $CodingMode, $idDVDVMGM, $Number, $Type))
        {
            $ResponseText = "Error binding parameters for DVDAUDIOSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
            $stmt->close();
            throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
        }
    }
    else
    {
        if (!$stmt->bind_param("iiissiis", $ID, $SampleRate, $Channels, $Quantisation, $CodingMode, $idDVDVTS, $Number, $Type))
        {
            $ResponseText = "Error binding parameters for DVDAUDIOSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
            $stmt->close();
            throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
        }
    }

    if (!$stmt->execute())
    {
        $ResponseText = "Error executing statement on DVDAUDIOSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    $stmt->close();
}

function addAudioStreamEx($mysqli, $audiostreamExTag, $idDVDVTS)
{
    $attributes                         = $audiostreamExTag->attributes();

    $Number                             = value_or_null($attributes["Number"]);
    $Type                               = value_or_null($attributes["Type"]);
    $SuitableForDolbySurroundDecoding   = value_or_null($attributes["SuitableForDolbySurroundDecoding"]);
    $KaraokeVersion                     = value_or_null($attributes["KaraokeVersion"]);
    $ApplicationMode                    = value_or_null($attributes["ApplicationMode"]);
    $MCIntroPresent                     = value_or_null($attributes["MCIntroPresent"]);
    $LanguageCodePresent                = value_or_null($attributes["LanguageCodePresent"]);
    $LanguageCode                       = value_or_null($attributes["LanguageCode"]);
    $CodeExtPresent                     = value_or_null($attributes["CodeExtPresent"]);
    $CodeExt                            = value_or_null($attributes["CodeExt"]);

    if ($idDVDVTS == null)
    {
        $Type = "VMG";
    }

    $strSQL = "INSERT INTO DVDAUDIOSTREAMEX (DVDVTSKey, Number, SuitableForDolbySurroundDecoding, KaraokeVersion, ApplicationMode, MCIntroPresent, LanguageCodePresent, LanguageCode, CodeExtPresent, CodeExt) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    $stmt = $mysqli->prepare($strSQL);

    if (!$stmt)
    {
        $ResponseText = "Error preparing insert statement for DVDAUDIOSTREAMEX table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if (!$stmt->bind_param("iiiisiisii", $idDVDVTS, $Number, $SuitableForDolbySurroundDecoding, $KaraokeVersion, $ApplicationMode, $MCIntroPresent, $LanguageCodePresent, $LanguageCode, $CodeExtPresent, $CodeExt))
    {
        $ResponseText = "Error binding parameters for DVDAUDIOSTREAMEX table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if (!$stmt->execute())
    {
        $ResponseText = "Error executing statement on DVDAUDIOSTREAMEX table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    $stmt->close();
}

function updateAudioStreamEx($mysqli, $audiostreamExTag, $idDVDVTS)
{
    $attributes                         = $audiostreamExTag->attributes();

    $Number                             = value_or_null($attributes["Number"]);
    $SuitableForDolbySurroundDecoding   = value_or_null($attributes["SuitableForDolbySurroundDecoding"]);
    $KaraokeVersion                     = value_or_null($attributes["KaraokeVersion"]);
    $ApplicationMode                    = value_or_null($attributes["ApplicationMode"]);
    $MCIntroPresent                     = value_or_null($attributes["MCIntroPresent"]);
    $LanguageCodePresent                = value_or_null($attributes["LanguageCodePresent"]);
    $LanguageCode                       = value_or_null($attributes["LanguageCode"]);
    $CodeExtPresent                     = value_or_null($attributes["CodeExtPresent"]);
    $CodeExt                            = value_or_null($attributes["CodeExt"]);

    $strSQL = "UPDATE DVDAUDIOSTREAMEX SET SuitableForDolbySurroundDecoding = ?, KaraokeVersion = ?, ApplicationMode = ?, MCIntroPresent = ?, LanguageCodePresent = ?, LanguageCode = ?, CodeExtPresent = ?, CodeExt = ? WHERE DVDVTSKey = ? AND Number = ?;";

    $stmt = $mysqli->prepare($strSQL);

    if (!$stmt)
    {
        $ResponseText = "Error preparing update statement for DVDAUDIOSTREAMEX table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if (!$stmt->bind_param("isiisiiiii", $SuitableForDolbySurroundDecoding, $KaraokeVersion, $ApplicationMode, $MCIntroPresent, $LanguageCodePresent, $LanguageCode, $CodeExtPresent, $CodeExt, $idDVDVTS, $Number))
    {
        $ResponseText = "Error binding parameters for DVDAUDIOSTREAMEX table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if (!$stmt->execute())
    {
        $ResponseText = "Error executing statement on DVDAUDIOSTREAMEX table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    $stmt->close();
}

function addSubPictureStream($mysqli, $tagDVDSUBPICSTREAM, $idDVDVMGM, $idDVDVTS)
{
    $attributes             = $tagDVDSUBPICSTREAM->attributes();

    $Number                 = value_or_null($attributes["Number"]);
    $ID                     = value_or_null($attributes["ID"]);
    $Type                   = value_or_null($attributes["Type"]);
    $CodingMode             = value_or_null($attributes["CodingMode"]);
    $LanguageCodePresent    = value_or_null($attributes["LanguageCodePresent"]);
    $LanguageCode           = value_or_null($attributes["LanguageCode"]);
    $CodeExtPresent         = value_or_null($attributes["CodeExtPresent"]);
    $CodeExt                = value_or_null($attributes["CodeExt"]);

    if ($idDVDVTS == null)
    {
        $Type = "VMG";
    }

    $strSQL = "INSERT INTO DVDSUBPICSTREAM (DVDVMGMKey, DVDVTSKey, Number, ID, Type, CodingMode, LanguageCodePresent, LanguageCode, CodeExtPresent, CodeExt) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    $stmt = $mysqli->prepare($strSQL);

    if (!$stmt)
    {
        $ResponseText = "Error preparing insert statement for DVDSUBPICSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if (!$stmt->bind_param("iiissiisii", $idDVDVMGM, $idDVDVTS, $Number, $ID, $Type, $CodingMode, $LanguageCodePresent, $LanguageCode, $CodeExtPresent, $CodeExt))
    {
        $ResponseText = "Error binding parameters for DVDSUBPICSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if (!$stmt->execute())
    {
        $ResponseText = "Error executing statement on DVDSUBPICSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    $stmt->close();
}

function updateSubPictureStream($mysqli, $tagDVDSUBPICSTREAM, $idDVDVMGM, $idDVDVTS)
{
    $attributes             = $tagDVDSUBPICSTREAM->attributes();

    $Number                 = value_or_null($attributes["Number"]);
    $ID                     = value_or_null($attributes["ID"]);
    $Type                   = value_or_null($attributes["Type"]);
    $CodingMode             = value_or_null($attributes["CodingMode"]);
    $LanguageCodePresent    = value_or_null($attributes["LanguageCodePresent"]);
    $LanguageCode           = value_or_null($attributes["LanguageCode"]);
    $CodeExtPresent         = value_or_null($attributes["CodeExtPresent"]);
    $CodeExt                = value_or_null($attributes["CodeExt"]);

    if ($idDVDVTS == null)
    {
        $Type = "VMG";
    }

    $strSQL = "UPDATE DVDSUBPICSTREAM SET ID = ?, CodingMode = ?, LanguageCodePresent = ?, LanguageCode = ?, CodeExtPresent = ?, CodeExt = ? ";

    if ($idDVDVMGM != null)
    {
        $strSQL .= "WHERE DVDVMGMKey = ? AND DVDVTSKey IS NULL AND Number = ? AND Type = ?;";
    }
    else
    {
        $strSQL .= "WHERE DVDVMGMKey IS NULL AND DVDVTSKey = ? AND Number = ? AND Type = ?;";
    }

    $stmt = $mysqli->prepare($strSQL);

    if (!$stmt)
    {
        $ResponseText = "Error preparing update statement for DVDSUBPICSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if ($idDVDVMGM != null)
    {
        if (!$stmt->bind_param("siisiiiis", $ID, $CodingMode, $LanguageCodePresent, $LanguageCode, $CodeExtPresent, $CodeExt, $idDVDVMGM, $Number, $Type))
        {
            $ResponseText = "Error binding parameters for DVDSUBPICSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
            $stmt->close();
            throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
        }
    }
    else
    {
        if (!$stmt->bind_param("siisiiiis", $ID, $CodingMode, $LanguageCodePresent, $LanguageCode, $CodeExtPresent, $CodeExt, $idDVDVTS, $Number, $Type))
        {
            $ResponseText = "Error binding parameters for DVDSUBPICSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
            $stmt->close();
            throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
        }
    }

    if (!$stmt->execute())
    {
        $ResponseText = "Error executing statement on DVDSUBPICSTREAM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    $stmt->close();
}

function addFileset($mysqli, $tagDVDFILE, $idDVDVMGM, $idDVDVTS)
{
    $attributes     = $tagDVDFILE->attributes();

    $FileSetNo      = value_or_null($attributes["Number"]);
    $Type           = value_or_null($attributes["Type"]);
    $VobNo          = value_or_null($attributes["VobNo"]);
    $Size           = value_or_null($attributes["Size"]);
    $Date           = value_or_null($attributes["Date"]);

    $strSQL = "INSERT INTO DVDFILE (DVDVMGMKey, DVDVTSKey, FileSetNo, Type, VobNo, Size, Date) VALUES (?, ?, ?, ?, ?, ?, ?);";

    $stmt = $mysqli->prepare($strSQL);

    if (!$stmt)
    {
        $ResponseText = "Error preparing insert statement for DVDFILE table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if (!$stmt->bind_param("iiisiis", $idDVDVMGM, $idDVDVTS, $FileSetNo, $Type, $VobNo, $Size, $Date))
    {
        $ResponseText = "Error binding parameters for DVDFILE table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if (!$stmt->execute())
    {
        $ResponseText = "Error executing statement on DVDFILE table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    $stmt->close();
}

function updateFileset($mysqli, $tagDVDFILE, $idDVDVMGM, $idDVDVTS)
{
    $attributes     = $tagDVDFILE->attributes();

    $FileSetNo      = value_or_null($attributes["Number"]);
    $Type           = value_or_null($attributes["Type"]);
    $VobNo          = value_or_null($attributes["VobNo"]);
    $Size           = value_or_null($attributes["Size"]);
    $Date           = value_or_null($attributes["Date"]);

    $strSQL = "UPDATE `DVDFILE` SET `Type` = ?, `VobNo` = ?, `Size` = ?, `Date` = ? ";

    if ($idDVDVMGM != null)
    {
        $strSQL .= "WHERE DVDVMGMKey = ? AND DVDVTSKey IS NULL AND FileSetNo = ?;";
    }
    else
    {
        $strSQL .= "WHERE DVDVMGMKey IS NULL AND DVDVTSKey = ? AND FileSetNo = ?;";
    }


    $stmt = $mysqli->prepare($strSQL);

    if (!$stmt)
    {
        $ResponseText = "Error preparing insert statement for DVDFILE table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    if ($idDVDVMGM != null)
    {
        if (!$stmt->bind_param("siisii", $Type, $VobNo, $Size, $Date, $idDVDVMGM, $FileSetNo))
        {
            $ResponseText = "Error binding parameters for DVDFILE table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
            $stmt->close();
            throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
        }
    }
    else
    {
        if (!$stmt->bind_param("siisii", $Type, $VobNo, $Size, $Date, $idDVDVTS, $FileSetNo))
        {
            $ResponseText = "Error binding parameters for DVDFILE table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
            $stmt->close();
            throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
        }
    }

    if (!$stmt->execute())
    {
        $ResponseText = "Error executing statement on DVDFILE table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        $stmt->close();
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    $stmt->close();
}

function submit($mysqli, $xml, $XmlVersion) 
{
/*
    query_server($mysqli, "TRUNCATE TABLE `DVDAUDIOSTREAM`;");
    query_server($mysqli, "TRUNCATE TABLE `DVDAUDIOSTREAMEX`;");
    query_server($mysqli, "TRUNCATE TABLE `DVDSUBPICSTREAM`;");
    query_server($mysqli, "TRUNCATE TABLE `DVDVIDEOSTREAM`;");
    query_server($mysqli, "TRUNCATE TABLE `DVDFILE`;");
    query_server($mysqli, "TRUNCATE TABLE `DVDPTTVMG`;");
    query_server($mysqli, "TRUNCATE TABLE `DVDPTTVTS`;");
    query_server($mysqli, "TRUNCATE TABLE `DVDUNIT`;");
    query_server($mysqli, "TRUNCATE TABLE `DVDCELL`;");
    query_server($mysqli, "TRUNCATE TABLE `DVDPROGRAM`;");
    query_server($mysqli, "TRUNCATE TABLE `DVDPGC`;");
    query_server($mysqli, "TRUNCATE TABLE `DVDVTS`;");
    query_server($mysqli, "TRUNCATE TABLE `DVDVMGM`;");
*/
    // Start a transaction
    query_server($mysqli, "START TRANSACTION;");

    foreach ($xml->DVD as $tagDVDVMGM)
    {
        $attributes         = $tagDVDVMGM->attributes();

        $Hash               = value_or_null($attributes["Hash"]);
        $SubmitterIP        = value_or_null($_SERVER['REMOTE_ADDR']);

        $Album              = value_or_null($tagDVDVMGM->Album);
        $OriginalAlbum      = value_or_null($tagDVDVMGM->OriginalAlbum);
        $AlbumArtist        = value_or_null($tagDVDVMGM->AlbumArtist);
        $Genre              = value_or_null($tagDVDVMGM->Genre);
        $Cast               = value_or_null($tagDVDVMGM->Cast);
        $Crew               = value_or_null($tagDVDVMGM->Crew);
        $Director           = value_or_null($tagDVDVMGM->Director);
        $Screenplay         = value_or_null($tagDVDVMGM->Screenplay);
        $Producer           = value_or_null($tagDVDVMGM->Producer);
        $Editing            = value_or_null($tagDVDVMGM->Editing);
        $Cinematography     = value_or_null($tagDVDVMGM->Cinematography);
        $Country            = value_or_null($tagDVDVMGM->Country);
        $OriginalLanguage   = value_or_null($tagDVDVMGM->OriginalLanguage);
        $ReleaseDate        = date_or_null($tagDVDVMGM->ReleaseDate);
        $SpecialFeatures    = value_or_null($tagDVDVMGM->SpecialFeatures);
        $EAN_UPC            = value_or_null($tagDVDVMGM->EAN_UPC);
        $Storyline          = value_or_null($tagDVDVMGM->Storyline);
        $Submitter          = value_or_null($tagDVDVMGM->Submitter);
        $Client             = value_or_null($tagDVDVMGM->Client);
        $Remarks            = value_or_null($tagDVDVMGM->Remarks);
        $Keywords           = value_or_null($tagDVDVMGM->Keywords);
        $RegionProhibited1  = value_or_null($attributes["RegionProhibited1"]);
        $RegionProhibited2  = value_or_null($attributes["RegionProhibited2"]);
        $RegionProhibited3  = value_or_null($attributes["RegionProhibited3"]);
        $RegionProhibited4  = value_or_null($attributes["RegionProhibited4"]);
        $RegionProhibited5  = value_or_null($attributes["RegionProhibited5"]);
        $RegionProhibited6  = value_or_null($attributes["RegionProhibited6"]);
        $RegionProhibited7  = value_or_null($attributes["RegionProhibited7"]);
        $RegionProhibited8  = value_or_null($attributes["RegionProhibited8"]);
        $VersionNumberMajor = value_or_null($attributes["VersionNumberMajor"]);
        $VersionNumberMinor = value_or_null($attributes["VersionNumberMinor"]);
        $NumberOfVolumes    = value_or_null($attributes["NumberOfVolumes"]);
        $VolumeNumber       = value_or_null($attributes["VolumeNumber"]);
        $SideID             = value_or_null($attributes["SideID"]);

        if ($Submitter == DEFSUBMITTER)
        {
            $Submitter = null;
        }

        // Check if dataset exists
        $found = FALSE;

/* Feature #884: deactivated unstable feature for rev 0.40

        $strSQL = "SELECT `idDVDVMGM`, `RowLastChanged`, `RowCreationDate`, `Submitter`, `SubmitterIP` FROM `DVDVMGM` ".
                    "WHERE `Hash` = '" . $Hash . "' AND `Active` = 1 " .
                    "ORDER BY `Revision` DESC;";

        $rsDVDVMGM = query_server($mysqli, $strSQL);

        if (is_array($Cols = $rsDVDVMGM->fetch_row()))
        {
            $idDVDVMGM          = $Cols[0];
            $RowLastChanged     = $Cols[1];
            $RowCreationDate    = $Cols[2];
            $LastSubmitter      = $Cols[3];
            $LastSubmitterIP    = $Cols[4];

            // TODO: maybe check submission time
            if ($Submitter == $LastSubmitter && $SubmitterIP == $LastSubmitterIP)
            {
                $found = TRUE;
            }
        }

        $rsDVDVMGM->close();
*/

        if (!$found)
        {
            // Not found: insert new
            $strSQL = "INSERT INTO `DVDVMGM` (`Hash`, `Album`, `AlbumArtist`, `Genre`, `Cast`, `Crew`, `Director`, `Country`, `ReleaseDate`, `SpecialFeatures`, `EAN_UPC`, `Storyline`, `Remarks`, `Submitter`, `SubmitterIP`, `Client`, `Keywords`, `RegionProhibited1`, `RegionProhibited2`, `RegionProhibited3`, `RegionProhibited4`, `RegionProhibited5`, `RegionProhibited6`, `RegionProhibited7`, `RegionProhibited8`, `VersionNumberMajor`, `VersionNumberMinor`, `NumberOfVolumes`, `VolumeNumber`, `SideID`, `OriginalAlbum`, `Screenplay`, `Producer`, `Editing`, `Cinematography`, `OriginalLanguage`) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

            $stmt = $mysqli->prepare($strSQL);

            if (!$stmt)
            {
                $ResponseText = "Error preparing insert statement for DVDVMGM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
            }

            if (!$stmt->bind_param("sssssssssssssssssiiiiiiiiiiiiissssss", $Hash, $Album, $AlbumArtist, $Genre, $Cast, $Crew, $Director, $Country, $ReleaseDate, $SpecialFeatures, $EAN_UPC, $Storyline, $Remarks, $Submitter, $SubmitterIP, $Client, $Keywords, $RegionProhibited1, $RegionProhibited2, $RegionProhibited3, $RegionProhibited4, $RegionProhibited5, $RegionProhibited6, $RegionProhibited7, $RegionProhibited8, $VersionNumberMajor, $VersionNumberMinor, $NumberOfVolumes, $VolumeNumber, $SideID, $OriginalAlbum, $Screenplay, $Producer, $Editing, $Cinematography, $OriginalLanguage))
            {
                $ResponseText = "Error binding parameters for DVDVMGM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                $stmt->close();
                $stmt = null;
                throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
            }

            if (!$stmt->execute())
            {
                $ResponseText = "Error executing statement on DVDVMGM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                $stmt->close();
                $stmt = null;
                throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
            }

            $idDVDVMGM = $mysqli->insert_id;

            $stmt->close();
            $stmt = null;

            // Physical structure

            $tagPhysical = $tagDVDVMGM->physical;

            foreach ($tagPhysical->DVDVTS as $tagDVDVTS)
            {
                // Insert DVDVTS (Video Title Set)
                $attributes         = $tagDVDVTS->attributes();

                $TitleSetNo         = $attributes["TitleSetNo"];
                $VersionNumberMajor = value_or_null($attributes["VersionNumberMajor"]);
                $VersionNumberMinor = value_or_null($attributes["VersionNumberMinor"]);

                $strSQL = "INSERT INTO `DVDVTS` (`DVDVMGMKey`, `TitleSetNo`, `VersionNumberMajor`, `VersionNumberMinor`) VALUES (?, ?, ?, ?);";

                $stmt = $mysqli->prepare($strSQL);

                if (!$stmt)
                {
                    $ResponseText = "Error preparing insert statement for DVDVTS table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                }

                if (!$stmt->bind_param("iiii", $idDVDVMGM, $TitleSetNo, $VersionNumberMajor, $VersionNumberMinor))
                {
                    $ResponseText = "Error binding parameters for DVDVTS table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                    $stmt->close();
                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);

                }

                if (!$stmt->execute())
                {
                    $ResponseText = "Error executing statement on DVDVTS table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                    $stmt->close();
                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                }

                $idDVDVTS = $mysqli->insert_id;

                $stmt->close();

                foreach ($tagDVDVTS->DVDPGC as $tagDVDPGC)
                {
                    // Insert DVDPGC (Program Chain)
                    $attributes     = $tagDVDPGC->attributes();

                    $ProgramChainNo = value_or_null($attributes["Number"]);
                    $EntryPGC       = value_or_null($attributes["EntryPGC"]);

                    $strSQL = "INSERT INTO `DVDPGC` (`DVDVTSKey`, `ProgramChainNo`, `EntryPGC`) VALUES (?, ?, ?);";

                    $stmt = $mysqli->prepare($strSQL);

                    if (!$stmt)
                    {
                        $ResponseText = "Error preparing insert statement for DVDPGC table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                    }

                    if (!$stmt->bind_param("iii", $idDVDVTS, $ProgramChainNo, $EntryPGC))
                    {
                        $ResponseText = "Error binding parameters for DVDPGC table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                        $stmt->close();
                        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                    }

                    if (!$stmt->execute())
                    {
                        $ResponseText = "Error executing statement on DVDPGC table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                        $stmt->close();
                        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                    }

                    $idDVDPGC = $mysqli->insert_id;

                    $stmt->close();

                    foreach ($tagDVDPGC->DVDPROGRAM as $tagDVDPROGRAM)
                    {
                        // Insert DVDPGC (Program)
                        $attributes     = $tagDVDPROGRAM->attributes();

                        $ProgramNo      = value_or_null($attributes["Number"]);

                        $strSQL = "INSERT INTO `DVDPROGRAM` (`DVDPGCKey`, `ProgramNo`) VALUES (?, ?);";

                        $stmt = $mysqli->prepare($strSQL);

                        if (!$stmt)
                        {
                            $ResponseText = "Error preparing insert statement for chapter table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                            throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                        }

                        if (!$stmt->bind_param("ii", $idDVDPGC, $ProgramNo))
                        {
                            $ResponseText = "Error binding parameters for chapter table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                            $stmt->close();
                            throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                        }

                        if (!$stmt->execute())
                        {
                            $ResponseText = "Error executing statement on chapter table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                            $stmt->close();
                            throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                        }

                        $idDVDPROGRAM = $mysqli->insert_id;

                        $stmt->close();

                        foreach ($tagDVDPROGRAM->DVDCELL as $tagDVDCELL)
                        {
                            // Insert DVDCELL (Cell)
                            $attributes                 = $tagDVDCELL->attributes();

                            $CellNo                     = value_or_null($attributes["Number"]);
                            $CellType                   = value_or_null($attributes["CellType"]);
                            $BlockType                  = value_or_null($attributes["BlockType"]);
                            $SeamlessMultiplex          = value_or_null($attributes["SeamlessMultiplex"]);
                            $Interleaved                = value_or_null($attributes["Interleaved"]);
                            $SCRdiscontinuity           = value_or_null($attributes["SCRdiscontinuity"]);
                            $SeamlessAngleLinkedInDSI   = value_or_null($attributes["SeamlessAngleLinkedInDSI"]);
                            $VOBStillMode               = value_or_null($attributes["VOBStillMode"]);
                            $StopsTrickPlay             = value_or_null($attributes["StopsTrickPlay"]);
                            $CellStillTime              = value_or_null($attributes["CellStillTime"]);
                            $CellCommand                = value_or_null($attributes["CellCommand"]);
                            $PlayTime                   = value_or_null($attributes["PlayTime"]);
                            $FrameRate                  = value_or_null($attributes["FrameRate"]);
                            $FirstVOBUStartSector       = value_or_null($attributes["FirstVOBUStartSector"]);
                            $FirstILVUEndSector         = value_or_null($attributes["FirstILVUEndSector"]);
                            $LastVOBUStartSector        = value_or_null($attributes["LastVOBUStartSector"]);
                            $LastVOBUEndSector          = value_or_null($attributes["LastVOBUEndSector"]);
                            $VOBidn                     = value_or_null($attributes["VOBidn"]);
                            $CELLidn                    = value_or_null($attributes["CELLidn"]);
                            $NumberOfVOBIds             = value_or_null($attributes["NumberOfVOBIds"]);

                            $strSQL = "INSERT INTO `DVDCELL` (`DVDPROGRAMKey`, `CellNo`, `CellType`, `BlockType`, `SeamlessMultiplex`, `Interleaved`, `SCRdiscontinuity`, `SeamlessAngleLinkedInDSI`, `VOBStillMode`, `StopsTrickPlay`, `CellStillTime`, `CellCommand`, `PlayTime`, `FrameRate`, `FirstVOBUStartSector`, `FirstILVUEndSector`, `LastVOBUStartSector`, `LastVOBUEndSector`, `VOBidn`, `CELLidn`, `NumberOfVOBIds`) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

                            $stmt = $mysqli->prepare($strSQL);

                            if (!$stmt)
                            {
                                $ResponseText = "Error preparing insert statement for DVDCELL table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                                throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                            }

                            if (!$stmt->bind_param("iissiiiiiiiiiiiiiiiii", $idDVDPROGRAM, $CellNo, $CellType, $BlockType, $SeamlessMultiplex, $Interleaved, $SCRdiscontinuity, $SeamlessAngleLinkedInDSI, $VOBStillMode, $StopsTrickPlay, $CellStillTime, $CellCommand, $PlayTime, $FrameRate, $FirstVOBUStartSector, $FirstILVUEndSector, $LastVOBUStartSector, $LastVOBUEndSector, $VOBidn, $CELLidn, $NumberOfVOBIds))
                            {
                                $ResponseText = "Error binding parameters for DVDCELL table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                                $stmt->close();
                                throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                            }

                            if (!$stmt->execute())
                            {
                                $ResponseText = "Error executing statement on DVDCELL table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                                $stmt->close();
                                throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                            }

                            $idDVDCELL = $mysqli->insert_id;

                            $stmt->close();

                            foreach ($tagDVDCELL->DVDUNIT as $tagDVDUNIT)
                            {
                                // Insert DVDUNIT (Unit)
                                $attributes                 = $tagDVDUNIT->attributes();

                                $UnitNo                     = value_or_null($attributes["Number"]);
                                $StartSector                = value_or_null($attributes["StartSector"]);
                                $EndSector                  = value_or_null($attributes["EndSector"]);

                                $strSQL = "INSERT INTO `DVDUNIT` (`DVDCELLKey`, `UnitNo`, `StartSector`, `EndSector`) VALUES (?, ?, ?, ?);";

                                $stmt = $mysqli->prepare($strSQL);

                                if (!$stmt)
                                {
                                    $ResponseText = "Error preparing insert statement for DVDUNIT table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                                }

                                if (!$stmt->bind_param("iiii", $idDVDCELL, $UnitNo, $StartSector, $EndSector))
                                {
                                    $ResponseText = "Error binding parameters for DVDUNIT table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                                    $stmt->close();
                                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                                }

                                if (!$stmt->execute())
                                {
                                    $ResponseText = "Error executing statement on DVDUNIT table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                                    $stmt->close();
                                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                                }

                                //$idDVDUNIT = $mysqli->insert_id;

                                $stmt->close();
                            }
                        }
                    }
                }

                // DVDVIDEOSTREAM
                foreach ($tagDVDVTS->DVDVIDEOSTREAM as $tagDVDVIDEOSTREAM)
                {
                    addVideoStream($mysqli, $tagDVDVIDEOSTREAM, null, $idDVDVTS);
                }

                // DVDAUDIOSTREAM
                foreach ($tagDVDVTS->DVDAUDIOSTREAM as $tagDVDAUDIOSTREAM)
                {
                    addAudioStream($mysqli, $tagDVDAUDIOSTREAM, null, $idDVDVTS);
                }

                // DVDAUDIOSTREAMEX
                foreach ($tagDVDVTS->DVDAUDIOSTREAMEX as $audiostreamExTag)
                {
                    addAudioStreamEx($mysqli, $audiostreamExTag, $idDVDVTS);
                }

                // DVDSUBPICSTREAM
                foreach ($tagDVDVTS->DVDSUBPICSTREAM as $tagDVDSUBPICSTREAM)
                {
                    addSubpictureStream($mysqli, $tagDVDSUBPICSTREAM, null, $idDVDVTS);
                }

                // Fileset
                foreach ($tagDVDVTS->DVDFILE as $tagDVDFILE)
                {
                    addFileset($mysqli, $tagDVDFILE, null, $idDVDVTS);
                }
            }

            // Virtual structure

            $tagVirtual = $tagDVDVMGM->virtual;

            foreach ($tagVirtual->DVDPTTVMG as $tagDVDPTTVMG)
            {
                // Insert DVDPTTVMG (Video Title Set)
                $attributes             = $tagDVDPTTVMG->attributes();

                $Title                  = value_or_null($tagDVDPTTVMG->Title);

                $TitleSetNo             = value_or_null($attributes["TitleSetNo"]);
                $PlaybackType           = value_or_null($attributes["PlaybackType"]);
                $NumberOfVideoAngles    = value_or_null($attributes["NumberOfVideoAngles"]);
                $ParentalMgmMaskVMG     = value_or_null($attributes["ParentalMgmMaskVMG"]);
                $ParentalMgmMaskVTS     = value_or_null($attributes["ParentalMgmMaskVTS"]);
                $NumberOfVideoAngles    = value_or_null($attributes["NumberOfVideoAngles"]);
                $VideoTitleSetNo        = value_or_null($attributes["VideoTitleSetNo"]);
                $TitleNo                = value_or_null($attributes["TitleNo"]);

                $strSQL = "INSERT INTO `DVDPTTVMG` (`DVDVMGMKey`, `TitleSetNo`, `Title`, `PlaybackType`, `NumberOfVideoAngles`, `ParentalMgmMaskVMG`, `ParentalMgmMaskVTS`, `VideoTitleSetNo`, `TitleNo`) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

                $stmt = $mysqli->prepare($strSQL);

                if (!$stmt)
                {
                    $ResponseText = "Error preparing insert statement for DVDPTTVMG table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                }

                if (!$stmt->bind_param("iisiiiiii", $idDVDVMGM, $TitleSetNo, $Title, $PlaybackType, $NumberOfVideoAngles, $ParentalMgmMaskVMG, $ParentalMgmMaskVTS, $VideoTitleSetNo, $TitleNo))
                {
                    $ResponseText = "Error binding parameters for DVDPTTVMG table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                    $stmt->close();
                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);

                }

                if (!$stmt->execute())
                {
                    $ResponseText = "Error executing statement on DVDPTTVMG table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                    $stmt->close();
                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                }

                $idDVDPTTVMG = $mysqli->insert_id;

                $stmt->close();

                foreach ($tagDVDPTTVMG->DVDPTTVTS as $tagDVDPTTVTS)
                {
                    // Insert DVDPTTVTS (Chapter)
                    $attributes             = $tagDVDPTTVTS->attributes();

                    $Title                  = value_or_null($tagDVDPTTVTS->Title);
                    $Artist                 = value_or_null($tagDVDPTTVTS->Artist);

                    $ProgramChainNo         = value_or_null($attributes["ProgramChainNo"]);
                    $ProgramNo              = value_or_null($attributes["ProgramNo"]);
                    $PttTitleSetNo          = value_or_null($attributes["PttTitleSetNo"]);
                    $PttChapterNo           = value_or_null($attributes["Number"]);
                    $TitleSetNo             = value_or_null($attributes["TitleSetNo"]);

                    $strSQL = "INSERT INTO `DVDPTTVTS` (`DVDPTTVMGKey`, `Artist`, `Title`, `ProgramChainNo`, `ProgramNo`, `PttTitleSetNo`, `PttChapterNo`, `TitleSetNo`) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

                    $stmt = $mysqli->prepare($strSQL);

                    if (!$stmt)
                    {
                        $ResponseText = "Error preparing insert statement for DVDPTTVTS table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                    }

                    if (!$stmt->bind_param("issiiiii", $idDVDPTTVMG, $Artist, $Title, $ProgramChainNo, $ProgramNo, $PttTitleSetNo, $PttChapterNo, $TitleSetNo))
                    {
                        $ResponseText = "Error binding parameters for DVDPTTVTS table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                        $stmt->close();
                        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);

                    }

                    if (!$stmt->execute())
                    {
                        $ResponseText = "Error executing statement on DVDPTTVTS table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                        $stmt->close();
                        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                    }

                    //$idDVDPTTVTS = $mysqli->insert_id;

                    $stmt->close();
                }
            }

            // DVDVIDEOSTREAM
            foreach ($tagDVDVMGM->DVDVIDEOSTREAM as $tagDVDVIDEOSTREAM)
            {
                addVideoStream($mysqli, $tagDVDVIDEOSTREAM, $idDVDVMGM, null);
            }

            // DVDAUDIOSTREAM
            foreach ($tagDVDVMGM->DVDAUDIOSTREAM as $tagDVDAUDIOSTREAM)
            {
                addAudioStream($mysqli, $tagDVDAUDIOSTREAM, $idDVDVMGM, null);
            }

            // DVDSUBPICSTREAM
            foreach ($tagDVDVMGM->DVDSUBPICSTREAM as $tagDVDSUBPICSTREAM)
            {
                addSubpictureStream($mysqli, $tagDVDSUBPICSTREAM, $idDVDVMGM, null);
            }

            // Fileset
            foreach ($tagDVDVMGM->DVDFILE as $tagDVDFILE)
            {
                addFileset($mysqli, $tagDVDFILE, $idDVDVMGM, null);
            }
        }
        else
        {
            // Found: do an update
            $strSQL = "UPDATE `DVDVMGM` SET `Album` = ?, `AlbumArtist` = ?, `Genre` = ?, `Cast` = ?, `Crew` = ?, `Director` = ?, `Country` = ?, `ReleaseDate` = ?, `SpecialFeatures` = ?, `EAN_UPC` = ?, `Storyline` = ?, `Remarks` = ?, `Submitter` = ?, `SubmitterIP` = ?, `Client` = ?, `Keywords` = ?, `RegionProhibited1` = ?, `RegionProhibited2` = ?, `RegionProhibited3` = ?, `RegionProhibited4` = ?, `RegionProhibited5` = ?, `RegionProhibited6` = ?, `RegionProhibited7` = ?, `RegionProhibited8` = ?, `VersionNumberMajor` = ?, `VersionNumberMinor` = ?, `NumberOfVolumes` = ?, `VolumeNumber` = ?, `OriginalAlbum` = ?, `Screenplay` = ?, `Producer` = ?, `Editing` = ?, `Cinematography` = ?, `OriginalLanguage` = ?, `SideID` = ? WHERE `idDVDVMGM` = ?;";

            $stmt = $mysqli->prepare($strSQL);

            if (!$stmt)
            {
                $ResponseText = "Error preparing update statement for DVDVMGM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
            }

            if (!$stmt->bind_param("ssssssssssssssssiiiiiiiiiiiissssssii", $Album, $AlbumArtist, $Genre, $Cast, $Crew, $Director, $Country, $ReleaseDate, $SpecialFeatures, $EAN_UPC, $Storyline, $Remarks, $Submitter, $SubmitterIP, $Client, $Keywords, $RegionProhibited1, $RegionProhibited2, $RegionProhibited3, $RegionProhibited4, $RegionProhibited5, $RegionProhibited6, $RegionProhibited7, $RegionProhibited8, $VersionNumberMajor, $VersionNumberMinor, $NumberOfVolumes, $VolumeNumber, $OriginalAlbum, $Screenplay, $Producer, $Editing, $Cinematography, $OriginalLanguage, $SideID, $idDVDVMGM))
            {
                $ResponseText = "Error binding parameters for DVDVMGM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                $stmt->close();
                $stmt = null;
                throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
            }

            if (!$stmt->execute())
            {
                $ResponseText = "Error executing statement on DVDVMGM table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                $stmt->close();
                $stmt = null;
                throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
            }

            $stmt->close();
            $stmt = null;

            // Physical structure

            $tagPhysical = $tagDVDVMGM->physical;

            foreach ($tagPhysical->DVDVTS as $tagDVDVTS)
            {
                // Update DVDVTS (Video Title Set)
                $attributes         = $tagDVDVTS->attributes();

                $TitleSetNo         = $attributes["TitleSetNo"];
                $VersionNumberMajor = value_or_null($attributes["VersionNumberMajor"]);
                $VersionNumberMinor = value_or_null($attributes["VersionNumberMinor"]);

                $idDVDVTS = getPrimaryKey($mysqli, "DVDVTS", "idDVDVTS", "`DVDVMGMKey` = $idDVDVMGM AND `TitleSetNo` = $TitleSetNo");

                $strSQL = "UPDATE `DVDVTS` SET `TitleSetNo` = ?, `VersionNumberMajor` = ?, `VersionNumberMinor` = ? WHERE `idDVDVTS` = ?;";

                $stmt = $mysqli->prepare($strSQL);

                if (!$stmt)
                {
                    $ResponseText = "Error preparing update statement for DVDVTS table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                }

                if (!$stmt->bind_param("iiii", $TitleSetNo, $VersionNumberMajor, $VersionNumberMinor, $idDVDVTS))
                {
                    $ResponseText = "Error binding parameters for DVDVTS table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                    $stmt->close();
                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                }

                if (!$stmt->execute())
                {
                    $ResponseText = "Error executing statement on DVDVTS table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                    $stmt->close();
                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                }

                $stmt->close();

                foreach ($tagDVDVTS->DVDPGC as $tagDVDPGC)
                {
                    // Update DVDPGC (Program Chain)
                    $attributes     = $tagDVDPGC->attributes();

                    $ProgramChainNo = value_or_null($attributes["Number"]);
                    $EntryPGC       = value_or_null($attributes["EntryPGC"]);

                    $idDVDPGC = getPrimaryKey($mysqli, "DVDPGC", "idDVDPGC", "`DVDVTSKey` = $idDVDVTS AND `ProgramChainNo` = $ProgramChainNo");

                    $strSQL = "UPDATE `DVDPGC` SET `EntryPGC` = ? WHERE `idDVDPGC` = ?;";

                    $stmt = $mysqli->prepare($strSQL);

                    if (!$stmt)
                    {
                        $ResponseText = "Error preparing insert statement for DVDPGC table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                    }

                    if (!$stmt->bind_param("ii", $EntryPGC, $idDVDPGC))
                    {
                        $ResponseText = "Error binding parameters for DVDPGC table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                        $stmt->close();
                        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                    }

                    if (!$stmt->execute())
                    {
                        $ResponseText = "Error executing statement on DVDPGC table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                        $stmt->close();
                        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                    }

                    $stmt->close();

                    foreach ($tagDVDPGC->DVDPROGRAM as $tagDVDPROGRAM)
                    {
                        // Update DVDPGC (Program)
                        $attributes     = $tagDVDPROGRAM->attributes();

                        $ProgramNo      = value_or_null($attributes["Number"]);

                        // Nothing to update...

                        $idDVDPROGRAM = getPrimaryKey($mysqli, "DVDPROGRAM", "idDVDPROGRAM", "`DVDPGCKey` = $idDVDPGC AND `ProgramNo` = $ProgramNo");

                        foreach ($tagDVDPROGRAM->DVDCELL as $tagDVDCELL)
                        {
                            // Update DVDCELL (Cell)
                            $attributes                 = $tagDVDCELL->attributes();

                            $CellNo                     = value_or_null($attributes["Number"]);
                            $CellType                   = value_or_null($attributes["CellType"]);
                            $BlockType                  = value_or_null($attributes["BlockType"]);
                            $SeamlessMultiplex          = value_or_null($attributes["SeamlessMultiplex"]);
                            $Interleaved                = value_or_null($attributes["Interleaved"]);
                            $SCRdiscontinuity           = value_or_null($attributes["SCRdiscontinuity"]);
                            $SeamlessAngleLinkedInDSI   = value_or_null($attributes["SeamlessAngleLinkedInDSI"]);
                            $VOBStillMode               = value_or_null($attributes["VOBStillMode"]);
                            $StopsTrickPlay             = value_or_null($attributes["StopsTrickPlay"]);
                            $CellStillTime              = value_or_null($attributes["CellStillTime"]);
                            $CellCommand                = value_or_null($attributes["CellCommand"]);
                            $PlayTime                   = value_or_null($attributes["PlayTime"]);
                            $FrameRate                  = value_or_null($attributes["FrameRate"]);
                            $FirstVOBUStartSector       = value_or_null($attributes["FirstVOBUStartSector"]);
                            $FirstILVUEndSector         = value_or_null($attributes["FirstILVUEndSector"]);
                            $LastVOBUStartSector        = value_or_null($attributes["LastVOBUStartSector"]);
                            $LastVOBUEndSector          = value_or_null($attributes["LastVOBUEndSector"]);
                            $VOBidn                     = value_or_null($attributes["VOBidn"]);
                            $CELLidn                    = value_or_null($attributes["CELLidn"]);
                            $NumberOfVOBIds             = value_or_null($attributes["NumberOfVOBIds"]);

                            $idDVDCELL = getPrimaryKey($mysqli, "DVDCELL", "idDVDCELL", "`DVDPROGRAMKey` = $idDVDPROGRAM AND `CellNo` = $CellNo");

                            $strSQL = "UPDATE `DVDCELL` SET `CellType` = ?, `BlockType` = ?, `SeamlessMultiplex` = ?, `Interleaved` = ?, `SCRdiscontinuity` = ?, `SeamlessAngleLinkedInDSI` = ?, `VOBStillMode` = ?, `StopsTrickPlay` = ?, `CellStillTime` = ?, `CellCommand` = ?, `PlayTime` = ?, `FrameRate` = ?, `FirstVOBUStartSector` = ?, `FirstILVUEndSector` = ?, `LastVOBUStartSector` = ?, `LastVOBUEndSector` = ?, `VOBidn` = ?, `CELLidn` = ?, `NumberOfVOBIds` = ? WHERE `idDVDCELL` = ?;";

                            $stmt = $mysqli->prepare($strSQL);

                            if (!$stmt)
                            {
                                $ResponseText = "Error preparing insert statement for DVDCELL table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                                throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                            }

                            if (!$stmt->bind_param("ssiiiiiiiiiiiiiiiiii", $CellType, $BlockType, $SeamlessMultiplex, $Interleaved, $SCRdiscontinuity, $SeamlessAngleLinkedInDSI, $VOBStillMode, $StopsTrickPlay, $CellStillTime, $CellCommand, $PlayTime, $FrameRate, $FirstVOBUStartSector, $FirstILVUEndSector, $LastVOBUStartSector, $LastVOBUEndSector, $VOBidn, $CELLidn, $NumberOfVOBIds, $idDVDCELL))
                            {
                                $ResponseText = "Error binding parameters for DVDCELL table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                                $stmt->close();
                                throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                            }

                            if (!$stmt->execute())
                            {
                                $ResponseText = "Error executing statement on DVDCELL table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                                $stmt->close();
                                throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                            }

                            $stmt->close();

                            foreach ($tagDVDCELL->DVDUNIT as $tagDVDUNIT)
                            {
                                // Update DVDUNIT (Unit)
                                $attributes                 = $tagDVDUNIT->attributes();

                                $UnitNo                     = value_or_null($attributes["Number"]);
                                $StartSector                = value_or_null($attributes["StartSector"]);
                                $EndSector                  = value_or_null($attributes["EndSector"]);

                                $strSQL = "UPDATE `DVDUNIT` SET `StartSector` = ?, `EndSector` = ? WHERE `DVDCELLKey` = ? AND `UnitNo` = ?;";

                                $stmt = $mysqli->prepare($strSQL);

                                if (!$stmt)
                                {
                                    $ResponseText = "Error preparing insert statement for DVDUNIT table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                                }

                                if (!$stmt->bind_param("iiii", $StartSector, $EndSector, $idDVDCELL, $UnitNo))
                                {
                                    $ResponseText = "Error binding parameters for DVDUNIT table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                                    $stmt->close();
                                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                                }

                                if (!$stmt->execute())
                                {
                                    $ResponseText = "Error executing statement on DVDUNIT table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                                    $stmt->close();
                                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                                }

                                //$idDVDUNIT = $mysqli->insert_id;

                                $stmt->close();
                            }
                        }
                    }
                }

                // DVDVIDEOSTREAM
                foreach ($tagDVDVTS->DVDVIDEOSTREAM as $tagDVDVIDEOSTREAM)
                {
                    updateVideoStream($mysqli, $tagDVDVIDEOSTREAM, null, $idDVDVTS);
                }

                // DVDAUDIOSTREAM
                foreach ($tagDVDVTS->DVDAUDIOSTREAM as $tagDVDAUDIOSTREAM)
                {
                    updateAudioStream($mysqli, $tagDVDAUDIOSTREAM, null, $idDVDVTS);
                }

                // DVDAUDIOSTREAMEX
                foreach ($tagDVDVTS->DVDAUDIOSTREAMEX as $audiostreamExTag)
                {
                    updateAudioStreamEx($mysqli, $audiostreamExTag, $idDVDVTS);
                }

                // DVDSUBPICSTREAM
                foreach ($tagDVDVTS->DVDSUBPICSTREAM as $tagDVDSUBPICSTREAM)
                {
                    updateSubpictureStream($mysqli, $tagDVDSUBPICSTREAM, null, $idDVDVTS);
                }

                // Fileset
                foreach ($tagDVDVTS->DVDFILE as $tagDVDFILE)
                {
                    updateFileset($mysqli, $tagDVDFILE, null, $idDVDVTS);
                }
            }

            // Virtual structure

            $tagVirtual = $tagDVDVMGM->virtual;

            foreach ($tagVirtual->DVDPTTVMG as $tagDVDPTTVMG)
            {
                // Update DVDPTTVMG (Video Title Set)
                $attributes             = $tagDVDPTTVMG->attributes();

                $Title                  = value_or_null($tagDVDPTTVMG->Title);

                $TitleSetNo             = value_or_null($attributes["TitleSetNo"]);
                $PlaybackType           = value_or_null($attributes["PlaybackType"]);
                $NumberOfVideoAngles    = value_or_null($attributes["NumberOfVideoAngles"]);
                $ParentalMgmMaskVMG     = value_or_null($attributes["ParentalMgmMaskVMG"]);
                $ParentalMgmMaskVTS     = value_or_null($attributes["ParentalMgmMaskVTS"]);
                $NumberOfVideoAngles    = value_or_null($attributes["NumberOfVideoAngles"]);
                $VideoTitleSetNo        = value_or_null($attributes["VideoTitleSetNo"]);
                $TitleNo                = value_or_null($attributes["TitleNo"]);

                $idDVDPTTVMG = getPrimaryKey($mysqli, "DVDPTTVMG", "idDVDPTTVMG", "`DVDVMGMKey` = $idDVDVMGM AND `TitleSetNo` = $TitleSetNo");

                $strSQL = "UPDATE `DVDPTTVMG` SET `Title` = ?, `PlaybackType` = ?, `NumberOfVideoAngles` = ?, `ParentalMgmMaskVMG` = ?, `ParentalMgmMaskVTS` = ?, `VideoTitleSetNo` = ?, `TitleNo` = ? WHERE `idDVDPTTVMG` = ?;";

                $stmt = $mysqli->prepare($strSQL);

                if (!$stmt)
                {
                    $ResponseText = "Error preparing insert statement for DVDPTTVMG table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                }

                if (!$stmt->bind_param("siiiiiii", $Title, $PlaybackType, $NumberOfVideoAngles, $ParentalMgmMaskVMG, $ParentalMgmMaskVTS, $VideoTitleSetNo, $TitleNo, $idDVDPTTVMG))
                {
                    $ResponseText = "Error binding parameters for DVDPTTVMG table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                    $stmt->close();
                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);

                }

                if (!$stmt->execute())
                {
                    $ResponseText = "Error executing statement on DVDPTTVMG table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                    $stmt->close();
                    throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                }

                $stmt->close();

                foreach ($tagDVDPTTVMG->DVDPTTVTS as $tagDVDPTTVTS)
                {
                    // Update DVDPTTVTS (Chapter)
                    $attributes             = $tagDVDPTTVTS->attributes();

                    $Title                  = value_or_null($tagDVDPTTVTS->Title);
                    $Artist                 = value_or_null($tagDVDPTTVTS->Artist);

                    $ProgramChainNo         = value_or_null($attributes["ProgramChainNo"]);
                    $ProgramNo              = value_or_null($attributes["ProgramNo"]);
                    $PttTitleSetNo          = value_or_null($attributes["PttTitleSetNo"]);
                    $PttChapterNo           = value_or_null($attributes["Number"]);
                    $TitleSetNo             = value_or_null($attributes["TitleSetNo"]);

                    $strSQL = "UPDATE `DVDPTTVTS` SET `Artist` = ?, `Title` = ?, `ProgramChainNo` = ?, `ProgramNo` = ?, `PttTitleSetNo` = ?, TitleSetNo = ? WHERE `DVDPTTVMGKey` = ? AND `PttChapterNo` = ? ;";

                    $stmt = $mysqli->prepare($strSQL);

                    if (!$stmt)
                    {
                        $ResponseText = "Error preparing insert statement for DVDPTTVTS table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                    }

                    if (!$stmt->bind_param("ssiiiiii", $Artist, $Title, $ProgramChainNo, $ProgramNo, $PttTitleSetNo, $TitleSetNo, $idDVDPTTVMG, $PttChapterNo))
                    {
                        $ResponseText = "Error binding parameters for DVDPTTVTS table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                        $stmt->close();
                        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);

                    }

                    if (!$stmt->execute())
                    {
                        $ResponseText = "Error executing statement on DVDPTTVTS table.\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
                        $stmt->close();
                        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
                    }

                    //$idDVDPTTVTS = $mysqli->insert_id;

                    $stmt->close();
                }
            }

            // DVDVIDEOSTREAM
            foreach ($tagDVDVMGM->DVDVIDEOSTREAM as $tagDVDVIDEOSTREAM)
            {
                updateVideoStream($mysqli, $tagDVDVIDEOSTREAM, $idDVDVMGM, null);
            }

            // DVDAUDIOSTREAM
            foreach ($tagDVDVMGM->DVDAUDIOSTREAM as $tagDVDAUDIOSTREAM)
            {
                updateAudioStream($mysqli, $tagDVDAUDIOSTREAM, $idDVDVMGM, null);
            }

            // DVDSUBPICSTREAM
            foreach ($tagDVDVMGM->DVDSUBPICSTREAM as $tagDVDSUBPICSTREAM)
            {
                updateSubpictureStream($mysqli, $tagDVDSUBPICSTREAM, $idDVDVMGM, null);
            }

            // Fileset
            foreach ($tagDVDVMGM->DVDFILE as $tagDVDFILE)
            {
                updateFileset($mysqli, $tagDVDFILE, $idDVDVMGM, null);
            }
        }
    }

    // Commit the transaction
    $rs = query_server($mysqli, "COMMIT;");
}

?>
