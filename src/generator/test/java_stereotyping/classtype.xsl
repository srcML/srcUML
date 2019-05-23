<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns="http://www.srcML.org/srcML/src" 
    xmlns:src="http://www.srcML.org/srcML/src"
    xmlns:exsl="http://exslt.org/common"
    xmlns:str="http://exslt.org/strings"
    xmlns:html="http://www.w3.org/1999/xhtml"
    xmlns:func="http://exslt.org/functions"
    extension-element-prefixes="exsl func"
    exclude-result-prefixes="src str"
    version="1.0">
<!-- 
@file classtype.xsl

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

<!-- primarily used for misc placement functions (last_ws) -->
<xsl:import href="stereotype_base.xsl"/>

<xsl:output method="xml" encoding="UTF-8"/>

<!-- Detection rules for class stereotypes -->

<!-- Minimal Entity
* The only method stereotypes are get, set, and command/non-void-command
  {methods} - ({get}∪ {set} ∪ {command} ∪ {non-void-command}) = ∅ & 
  |get| ≠ 0 & |set| ≠ 0 & ({command} ∪ {non-void-command}) ≠ ∅
* The ratio of collaborational to non-collaborational methods is 2:1
  |collaborators| / |non-collaborators| = 2
-->
<xsl:template match="src:class[

  (src:methods() - (src:count('get') + src:count('set') + src:count('command') + src:count('non-void-command')) = 0)

    and

  (src:count('get') and src:count('set') and (src:count('command') + src:count('non-void-command')))

    and

  ((src:collaborational2() div src:methods()) &lt;= (2 div 3))

]" mode="class_stereotype">Minimal Entity</xsl:template>

<!-- Entity

* accessor besides get and a mutator besides set
  |accessors| - |get| > 0 & |mutators| - |set| > 0
* ratio of collaborational to non-collaborational methods is 2:1
  |collaborators| / |non-collaborators| > 2
* They can have factory methods but no controller methods
  |controller| = 0
-->
<xsl:template match="src:class[

  ((src:accessor() - src:count('get')) &gt; 0) and
  ((src:mutator()  - src:count('set')) &gt; 0)

    and

  ((src:collaborational2() div src:methods()) &lt;= (2 div 3))

    and

  (not(src:count('controller')))

]" mode="class_stereotype">Entity&#xa;&#xa;  Data &amp; behavior encapsulation</xsl:template>

<!-- Data Provider

* It consists mostly of accessors
  |accessors| > 2 · |mutators|
* Low control of other classes
  |accessors| > 2 (|controller| + |factory|)
-->
<xsl:template match="src:class[

  (src:accessor() &gt; 2 * src:mutator())

      and

  (src:accessor() &gt; 2 * (src:count('controller') + src:count('factory')))

]" mode="class_stereotype">Data Provider&#xa;&#xa;  Mainly accessor methods</xsl:template>

<!-- Commander
* It consists mostly of mutators 
  |mutators| > 2 · |accessors|

* Low control of other classes
  |mutators| > 2 · (|controller| + |factory|)
-->
<xsl:template match="src:class[

  (src:mutator() &gt; (2 * src:accessor()))

    and

  (src:mutator() &gt; (2 * (src:count('controller') + src:count('factory'))))

]" mode="class_stereotype">Commander</xsl:template>

<!-- Boundary

* More collaborators then non-collaborators
  |collaborators| > |non-collaborators|
* Not all the methods are factory methods
  |factory| < 1⁄2 · |methods|
* Low number of controller methods
  |controller| < 1⁄3 · |methods|
-->
<xsl:template match="src:class[

  (src:collaborational2() &gt; (src:methods() - src:collaborational2()))

    and

  (src:count('factory') &lt; (src:methods() div 2))

    and

  (src:count('controller') &lt; (src:methods() div 3))


]" mode="class_stereotype">Boundary</xsl:template>


<!-- Factory
* Consists mostly of factory methods
  |factory| > 2⁄3 · |methods|
-->
<xsl:template match="src:class[

  src:count('factory') &gt; ((2 div 3) * src:methods())

]" mode="class_stereotype">Factory</xsl:template>

<!-- Controller

* High control of other classes
  |controller| + |factory| > 2⁄3 · |methods|
* Accessor or mutator are present (not only methods that work on external objects exist)
  |accessors| ≠ 0 ∨ |mutators| ≠ 0

-->
<xsl:template match="src:class[

  (src:count('controller') + src:count('factory')) &gt; ((2 div 3) * src:methods())

    and

  src:accessor() and src:mutator()

]" mode="class_stereotype">Controller</xsl:template>

<!-- Pure Controller
* Only controller and factory methods with no mutator, accessor, or collaborator methods
  |controller| + |factory| ≠ 0 &
  |accessors| + | mutators| + |collaborator| = 0
* There must be at least one controller method
  |controller| ≠ 0
-->
<xsl:template match="src:class[

  ((src:count('controller') + src:count('factory')) &gt; 0)

    and

  ((src:accessor() + src:mutator() + src:collaborational2()) = 0)

    and

  (src:count('controller') &gt; 0)

]" mode="class_stereotype">Pure Controller</xsl:template>

<!-- Large Class

* Categories of stereotypes (accessor with mutator) and stereotypes, factory and controller, are approximately in equal proportions
  1/5 · |methods| < |accessors| + |mutators|
  < 2⁄3 · |methods|
& 1/5 · |methods| < |factory| + |controller| < 2⁄3 · |methods|
* Controller and factory have to be present |factory| ≠ 0 & |controller|≠ 0
* Accessor and mutator have to be present |accessors| ≠ 0 & |mutators|≠ 0
* Number of methods in a class is high |methods| > average + stdev
  * Note, average and stdev of number of methods are calculated per system.
-->
<xsl:template match="src:class[

  ((src:methods() div 5) &lt; (src:accessor() + src:mutator()))

    and

  ((src:accessor() + src:mutator()) &lt; ((2 div 3) * src:methods()))

    and

  ((src:methods() div 5) &lt; (src:count('factory') + src:count('controller')))

    and

  ((src:count('factory') + src:count('controller')) &lt; ((2 div 3) * src:methods()))

    and

  src:count('factory') and src:count('controller')

    and

  src:accessor() and src:mutator()

]" mode="class_stereotype">Large Class</xsl:template>

<!-- Lazy Class

* It has to contain get/set methods
  |get| + |set| ≠ 0
* It might have a large number of degenerate methods
  |degenerate| / |methods| > 1⁄3
* Occurrence of other stereotypes is low
  |methods| – (|get| + |set| + |degenerate|) <= 1/5
-->
<xsl:template match="src:class[

 (((src:count('get') + src:count('set')) &gt; 0)

    and

  ((src:methods() - (src:count('get') + src:count('set') + src:degenerate())) &lt; (1 div 5)))

    or

  ((src:degenerate() div src:methods()) &gt; (1 div 3))

]" mode="class_stereotype">Lazy Class</xsl:template>

<!-- Degenerate Class
* Many degenerate methods
  |degenerate| / |methods| > 1⁄2
-->
<xsl:template match="src:class[

  (src:degenerate() div src:methods()) &gt; (1 div 2)

]" mode="class_stereotype">Degenerate Class</xsl:template>

<!-- Data Class
* Only the simple accessor/mutators get and set are present:
  |get| + |set| ≠ 0
* |methods| – (|get| + |set|) = 0
-->
<xsl:template match="src:class[

  (src:count('get') + src:count('set') &gt; 0)

    and

  ((src:methods() - (src:count('get') + src:count('set'))) = 0)

]" mode="class_stereotype">Data Class</xsl:template>

<!-- Small Class

* Number of methods in a class is less than 3:
  |methods| < 3
-->
<xsl:template match="src:class[

  src:methods() &lt; 3

]" mode="class_stereotype">Small Class</xsl:template>

<!-- Data Provider

* It consists mostly of accessors
  |accessors| > 2 · |mutators|
* Low control of other classes
  |accessors| > 2 (|controller| + |factory|)
-->
<xsl:template match="src:class[

  (src:accessor() &gt; (2 * src:mutator()))

    and

  (src:accessor() &gt; (2 * (src:count('controller') + src:count('factor'))))

]" mode="class_stereotype">Data Provider&#xa;&#xa;  Mainly accessor methods</xsl:template>

<!-- Unclassified
-->
<xsl:template match="src:class" mode="class_stereotype">Unclassified</xsl:template>

  <!--
    Union two lists together.
  -->
  <func:function name="src:union">
    <xsl:param name="first"/>
    <xsl:param name="second"/>

    <func:result select="$first | $second"/>
  </func:function>

<!-- Key for all comments with sterotypes 
     Value is prefixed with id of class, i.e., <id>-<stereotype>, e.g., isdmj1093u0-get
     Value is also id of class, i.e., <id>
-->
<xsl:key name="expslist" match="src:class/src:block/src:comment[contains(., '@stereotype')]"
  use="src:union(str:split(str:replace(concat(' ', normalize-space(substring-before(substring-after(., '@stereotype'),'*/'))), ' ', concat(' ', generate-id(current()/../..), '-'))), exsl:node-set(generate-id(current()/../..)))"/>

<func:function name="src:methods">
  <func:result select="count(key('expslist', generate-id(current()))[not(contains(., '@stereotype constructor')) and not(contains(., '@stereotype copy-constructor'))])"/>
</func:function>

<!-- Count of a particular stereotype for the current class -->
<func:function name="src:count">
  <xsl:param name="s" />

  <func:result select="count(key('expslist', concat(generate-id(current()), '-', $s)))"/>

</func:function>

<func:function name="src:accessor">
  <func:result select="src:count('get') + src:count('nonconstget') + src:count('predicate') + src:count('property') + src:count('void-accessor')"/>
</func:function>

<func:function name="src:mutator">
  <func:result select="src:count('set') + src:count('command') + src:count('non-void-command')"/>
</func:function>

<func:function name="src:collaborational2">
  <func:result select="src:count('collaborator')"/>
</func:function>

<func:function name="src:collaborational">
  <func:result select="src:count('controller')"/>
</func:function>

<func:function name="src:creational">
  <func:result select="src:count('constructor') + src:count('copy-constructor') + src:count('factory')"/>
</func:function>

<func:function name="src:degenerate">
  <func:result select="src:count('stateless') + src:count('incidental') + src:count('empty')"/>
</func:function>

<xsl:template match="src:class" mode="table">

  <xsl:variable name="class" select="."/>

  <!-- Stereotype comments -->
  <xsl:variable name="sstereotype_count" select="count(key('expslist', generate-id(.)))"/>

  <xsl:variable name="stereotype_list">
  <stereotype category="accessor" name="get" count="{src:count('get')}"/>
  <stereotype category="accessor" name="nonconstget" count="{src:count('nonconstget')}"/>
  <stereotype category="accessor" name="predicate" count="{src:count('predicate')}"/>
  <stereotype category="accessor" name="property" count="{src:count('property')}"/>
  <stereotype category="accessor" name="void-accessor" count="{src:count('void-accessor')}"/>
  <stereotype category="mutator" name="set" count="{src:count('set')}"/>
  <stereotype category="mutator" name="command" count="{src:count('command')}"/>
  <stereotype category="mutator" name="non-void-command" count="{src:count('non-void-command')}"/>

  <stereotype category="collaborational2" name="collaborator" count="{src:count('collaborator')}"/>
  <stereotype category="collaborational" name="controller" count="{src:count('controller')}"/>
  <stereotype category="creational" name="constructor" count="{src:count('constructor')}"/>
  <stereotype category="creational" name="copy-constructor" count="{src:count('copy-constructor')}"/>
  <stereotype category="creational" name="factory" count="{src:count('factory')}"/>
  <stereotype category="degenerate" name="stateless" count="{src:count('stateless')}"/>
  <stereotype category="degenerate" name="incidental" count="{src:count('incidental')}"/>
  <stereotype category="degenerate" name="empty" count="{src:count('empty')}"/>
  </xsl:variable>

  <xsl:variable name="stereotype_category_list">
  <category display="Accessor" name="accessor" count="{src:accessor()}"/>
  <category display="Mutator" name="mutator" count="{src:mutator()}"/>
  <category display="Collaborational" name="collaborational" count="{src:collaborational()}"/>
  <category display="Collaborational" name="collaborational2" count="{src:collaborational2()}"/>
  <category display="Creational" name="creational" count="{src:creational()}"/>
  <category display="Degenerate" name="degenerate" count="{src:degenerate()}"/>
  </xsl:variable>

  <!-- insert class stereotype -->
  @class-stereotype <xsl:apply-templates select="current()" mode="class_stereotype"/><xsl:text>&#xa;&#xa;</xsl:text>

  <!-- list of method stereotype categories, listed in descending order, with counts and percentages -->
  <xsl:text>  Primary Method Stereotypes&#xa;</xsl:text>
  <xsl:for-each select="exsl:node-set($stereotype_category_list)/src:category[not(@name='collaborational2')]">
    <xsl:sort select="@count" data-type="number" order="descending"/>
    <xsl:if test="@count != 0">
    <xsl:text>  ------------------------&#xa;</xsl:text>
    <xsl:text>  </xsl:text><xsl:value-of select="str:align(concat(@display, ':'), '                   ')"/><xsl:value-of select="str:align(@count, '   ', 'right')"/><xsl:text>, </xsl:text><xsl:value-of select="str:align(round(100 * @count div $sstereotype_count), '  ', 'right')"/><xsl:text>%&#xa;</xsl:text>

      <!-- list of method stereotypes, listed in descending order, with counts and percentages -->
      <xsl:for-each select="exsl:node-set($stereotype_list)/src:stereotype[@category=current()/@name and not(@name='collaborator')]">
        <xsl:sort select="@count" data-type="number" order="descending"/>
        <xsl:if test="@count != 0">
        <xsl:text>    </xsl:text><xsl:copy-of select="str:align(concat(@name, ':'),'                 ')"/><xsl:value-of select="str:align(@count, '   ', 'right')"/><xsl:text>, </xsl:text><xsl:value-of select="str:align(round(100 * @count div $sstereotype_count), '  ', 'right')"/><xsl:text>%&#xa;</xsl:text>
        </xsl:if>
      </xsl:for-each>

    </xsl:if>
  </xsl:for-each>

  <xsl:text>&#xa;  Secondary Method Stereotypes&#xa;</xsl:text>
  <xsl:for-each select="exsl:node-set($stereotype_category_list)/src:category[@name='collaborational2']">
    <xsl:sort select="@count" data-type="number" order="descending"/>
    <xsl:if test="@count != 0">
    <xsl:text>  ------------------------&#xa;</xsl:text>
    <xsl:text>  </xsl:text><xsl:value-of select="str:align(concat('Collaborational', ':'), '                   ')"/><xsl:value-of select="str:align(@count, '   ', 'right')"/><xsl:text>, </xsl:text><xsl:value-of select="str:align(round(100 * @count div $sstereotype_count), '  ', 'right')"/><xsl:text>%&#xa;</xsl:text>

      <!-- list of method stereotypes, listed in descending order, with counts and percentages -->
      <xsl:for-each select="exsl:node-set($stereotype_list)/src:stereotype[@category=current()/@name and @name='collaborator']">
        <xsl:sort select="@count" data-type="number" order="descending"/>
        <xsl:if test="@count != 0">
        <xsl:text>    </xsl:text><xsl:copy-of select="str:align(concat(@name, ':'),'                 ')"/><xsl:value-of select="str:align(@count, '   ', 'right')"/><xsl:text>, </xsl:text><xsl:value-of select="str:align(round(100 * @count div $sstereotype_count), '  ', 'right')"/><xsl:text>%&#xa;</xsl:text>
        </xsl:if>
      </xsl:for-each>

    </xsl:if>
  </xsl:for-each>

  <xsl:if test="current()[not(ancestor::src:class)]">
  <xsl:apply-templates select="current()" mode="viz">
    <xsl:with-param name="stereotype_category_list" select="$stereotype_category_list"/>
    <xsl:with-param name="stereotype_list" select="$stereotype_list"/>
  </xsl:apply-templates>
  </xsl:if>

</xsl:template>

<!-- Existing class comment -->
<xsl:template match="src:comment[@type='block'][following-sibling::*[1][self::src:class]]">

  <xsl:variable name="class" select="following-sibling::*[1]"/>

  <!-- copy start and contents of existing comment -->
  <xsl:element name="comment" namespace="http://www.srcML.org/srcML/src">
    <xsl:copy-of select="@*"/>

  <xsl:value-of select="substring-before(., '*/')"/>

  <!-- class stereotype and tables -->
  <xsl:apply-templates select="$class" mode="table"/>

  <!-- end the comment with the last line -->
  <!--
  <xsl:value-of select="str:split(., '&#xa;')[last()]"/></xsl:element>
-->
  <!-- TODO: Fit all whitespace on last line here, so if */ is indented originally,
      it would be here -->
  <xsl:text>*/</xsl:text></xsl:element>

</xsl:template>

<!-- No existing class comment -->
<xsl:template match="src:class[not(preceding-sibling::*[1][self::src:comment][@type='block'])]">

  <xsl:variable name="class" select="."/>

    <!-- insert new stereotype comment -->
  <comment type="block"><!--<xsl:value-of select="src:last_ws(.)"/>--><xsl:text>/*</xsl:text>

    <!-- class stereotype and tables -->
    <xsl:variable name="inset">
      <xsl:apply-templates select="$class" mode="table"/>
    </xsl:variable>

    <xsl:copy-of select="str:replace($inset, '&#xa;', '&#xa;    ')"/>

   <xsl:text> */</xsl:text>
 </comment>
   <xsl:text>&#xa;</xsl:text>

  <!-- insert a copy of the indentation for the function, since we stole it for the inserted comment, must repeat it -->
  <xsl:value-of select="str:split(preceding-sibling::text()[1], '&#xa;')[last()]"/>

  <!-- copy existing class -->
  <xsl:copy-of select="."/>


</xsl:template>

<xsl:template match="src:class" mode="viz">
  <xsl:param name="stereotype_category_list"/>
  <xsl:param name="stereotype_list"/>

  <!-- @todo These are NOT being detected by the stereotype_base -->

  <!-- Generate document category.csv for the stereotype category list and counts -->
  <exsl:document href="category.csv" method="text">

    <xsl:text>Class</xsl:text>
    <xsl:for-each select="exsl:node-set($stereotype_category_list)/src:category[not(@name='collaborational2')]">
<!--      <xsl:sort select="src:count" data-type="number" order="descending"/> -->
      <xsl:if test="@count != 0">
      <xsl:text>,</xsl:text><xsl:value-of select="@display"/>
      </xsl:if>
    </xsl:for-each>
    <xsl:text>&#xa;</xsl:text>

    <xsl:value-of select="current()/src:name"/>
    <xsl:for-each select="exsl:node-set($stereotype_category_list)/src:category[not(@name='collaborational2')]">
<!--      <xsl:sort select="src:count" data-type="number" order="descending"/> -->
      <xsl:if test="@count != 0">
      <xsl:text>,</xsl:text><xsl:value-of select="@count"/>
      </xsl:if>
    </xsl:for-each>
    <xsl:text>&#xa;</xsl:text>

  </exsl:document>

  <!-- Generate document stereotype.csv for the stereotype list and counts -->
  <exsl:document href="stereotype.csv" method="text">

    <xsl:text>Class</xsl:text>
    <xsl:for-each select="exsl:node-set($stereotype_list)/src:stereotype[not(@name='collaborator')]">
<!--      <xsl:sort select="src:count" data-type="number" order="descending"/> -->
      <xsl:if test="@count != 0">
      <xsl:text>,</xsl:text><xsl:value-of select="@name"/>
      </xsl:if>
    </xsl:for-each>
    <xsl:text>&#xa;</xsl:text>

    <xsl:value-of select="current()/src:name"/>
    <xsl:for-each select="exsl:node-set($stereotype_list)/src:stereotype[not(@name='collaborator')]">
<!--      <xsl:sort select="src:count" data-type="number" order="descending"/> -->
      <xsl:if test="@count != 0">
      <xsl:text>,</xsl:text><xsl:value-of select="@count"/>
      </xsl:if>
    </xsl:for-each>
    <xsl:text>&#xa;</xsl:text>

  </exsl:document>

</xsl:template>

</xsl:stylesheet>