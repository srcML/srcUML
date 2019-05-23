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
@file stereotype_attr.xsl

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
  method stereotype names 
  Note: Leave a single blank space after name
-->
<xsl:template name="method_get">get </xsl:template>
<xsl:template name="method_nonconstget">nonconstget </xsl:template>
<xsl:template name="method_predicate">predicate </xsl:template>
<xsl:template name="method_property">property </xsl:template>
<xsl:template name="method_voidaccessor">void-accessor </xsl:template>
<xsl:template name="method_set">set </xsl:template>
<xsl:template name="method_command">command </xsl:template>
<xsl:template name="method_non-void-command">non-void-command </xsl:template>
<xsl:template name="method_collaborational-predicate">controller </xsl:template>
<xsl:template name="method_collaborational-property">controller </xsl:template>
<xsl:template name="method_collaborational-voidaccessor">controller </xsl:template>
<xsl:template name="method_collaborational-command">controller </xsl:template>
<xsl:template name="method_collaborator">collaborator </xsl:template>
<xsl:template name="method_factory">factory </xsl:template>
<xsl:template name="method_stateless">stateless </xsl:template>
<xsl:template name="method_incidental">incidental </xsl:template>
<xsl:template name="method_empty">empty </xsl:template>

<xsl:template name="method_constructor">constructor </xsl:template>
<xsl:template name="method_copy-constructor">copy-constructor </xsl:template>

<!-- add stereotype attribut to function/constructor -->
<xsl:template match="src:function | src:constructor">

  <!-- copy existing method/constructor -->
  <xsl:copy>
    <xsl:apply-templates select="@*"/>
    <xsl:attribute name="stereotype"><xsl:value-of select="src:stereotype(.)"/></xsl:attribute> 
    <xsl:apply-templates select="node()"/>
  </xsl:copy>

</xsl:template>

<!--
  Section responsible for actually applying all of the stereotypes and annotating the source code with a comment.
-->

<func:function name="src:stereotype">
  <xsl:param name="context"/>

  <!-- classifies stereotypes using criteria from stereotype_base.xsl -->
  <xsl:variable name="stereotype">
    <xsl:variable name="raw_stereotype"><xsl:apply-templates select="$context" mode="stereotype"/></xsl:variable>
    <xsl:choose>
      <xsl:when test="$raw_stereotype != ''"><xsl:value-of select="$raw_stereotype"/></xsl:when>
      <xsl:otherwise>unclassified </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>

  <func:result select="normalize-space($stereotype)"/>
</func:function>


</xsl:stylesheet>