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

/*! \file submit.php
 *
 *  \brief Process client submission request
 */

    include_once 'inc/functions.inc.php';
    include_once 'inc/functions.buildxml.inc.php';
    include_once 'inc/functions.submit.inc.php';

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

    $XmlMode = XMLMODE_SUBMIT_RESPONSE;
    $mysqli = null;

    try
    {
        createXmlDocument($domtree, $xmlRoot, $XmlMode);

        libxml_use_internal_errors(true);
        $xml = simplexml_load_string($xmlstr);

        if (!$xml)
        {
            $ResponseText = "XML Error: " . get_xml_error($xmlstr);
            throw new Exception($ResponseText, XMLRESULT_XML_ERROR);
        }

        $attributes = $xml->attributes();
        $XmlMode            = $attributes["XmlMode"];
        $ProtocolVersion    = $attributes["ProtocolVersion"];
        $LibraryVersion     = $attributes["LibraryVersion"];
        $LibraryName        = $attributes["LibraryName"];
        $ClientName         = $attributes["ClientName"];

        // Use requested version
        $xmlRoot->setAttribute("ProtocolVersion", $ProtocolVersion);

        checkVersion($ProtocolVersion, LIBDVDETECT_PROTOCOL_VERSION);

        $mysqli = connect_server();

        // Check connection
        if (!$mysqli)
        {
            $ResponseText = "Cannot connect to database.\nSQL Error: " . mysqli_connect_error();
            throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
        }

        submit($mysqli, $xml, $ProtocolVersion);
    }
    catch (Exception $e)
    {
        $Result = $e->getCode();
        $ResponseText = $e->getMessage();
    }

    disconnect_server($mysqli);
    $mysqli = null;

    // Create response
    $responseTag = $domtree->createElement("response");
    $responseTag->setAttribute("Result", $Result);
    $responseTag->appendChild($domtree->createElement('ResponseText', $ResponseText));
    $xmlRoot->appendChild($responseTag);
    $responseTag = null;

    // Send the xml document out
    header('Content-type: text/xml; charset=utf-8');
    $xml = $domtree->saveXML();
    header('Content-Length: ' . strlen($xml));

    print($xml);
    $domtree = null;
?>
