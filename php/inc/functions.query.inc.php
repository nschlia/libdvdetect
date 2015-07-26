<?php
/*
  dvdetect DVD detection, analysis & DVDETECT lookup library

  Copyright (C) 2013-2015 Norbert Schlia <nschlia@dvdetect.de>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU LESSER GENERAL PUBLIC LICENSE for more details.

  You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*! \file functions.query.inc.php
 *
 *  \brief PHP function collection
 */

function query_dvd($mysqli, $xml, &$Hash, &$rsDVDVMGM)
{
    $dvdTag = $xml->DVD[0];
    $attributes = $dvdTag->attributes();
    $Hash = $attributes["Hash"];

    $strSQL = "SELECT " . QUERY_SET . " FROM DVDVMGM " .
                  "WHERE Hash = '" . $Hash . "' AND Active = 1 " .
                  "ORDER BY Revision DESC;";

    $rsDVDVMGM = query_server($mysqli, $strSQL);
}

?>
