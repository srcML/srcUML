<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns="http://www.srcML.org/srcML/src" 
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns:str="http://exslt.org/strings"
    xmlns:func="http://exslt.org/functions"
    extension-element-prefixes="func"
    exclude-result-prefixes="src str"
    version="1.0">
<!-- 
@file stereotype.xsl

@copyright Copyright (C) 2018 srcML, LLC. (www.srcML.org)

The stereocode is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

The stereocode Toolkit is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with the stereocode Toolkit; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 -->

<!--
  Define what is to be inserted for each method in a comment. For method stereotypes, this
  is just the name of the stereotype. For method summarization, this is the summary
-->
<xsl:import href="stereotype_base.xsl"/>

<!--
  Section responsible for actually applying all of the stereotypes and annotating the source code with a comment.
-->

<xsl:template match="src:comment[@type='block'][following-sibling::*[1][self::src:function or self::src:constructor][@stereotype]]">

  <!-- copy start and contents of existing comment -->
  <xsl:copy>
    <xsl:copy-of select="@*"/>

  <!-- copy start and contents of existing comment -->
  <xsl:value-of select="substring(., 1, string-length(.) - 2)"/>

  <!-- insert generated stereotype comment -->
  <xsl:text>&#xa;</xsl:text>
  <xsl:variable name="lastws" select="src:last_ws(.)"/>
  <xsl:if test="string-length($lastws) = 0">
    <xsl:text>    </xsl:text>
  </xsl:if>
  <xsl:value-of select="$lastws"/>

<xsl:text>@stereotype </xsl:text><xsl:value-of select="current()/following-sibling::*[1]/@stereotype"/><xsl:text> </xsl:text>

  <!-- end the comment -->
  <xsl:if test="string-length($lastws) &gt; 0">
    <xsl:text>&#xa;</xsl:text><xsl:value-of select="src:last_ws(.)"/>
  </xsl:if>
  <xsl:text>*/</xsl:text></xsl:copy>

</xsl:template>

<!-- function/constructor which does not have a preceding block comment -->
<xsl:template match="src:*[self::src:function or self::src:constructor][not(preceding-sibling::*[1][self::src:comment[@type='block']])][@stereotype]">

  <!-- insert new stereotype comment -->

  <comment type="block"><xsl:text>/* @stereotype </xsl:text><xsl:value-of select="current()/@stereotype"/><xsl:text> */</xsl:text></comment><xsl:text>&#xa;</xsl:text>

  <!-- insert a copy of the indentation for the function, since we stole it for the inserted comment, must repeat it -->
  <xsl:value-of select="str:split(preceding-sibling::text()[1], '&#xa;')[last()]"/>

  <!-- copy existing method/constructor -->
  <xsl:copy-of select="."/>

</xsl:template>

</xsl:stylesheet>