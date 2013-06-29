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

/*! \file search.php
 *
 *  \brief Process client search request
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

    $Result = XMLRESULT_NOT_IMPLEMENTED;
    $ResponseText = "Not implemented";

    $XmlMode = XMLMODE_RESPONSE;
    $mysqli = null;

    try
    {
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
   	}
    catch (Exception $e)
    {
    }

    $responseTag->setAttribute("Result", $Result);
    $responseTag->appendChild($domtree->createElement('ResponseText', $ResponseText));

    // Send the xml document out
    header('Content-type: text/xml');
    echo $domtree->saveXML();
?>
