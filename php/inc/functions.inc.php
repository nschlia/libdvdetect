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

/*! \file functions.inc.php
 *
 *  \brief PHP function collection
 */

define("MYSQLSERVER",                       "localhost");
define("MYSQLDATABASE",                     "dbdvdetect");
define("MYSQLUSERNAME",                     "dvdetect");
define("MYSQLPASSWORD",                     "hZBCvEtHJ3T6GyqJ");

define("XMLVERSION",                        1);
define("LIBRARYVERSION",                    "0.22 PHP");
define("LIBRARYNAME",                       "libdvdetect library PHP");
define("CLIENTNAME",                        "libDVDetect PHP server");

define("XMLMODE_INVALID",                   0);
define("XMLMODE_QUERY",                     1);
define("XMLMODE_SEARCH",                    2);
define("XMLMODE_SUBMIT_CONDENSED",          3);
define("XMLMODE_SUBMIT_VERBOSE",            4);
define("XMLMODE_RESPONSE",                  5);

define("XMLRESULT_SUCCESS",                 0);
define("XMLRESULT_NOTFOUND",                1);
define("XMLRESULT_NOT_IMPLEMENTED",         2);
define("XMLRESULT_SQL_ERROR",               3);
define("XMLRESULT_DUPLICATE_SUBMISSION",    4);
define("XMLRESULT_XML_ERROR",               5);

function connect_server()
{
    $mysqli = new mysqli(MYSQLSERVER, MYSQLUSERNAME, MYSQLPASSWORD, MYSQLDATABASE);

    if (mysqli_connect_errno())
    {
        $mysqli = null;
    }
	
	if (!$mysqli->set_charset("utf8")) 
	{
    	// printf("Error loading character set utf8: %s\n", $mysqli->error);
        $mysqli->close;
        $mysqli = null;
	}	
	
    return $mysqli;
}

function query_server($mysqli, $strSQL)
{
    $rs = $mysqli->query($strSQL);                               // Send query
    if (!$rs)
    {
        die('Invalid query: ' . $mysqli->error);
    }

    return $rs;
}

function disconnect_server($mysqli)
{
    if ($mysqli)
    {
        $mysqli->close();
    }
}

function mask_sql($value)
{
    if (!isset($value))
    {
        return "NULL";
    }

    if (is_numeric($value) || preg_match('/^\d+$/', $value))
    {
        return $value;
    }

    return "'" . str_replace("'", "\'", $value) . "'";
}

function mask_datetime($value)
{
    if (!isset($value))
    {
        return "NULL";
    }

    if (!isValidDateTime($value))
    {
        return "NULL";
    }

    return "'" . $value . "'";
}

function value_or_null($value)
{
    if (!isset($value))
    {
        return null;
    }

    return $value;
}

function date_or_null($value)
{
    if (!isset($value))
    {
        return null;
    }

    if (!isValidDate($value))
    {
        return null;
    }

    return $value;
}

function datetime_or_null($value)
{
    if (!isset($value))
    {
        return null;
    }

    if (!isValidDateTime($value))
    {
        return null;
    }

    return $value;
}

function isValidDate($dateTime)
{
    if (preg_match("/^(\d{4})-(\d{2})-(\d{2})$/", $dateTime, $matches))
    {
        if (checkdate($matches[2], $matches[3], $matches[1]))
        {
            return true;
        }
    }

    return false;
}

function isValidDateTime($dateTime)
{
    if (preg_match("/^(\d{4})-(\d{2})-(\d{2}) ([01][0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])$/", $dateTime, $matches))
    {
        if (checkdate($matches[2], $matches[3], $matches[1]))
        {
            return true;
        }
    }

    return false;
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
        $Tag->setAttribute($field, $value);
    }
}
?>
