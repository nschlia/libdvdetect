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

/*! \file query.php
 *
 *  \brief Process client query
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

		if (!$xml)
        {
            $Result = XMLRESULT_XML_ERROR;
            $ResponseText = get_xml_error($xmlstr);
            throw new Exception($ResponseText);
        }

    	$dvdTag = $xml->DVD[0];
    	$attributes = $dvdTag->attributes();
    	$Hash = $attributes["Hash"];

    	$mysqli = connect_server();

    	// Check connection
        if (!$mysqli)
        {
            $Result = XMLRESULT_SQL_ERROR;
            $ResponseText = mysqli_connect_error();
            throw new Exception($ResponseText);
        }

        $strSQL = "SELECT idDvd, Album, AlbumArtist, Revision, RowCreationDate, Genre, Cast, Crew, Director, Country, ReleaseDate, SpecialFeatures, EAN_UPC, Storyline, Submitter, SubmitterIP, Client, Remarks, RowLastChanged, Keywords FROM dvd ".
                  "WHERE Hash = '" . $Hash . "' AND Active = 1 " .
                  "ORDER BY Revision DESC;";

        $rsDvd = query_server($mysqli, $strSQL);

        $found = FALSE;

        while (is_array($Cols = $rsDvd->fetch_row()))
        {
            $found = TRUE;

            $idDvd              = $Cols[0];
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

            $dvdTag = $domtree->createElement("DVD");

            $dvdTag->setAttribute("Hash", $Hash);
            safeSetAttribute($dvdTag, "Album", $Album);
            safeSetAttribute($dvdTag, "AlbumArtist", $AlbumArtist);
            safeSetAttribute($dvdTag, "Revision", $Revision);
            safeSetAttribute($dvdTag, "DateCreated", $RowCreationDate);
            safeSetAttribute($dvdTag, "Genre", $Genre);
            safeSetAttribute($dvdTag, "Cast", $Cast);
            safeSetAttribute($dvdTag, "Crew", $Crew);
            safeSetAttribute($dvdTag, "Director", $Director);
            safeSetAttribute($dvdTag, "Country", $Country);
            safeSetAttribute($dvdTag, "ReleaseDate", $ReleaseDate);
            safeSetAttribute($dvdTag, "DateLastChanged", $RowLastChanged);
            safeSetAttribute($dvdTag, "SpecialFeatures", $SpecialFeatures);
            safeSetAttribute($dvdTag, "EAN_UPC", $EAN_UPC);
            safeSetAttribute($dvdTag, "Storyline", $Storyline);
            safeSetAttribute($dvdTag, "Submitter", $Submitter);
            safeSetAttribute($dvdTag, "SubmitterIP", $SubmitterIP);
            safeSetAttribute($dvdTag, "Client", $Client);
            safeSetAttribute($dvdTag, "Remarks", $Remarks);
            safeSetAttribute($dvdTag, "Keywords", $Keywords);

            $dvdTag = $xmlRoot->appendChild($dvdTag);

            $rsTitle = query_server($mysqli, "SELECT idTitleSet, TitleSetNo, Angles, Title FROM titleset WHERE DvdKey = " . $idDvd);

            while (is_array($Cols = $rsTitle->fetch_row()))
            {
                $idTitleSet         = $Cols[0];
                $TitleSetNo         = $Cols[1];
                $Angles             = $Cols[2];
                $Title              = $Cols[3];

                $titleTag = $domtree->createElement("title");
                $titleTag->setAttribute("TitleSetNo", $TitleSetNo);
                $titleTag->setAttribute("Angles", $Angles);
                safeSetAttribute($titleTag, "Title", $Title);
                $titleTag = $dvdTag->appendChild($titleTag);

                $rsChapter = query_server($mysqli, "SELECT idChapter, Number, PlayTime, Artist, Title FROM chapter WHERE TitleSetKey = " . $idTitleSet);

                while (is_array($Cols = $rsChapter->fetch_row()))
                {
                    $Number     = $Cols[1];
                    $PlayTime   = $Cols[2];
                    $Artist     = $Cols[3];
                    $Title      = $Cols[4];

                    $chapterTag = $domtree->createElement("chapter");
                    $chapterTag->setAttribute("Number", $Number);
                    $chapterTag->setAttribute("PlayTime", $PlayTime);
                    safeSetAttribute($chapterTag, "Artist", $Artist);
                    safeSetAttribute($chapterTag, "Title", $Title);
                    //$chapterTag->appendChild($domtree->createElement('Artist', $Artist));
                    //$chapterTag->appendChild($domtree->createElement('Title', $Title));
                    $chapterTag = $titleTag->appendChild($chapterTag);
                }
                // free result set
                $rsChapter->close();
                $rsChapter = null;
            }

            // free result set
            $rsTitle->close();
            $rsTitle = null;

            break; // Only one DVD for now
        }

        if (!$found)
        {
            $Result = XMLRESULT_NOTFOUND;
            $ResponseText = "No data found";
        }
        else
        {
            $Result = XMLRESULT_SUCCESS;
            $ResponseText = "Operation successful";
        }

        // free result set
        $rsDvd->close();
        $rsDvd = null;
    }
    catch (Exception $e)
    {
    }

    disconnect_server($mysqli);
    $mysqli = null;

    $responseTag->setAttribute("Result", $Result);
    $responseTag->appendChild($domtree->createElement('ResponseText', $ResponseText));
    $responseTag = null;

    // Send the xml document out
    header('Content-type: text/xml');
    echo $domtree->saveXML();
    $domtree = null;
?>
