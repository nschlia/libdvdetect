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

// Settings
define("MYSQLSERVER",                       "localhost");
define("MYSQLDATABASE",                     "dbdvdetect");
define("MYSQLUSERNAME",                     "dvdetect");
define("MYSQLPASSWORD",                     "hZBCvEtHJ3T6GyqJ");

if (!defined('PHP_VERSION_ID'))
{
    $version = explode('.', PHP_VERSION);

    define('PHP_VERSION_ID', ($version[0] * 10000 + $version[1] * 100 + $version[2]));
}

define("QUERY_SET", "idDVDVMGM, Album, AlbumArtist, Revision, RowCreationDate, Genre, Cast, Crew, Director, Country, ReleaseDate, SpecialFeatures, EAN_UPC, Storyline, Submitter, SubmitterIP, Client, Remarks, RowLastChanged, Keywords, Hash, RegionProhibited1, RegionProhibited2, RegionProhibited3, RegionProhibited4, RegionProhibited5, RegionProhibited6, RegionProhibited7, RegionProhibited8, VersionNumberMajor, VersionNumberMinor, NumberOfVolumes, VolumeNumber, SideID");

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
        $ResponseText = "Error executing command:\n\n" . $strSQL . "\n\nSQL Error: " . $mysqli->error . "\nSQL State: " . $mysqli->sqlstate;
        throw new Exception($ResponseText, XMLRESULT_SQL_ERROR);
    }

    return $rs;
}

function getPrimaryKey($mysqli, $table, $field, $where)
{
    $strSQL = "SELECT `$field` FROM `$table` WHERE $where;";

    $rs = query_server($mysqli, $strSQL);

    if (!is_array($Cols = $rs->fetch_row()))
    {
        return -1;
    }

    return $Cols[0];
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
    if (!isset($value) || (empty($value) && !is_numeric($value)))
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
    if (!isset($value) || (empty($value) && !is_numeric($value)))
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
    $value = trim($value);
    if (!isset($value) || (empty($value) && !is_numeric($value)))
    {
        return null;
    }

    return $value;
}

function date_or_null($value)
{
    $value = trim($value);
    if (!isset($value) || (empty($value) && !is_numeric($value)))
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
    $value = trim($value);
    if (!isset($value) || (empty($value) && !is_numeric($value)))
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
    $dateTime = trim($dateTime);
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
    $dateTime = trim($dateTime);
    if (preg_match("/^(\d{4})-(\d{2})-(\d{2}) ([01][0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])$/", $dateTime, $matches))
    {
        if (checkdate($matches[2], $matches[3], $matches[1]))
        {
            return true;
        }
    }

    return false;
}

?>
