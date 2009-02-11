#!/bin/bash
# Copyright 2009 Andreas Gölzer
# 
# This file is part of readESM.
# 
# readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
# 
# readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>.
echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" dir="ltr" lang="en-US">
	<head>
		<link rel="stylesheet" type="text/css" media="screen" href="auswertung/style.css"/>
		<title>Tachograph files index</title>
		<meta http-equiv="Content-Type" content="text/html; charset=latin1"/>
	</head>
<body>
<h1>Tachograph files index</h1>
<ul>' > index.html
for i in data/*.ESM; do
	base=$(basename $i)
	title=$(./readesm --infile "$i" --format=html --outfile "auswertung/$base.html")
	echo $(./readesm --infile "$i" --format=html --verbose=1 --outfile "auswertung/$base.verbose.html")
	echo "<li><a href='auswertung/$base.html'>$title</a> (<a href='auswertung/$base.verbose.html'>-v</a>) <small>from <a href='$i'>$base</a></small></li>" >> index.html
done
echo "</ul></body></html>" >> index.html
