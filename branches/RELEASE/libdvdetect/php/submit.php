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

/*! \file submit.php
 *
 *  \brief Process client submission request
 */

    include_once 'inc/functions.inc.php';

    //$path_parts = pathinfo($_SERVER['PHP_SELF']);
    //$strLangId = SaveSession("strLangId");
    //include_once 'inc/' . $strLangId . '/' . $path_parts["basename"];

    $xmlstr = $_POST["xml"];

    if (empty($xmlstr))
    {
        // Invalid or empty request: Redirect to db.dvdetect.de
        header('Location: http://www.dvdetect.de/');
    }

    $Result = XMLRESULT_SUCCESS;
    $ResponseText = "Operation successful";

    $XmlMode = XMLMODE_RESPONSE;
    $mysqli = null;

    try
    {
    	libxml_use_internal_errors(true);
        $xml = simplexml_load_string($xmlstr);
	
	if (!$xml)
        {
            $Result = XMLRESULT_XML_ERROR;
            $ResponseText = get_xml_error($xmlstr);
            throw new Exception($ResponseText);
        }

        $attributes = $xml->attributes();
        $XmlMode = $attributes["XmlMode"];
        $XmlVersion = $attributes["XmlVersion"];
        $LibraryVersion = $attributes["LibraryVersion"];
        $LibraryName = $attributes["LibraryName"];
        $ClientName = $attributes["ClientName"];

        $mysqli = connect_server();

        // Check connection
        if (!$mysqli)
        {
            $Result = XMLRESULT_SQL_ERROR;
            $ResponseText = mysqli_connect_error();
            throw new Exception($ResponseText);
        }

        // Start a transaction
        $rs = query_server($mysqli, "START TRANSACTION;");
        if (!$rs)
        {
            $Result = XMLRESULT_SQL_ERROR;
            $ResponseText = $mysqli->error;
            throw new Exception($ResponseText);
        }

        foreach ($xml->DVD as $dvdTag)
        {
            $attributes         = $dvdTag->attributes();

            $Hash               = value_or_null($attributes["Hash"]);
            $Album              = value_or_null($attributes["Album"]);
            $AlbumArtist        = value_or_null($attributes["AlbumArtist"]);
            $Genre              = value_or_null($attributes["Genre"]);
            $Cast               = value_or_null($attributes["Cast"]);
            $Crew               = value_or_null($attributes["Crew"]);
            $Director           = value_or_null($attributes["Director"]);
            $Country            = value_or_null($attributes["Country"]);
            $ReleaseDate        = date_or_null($attributes["ReleaseDate"]);
            $SpecialFeatures    = value_or_null($attributes["SpecialFeatures"]);
            $EAN_UPC            = value_or_null($attributes["EAN_UPC"]);
            $Storyline          = value_or_null($attributes["Storyline"]);
            $Submitter          = value_or_null($attributes["Submitter"]);
            $Client             = value_or_null($attributes["Client"]);
            $Remarks            = value_or_null($attributes["Remarks"]);
            $Keywords           = value_or_null($attributes["Keywords"]);
            $SubmitterIP        = value_or_null($_SERVER['REMOTE_ADDR']);

            // Check if dataset exists
            $found = FALSE;

            $strSQL = "SELECT idDvd, RowLastChanged, RowCreationDate, Submitter, SubmitterIP FROM dvd ".
                        "WHERE Hash = '" . $Hash . "' AND Active = 1 " .
                        "ORDER BY Revision DESC;";

            $rsDvd = query_server($mysqli, $strSQL);

            if (is_array($Cols = $rsDvd->fetch_row()))
            {
                $idDvd              = $Cols[0];
                $RowLastChanged     = $Cols[1];
                $RowCreationDate    = $Cols[2];
                $LastSubmitter      = $Cols[3];
                $LastSubmitterIP    = $Cols[4];
// TODO: ZEITRAUM
                if ($Submitter == $LastSubmitter && $SubmitterIP == $LastSubmitterIP)
                {
                    $found = TRUE;
                }
            }

            $rsDvd->close();

            if (!$found)
            {
                // Not found: insert new
                $strSQL = "INSERT INTO dvd (Hash, Album, AlbumArtist, Genre, Cast, Crew, Director, Country, ReleaseDate, SpecialFeatures, EAN_UPC, Storyline, Remarks, Submitter, SubmitterIP, Client, Keywords) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

                $stmt = $mysqli->prepare($strSQL);

                if (!$stmt)
                {
                    $Result = XMLRESULT_SQL_ERROR;
                    $ResponseText = $mysqli->error;
                    throw new Exception($ResponseText);
                }

                if (!$stmt->bind_param("sssssssssssssssss", $Hash, $Album, $AlbumArtist, $Genre, $Cast, $Crew, $Director, $Country, $ReleaseDate, $SpecialFeatures, $EAN_UPC, $Storyline, $Remarks, $Submitter, $SubmitterIP, $Client, $Keywords))
                {
                    $Result = XMLRESULT_SQL_ERROR;
                    $ResponseText = $mysqli->error;
                    $stmt->close();
                    $stmt = null;
                    throw new Exception($ResponseText);
                }

                if (!$stmt->execute())
                {
                    $Result = XMLRESULT_SQL_ERROR;
                    $ResponseText = $mysqli->error;
                    $stmt->close();
                    $stmt = null;
                    throw new Exception($ResponseText);
                }

                $idDvd = $mysqli->insert_id;

                $stmt->close();
                $stmt = null;

                foreach ($dvdTag->title as $TitleTag)
                {
                    $attributes         = $TitleTag->attributes();

                    $TitleSetNo         = $attributes["TitleSetNo"];

                    $Angles             = value_or_null($attributes["Angles"]);
                    $Title              = value_or_null($attributes["Title"]);
                    $VideoCodingMode    = value_or_null($attributes["VideoCodingMode"]);
                    $VideoStandard      = value_or_null($attributes["VideoStandard"]);
                    $VideoAspect        = value_or_null($attributes["VideoAspect"]);
                    $Resolution         = value_or_null($attributes["Resolution"]);
                    $LetterBoxed        = value_or_null($attributes["LetterBoxed"]);

                    $strSQL = "INSERT INTO titleset (DvdKey, TitleSetNo, Angles, Title, VideoCodingMode, VideoStandard, VideoAspect, Resolution, LetterBoxed) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

                    $stmt = $mysqli->prepare($strSQL);

                    if (!$stmt)
                    {
                        $Result = XMLRESULT_SQL_ERROR;
                        $ResponseText = $mysqli->error;
                        throw new Exception($ResponseText);
                    }

                    if (!$stmt->bind_param("iiisssssi", $idDvd, $TitleSetNo, $Angles, $Title, $VideoCodingMode, $VideoStandard, $VideoAspect, $Resolution, $LetterBoxed))
                    {
                        $Result = XMLRESULT_SQL_ERROR;
                        $ResponseText = $mysqli->error;
                        $stmt->close();
                        throw new Exception($ResponseText);
                    }

                    if (!$stmt->execute())
                    {
                        $Result = XMLRESULT_SQL_ERROR;
                        $ResponseText = $mysqli->error;
                        $stmt->close();
                        throw new Exception($ResponseText);
                    }

                    $idTitleSet = $mysqli->insert_id;

                    $stmt->close();

                    foreach ($TitleTag->chapter as $ChapterTag)
                    {
                        $attributes     = $ChapterTag->attributes();

                        $Number         = value_or_null($attributes["Number"]);
                        $PlayTime       = value_or_null($attributes["PlayTime"]);
                        $Artist         = value_or_null($attributes["Artist"]);
                        $Title          = value_or_null($attributes["Title"]);

                        $strSQL = "INSERT INTO chapter (TitleSetKey, Number, PlayTime, Artist, Title) VALUES (?, ?, ?, ?, ?);";

                        $stmt = $mysqli->prepare($strSQL);

                        if (!$stmt)
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            throw new Exception($ResponseText);
                        }

                        if (!$stmt->bind_param("iiiss", $idTitleSet, $Number, $PlayTime, $Artist, $Title))
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            $stmt->close();
                            throw new Exception($ResponseText);
                        }

                        if (!$stmt->execute())
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            $stmt->close();
                            throw new Exception($ResponseText);
                        }

                        $stmt->close();
                    }

                    // audiostream
                    foreach ($TitleTag->audiostream as $audiostreamTag)
                    {
                        $attributes     = $audiostreamTag->attributes();

                        $Number         = value_or_null($attributes["Number"]);
                        $SampleRate     = value_or_null($attributes["SampleRate"]);
                        $Channels       = value_or_null($attributes["Channels"]);
                        $Coding         = value_or_null($attributes["Coding"]);
                        $Quantisation   = value_or_null($attributes["Quantisation"]);
                        $Language       = value_or_null($attributes["Language"]);
                        $CodeExt        = value_or_null($attributes["CodeExt"]);

                        $strSQL = "INSERT INTO audiostream (TitleSetKey, Number, SampleRate, Channels, Quantisation, Coding, CodeExt, LanguageCode) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

                        $stmt = $mysqli->prepare($strSQL);

                        if (!$stmt)
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            throw new Exception($ResponseText);
                        }

                        if (!$stmt->bind_param("iiiissis", $idTitleSet, $Number, $SampleRate, $Channels, $Quantisation, $Coding, $CodeExt, $Language))
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            $stmt->close();
                            throw new Exception($ResponseText);
                        }

                        if (!$stmt->execute())
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            $stmt->close();
                            throw new Exception($ResponseText);
                        }

                        $stmt->close();
                    }

                    // subpicturestream
                    foreach ($TitleTag->subpicturestream as $subpicturestreamTag)
                    {
                        $attributes     = $subpicturestreamTag->attributes();

                        $Number          = value_or_null($attributes["Number"]);
                        $Language        = value_or_null($attributes["Language"]);
                        $CodeExt         = value_or_null($attributes["CodeExt"]);

                        $strSQL = "INSERT INTO subpicturestream (TitleSetKey, Number, LanguageCode, CodeExt) VALUES (?, ?, ?, ?);";

                        $stmt = $mysqli->prepare($strSQL);

                        if (!$stmt)
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            throw new Exception($ResponseText);
                        }

                        if (!$stmt->bind_param("iisi", $idTitleSet, $Number, $Language, $CodeExt))
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            $stmt->close();
                            throw new Exception($ResponseText);
                        }

                        if (!$stmt->execute())
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            $stmt->close();
                            throw new Exception($ResponseText);
                        }

                        $stmt->close();
                    }
                }
            }
            else
            {
                // Found: update
                $strSQL = "UPDATE dvd SET Album = ?, AlbumArtist = ?, Genre = ?, Cast = ?, Crew = ?, Director = ?, Country = ?, ReleaseDate = ?, SpecialFeatures = ?, EAN_UPC = ?, Storyline = ?, Remarks = ?, Submitter = ?, SubmitterIP = ?, Client = ?, Keywords = ? WHERE iddvd = ?;";

                $stmt = $mysqli->prepare($strSQL);

                if (!$stmt)
                {
                    $Result = XMLRESULT_SQL_ERROR;
                    $ResponseText = $mysqli->error;
                    throw new Exception($ResponseText);
                }

                if (!$stmt->bind_param("ssssssssssssssssi", $Album, $AlbumArtist, $Genre, $Cast, $Crew, $Director, $Country, $ReleaseDate, $SpecialFeatures, $EAN_UPC, $Storyline, $Remarks, $Submitter, $SubmitterIP, $Client, $Keywords, $idDvd))
                {
                    $Result = XMLRESULT_SQL_ERROR;
                    $ResponseText = $mysqli->error;
                    $stmt->close();
                    $stmt = null;
                    throw new Exception($ResponseText);
                }

                if (!$stmt->execute())
                {
                    $Result = XMLRESULT_SQL_ERROR;
                    $ResponseText = $mysqli->error;
                    $stmt->close();
                    $stmt = null;
                    throw new Exception($ResponseText);
                }

                $stmt->close();
                $stmt = null;

                foreach ($dvdTag->title as $TitleTag)
                {
                    $attributes         = $TitleTag->attributes();

                    $TitleSetNo         = $attributes["TitleSetNo"];
                    $Angles             = value_or_null($attributes["Angles"]);
                    $Title              = value_or_null($attributes["Title"]);
                    $VideoCodingMode    = value_or_null($attributes["VideoCodingMode"]);
                    $VideoStandard      = value_or_null($attributes["VideoStandard"]);
                    $VideoAspect        = value_or_null($attributes["VideoAspect"]);
                    $Resolution         = value_or_null($attributes["Resolution"]);
                    $LetterBoxed        = value_or_null($attributes["LetterBoxed"]);

                    $strSQL = "UPDATE titleset SET Angles = ?, Title = ?, VideoCodingMode = ?, VideoStandard = ?, VideoAspect = ?, Resolution = ?, LetterBoxed = ? WHERE dvdkey = ? AND TitleSetNo = ?;";

                    $stmt = $mysqli->prepare($strSQL);

                    if (!$stmt)
                    {
                        $Result = XMLRESULT_SQL_ERROR;
                        $ResponseText = $mysqli->error;
                        throw new Exception($ResponseText);
                    }

                    if (!$stmt->bind_param("isssssiii", $Angles, $Title, $VideoCodingMode, $VideoStandard, $VideoAspect, $Resolution, $LetterBoxed, $idDvd, $TitleSetNo))
                    {
                        $Result = XMLRESULT_SQL_ERROR;
                        $ResponseText = $mysqli->error;
                        $stmt->close();
                        throw new Exception($ResponseText);
                    }

                    if (!$stmt->execute())
                    {
                        $Result = XMLRESULT_SQL_ERROR;
                        $ResponseText = $mysqli->error;
                        $stmt->close();
                        throw new Exception($ResponseText);
                    }

                    $stmt->close();

                    foreach ($TitleTag->chapter as $ChapterTag)
                    {
                        $attributes     = $ChapterTag->attributes();

                        $Number         = value_or_null($attributes["Number"]);
                        $PlayTime       = value_or_null($attributes["PlayTime"]);
                        $Artist         = value_or_null($attributes["Artist"]);
                        $Title          = value_or_null($attributes["Title"]);

                        $strSQL = "UPDATE chapter SET PlayTime = ?, Artist = ?, Title = ? WHERE TitleSetKey = (SELECT idTitleSet FROM titleset WHERE dvdkey = ? AND TitleSetNo = ?) AND Number = ?;";

                        $stmt = $mysqli->prepare($strSQL);

                        if (!$stmt)
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            throw new Exception($ResponseText);
                        }

                        if (!$stmt->bind_param("issiii", $PlayTime, $Artist, $Title, $idDvd, $TitleSetNo, $Number))
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            $stmt->close();
                            throw new Exception($ResponseText);
                        }

                        if (!$stmt->execute())
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            $stmt->close();
                            throw new Exception($ResponseText);
                        }

                        $stmt->close();
                    }

                    // audiostream
                    foreach ($TitleTag->audiostream as $audiostreamTag)
                    {
                        $attributes     = $audiostreamTag->attributes();

                        $Number         = value_or_null($attributes["Number"]);
                        $SampleRate     = value_or_null($attributes["SampleRate"]);
                        $Channels       = value_or_null($attributes["Channels"]);
                        $Coding         = value_or_null($attributes["Coding"]);
                        $Quantisation   = value_or_null($attributes["Quantisation"]);
                        $Language       = value_or_null($attributes["Language"]);
                        $CodeExt        = value_or_null($attributes["CodeExt"]);

                        $strSQL = "UPDATE audiostream SET SampleRate = ?, Channels = ?, Quantisation = ?, Coding = ?, CodeExt = ?, LanguageCode = ? WHERE TitleSetKey = (SELECT idTitleSet FROM titleset WHERE dvdkey = ? AND TitleSetNo = ?) AND Number = ?;";

                        $stmt = $mysqli->prepare($strSQL);

                        if (!$stmt)
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            throw new Exception($ResponseText);
                        }

                        if (!$stmt->bind_param("iissisiii", $SampleRate, $Channels, $Quantisation, $Coding, $CodeExt, $Language, $idDvd, $TitleSetNo, $Number))
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            $stmt->close();
                            throw new Exception($ResponseText);
                        }

                        if (!$stmt->execute())
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            $stmt->close();
                            throw new Exception($ResponseText);
                        }

                        $stmt->close();
                    }

                    // subpicturestream
                    foreach ($TitleTag->subpicturestream as $subpicturestreamTag)
                    {
                        $attributes     = $subpicturestreamTag->attributes();

                        $Number          = value_or_null($attributes["Number"]);
                        $Language        = value_or_null($attributes["Language"]);
                        $CodeExt         = value_or_null($attributes["CodeExt"]);

                        $strSQL = "UPDATE subpicturestream SET TitleSetKey = ?, Number = ?, LanguageCode = ?, CodeExt = ?  WHERE TitleSetKey = (SELECT idTitleSet FROM titleset WHERE dvdkey = ? AND TitleSetNo = ?) AND Number = ?;";

                        $stmt = $mysqli->prepare($strSQL);

                        if (!$stmt)
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            throw new Exception($ResponseText);
                        }

                        if (!$stmt->bind_param("isiiii", $Number, $Language, $CodeExt, $idDvd, $TitleSetNo, $Number))
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            $stmt->close();
                            throw new Exception($ResponseText);
                        }

                        if (!$stmt->execute())
                        {
                            $Result = XMLRESULT_SQL_ERROR;
                            $ResponseText = $mysqli->error;
                            $stmt->close();
                            throw new Exception($ResponseText);
                        }

                        $stmt->close();
                    }
                }
            }
        }

        // Commit the transaction
        $rs = query_server($mysqli, "COMMIT;");
        if (!$rs)
        {
            $Result = XMLRESULT_SQL_ERROR;
            $ResponseText = $mysqli->error;
            throw new Exception($ResponseText);
        }
    }
    catch (Exception $e)
    {
    }

    disconnect_server($mysqli);
    $mysqli = null;

    // create a dom document with encoding utf8
    $domtree = new DOMDocument('1.0', 'UTF-8');

    // create the root element of the xml tree
    $xmlRoot = $domtree->createElement("xml");

    $comment = $domtree->createComment("libdvdetect data exchange");
    $xmlRoot->appendChild($comment);

    // append it to the document created
    $xmlRoot = $domtree->appendChild($xmlRoot);

    $xmlRoot->setAttribute("XmlMode", $XmlMode);
    $xmlRoot->setAttribute("XmlVersion", XMLVERSION);
    $xmlRoot->setAttribute("LibraryVersion", LIBRARYVERSION);
    $xmlRoot->setAttribute("LibraryName", LIBRARYNAME);
    $xmlRoot->setAttribute("ClientName", CLIENTNAME);

    $responseTag = $domtree->createElement("response");
    $xmlRoot->appendChild($responseTag);

    $responseTag->setAttribute("Result", $Result);
    $responseTag->appendChild($domtree->createElement('ResponseText', $ResponseText));

    // Send the xml document out
    header('Content-type: text/xml');
    echo $domtree->saveXML();
?>
