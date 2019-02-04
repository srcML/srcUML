<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:src="http://www.srcML.org/srcML/src" 
    xmlns:cpp="http://www.srcML.org/srcML/cpp"
    xmlns:set="http://exslt.org/sets"
    xmlns:exsl="http://exslt.org/common"
    xmlns:str="http://exslt.org/strings"
    xmlns:regexp="http://exslt.org/regular-expressions"
    xmlns:func="http://exslt.org/functions"
    xmlns:dyn="http://exslt.org/dynamic"
    xmlns:src_old="http://www.sdml.info/srcML/src"
    extension-element-prefixes="exsl str func regexp"
    exclude-result-prefixes="set src dyn src_old"
    version="1.0">
<!-- 
@file stereotype.xsl

@copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)

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

To identify the stereotype Accessor::Get the following conditions need to be satisfied: 
  • method is const 
  • returns a data member 
  • return type is primitive or container of a primitives 

To identify the stereotype Accessor::Predicate the following conditions need to be satisfied: 
  • method is const 
  • returns a Boolean value that is not a data member 

To identify the stereotype Accessor::Property the following conditions need to be satisfied: 
  • method is const 
  • does not return a data member 
  • return type is primitive or container of primitives 
  • return type is not Boolean 

To identify the stereotype Mutator::Set the following conditions need to be satisfied: 
  • method is not const 
  • return type is void or Boolean 
  • only one data member is changed 

To identify the stereotype Mutator::Command the following conditions need to be satisfied: 
  • method is not const 
  • return type is void or Boolean 
  • complex change to the object’s state is performed 
  e.g., more than one data member was changed 

To identify the stereotype Collaborator one of the following statements needs to be satisfied: 
  • returns void and at least one of the method’s 
    parameters or local variables is an object 
  • returns a parameter or local variable that is an 
    object 

To identify the stereotype Creator::Factory the following conditions need to be satisfied: 
  • returns an object created in the method’s body
     -->

  <!--
    Global/Constant Variables
  -->
  <!-- current encoding (XSLT cannot obtain internally) -->
  <xsl:param name="encoding" select="ISO-8859-1"/>
  <xsl:param name="processing_mode">ReDocSrc</xsl:param>
  <xsl:param name="more_namespaces"></xsl:param>
  <xsl:param name="more_native"></xsl:param>
  <xsl:param name="more_modifiers"></xsl:param>
  <xsl:param name="more_ignorable_calls"></xsl:param>

  <!-- <xsl:param name="" select=""/> -->
  <!-- provide identity transformation -->
  <xsl:output method="xml" encoding="UTF-8" standalone="yes"/>

<!--
    Stereotype matching function
    Classifies stereotypes using criteria on function definition

  -->
  <xsl:template match="src:function" mode="stereotype">
    <xsl:apply-templates select="." mode="get"/>
    <xsl:apply-templates select="." mode="nonconstget"/>
    <xsl:apply-templates select="." mode="predicate"/>
    <xsl:apply-templates select="." mode="property"/>
    <xsl:apply-templates select="." mode="voidaccessor"/>
    <xsl:apply-templates select="." mode="set"/>
    <xsl:apply-templates select="." mode="command"/>
    <xsl:apply-templates select="." mode="non-void-command"/>
    <xsl:apply-templates select="." mode="collaborational-predicate"/>
    <xsl:apply-templates select="." mode="collaborational-property"/>
    <xsl:apply-templates select="." mode="collaborational-voidaccessor"/>
    <xsl:apply-templates select="." mode="collaborational-command"/>
    <xsl:apply-templates select="." mode="factory"/>
    <xsl:apply-templates select="." mode="stateless"/>
    <xsl:apply-templates select="." mode="incidental"/>
    <xsl:apply-templates select="." mode="empty"/>
    <xsl:apply-templates select="." mode="collaborator"/>

  </xsl:template>

  <!-- constructor's are matched differently than functions
	   Each constructor specialization is a different pattern
	   of the same mode. Whichever matches the best wins.
	-->
  <xsl:template match="src:constructor[true()]" mode="stereotype">
    <xsl:apply-templates select="." mode="mode-constructor"/>
  </xsl:template>

  <!-- Accessors -->

  <!-- stereotype get 

       method is const

       return type is not void

       contains at least one return statement which is:

              a single variable
        pointer to a variable
              has no calls
              variable is a data member

      in other words, it is of the form:

                 return n;
                 return *n;

      where the n is a data member.
  -->
  
  <xsl:template match="src:function[

    not(src:type/src:name='void') and

    src:const() and

    src:return()[
      (src:name and not(*[2]) or
      *[1][self::src:operator='*'] and *[2][self::src:name] and not(*[3])) and

      src:primary_variable_name(src:name)[src:is_data_member()]][1]

  ]" mode="get"><xsl:call-template name="method_get"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="get"/>

  <xsl:template match="src:function[

    not(src:type/src:name='void') and

    not(src:const()) and

    not(src:data_member_is_written()) and 

    src:return()[
      (src:name and not(*[2]) or
      *[1][self::src:operator='*'] and *[2][self::src:name] and not(*[3])) and

      src:primary_variable_name(src:name)[src:is_data_member() and not(.='this')]][1]

    ]" mode="nonconstget"><xsl:call-template name="method_nonconstget"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="nonconstget"/>

  <!-- stereotype predicate 

       method is const

       return type includes bool
       
       data members are used or there is a pure call or call on data members

       at least one return expression contains:

        false
        true
        no variable, or more then one variable
        call
        variable plus an operators
        one of the variables is not a data member
  -->
  <xsl:template match="src:function[

    (src:type/src:name[.='boolean' or .='Boolean']) and
         
    src:cppconst() and

    (src:data_members() or
                
      (src:real_call()[src:is_pure_call() and 
           
      (not(src:name/src:operator='.') or src:name/src:name[1]=src:class_name()) or
                       
      src:calling_object()[src:is_data_member()]])
    ) and 
     
    src:return()[

      .='false' or .='true' or

      src:call[1] or

      count(src:name)!=1 or

      *[2][self::src:operator] or

      src:primary_variable_name(src:name)[src:is_declared()]][1]

    ]" mode="predicate"><xsl:call-template name="method_predicate"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="predicate"/>

  <!-- stereotype property

       method is const

       return type is not void or bool
       
       data members are used or data or there is a pure call or call on data members 
       
       return expression contains one of the following:

           more then one variable, or no variables
     a call
     single variable with an operator
     single variable that is not a data member
  -->
  <xsl:template match="src:function[

    not(src:type/src:name[.='void' or .='boolean' or .='Boolean']) and
           
    src:cppconst() and
           
    (src:data_members() or
           
      (src:real_call()[src:is_pure_call() and 
      
      (not(src:name/src:operator='.') or src:name/src:name[1]=src:class_name()) or
                   
      src:calling_object()[src:is_data_member()]])
    ) and 
       
    src:return() and
       
    not(src:return()[
      not(
        *[2][self::src:operator] or

        src:call[1] or

        count(src:name)!=1 or

        src:primary_variable_name(src:name)[src:is_declared()]
       )
     ][1])

  ]" mode="property"><xsl:call-template name="method_property"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="property"/>

  <!-- stereotype voidaccessor 

       specifier is const
       
       data members are used

       return is void (??? void * allowed ???)
  -->
  <xsl:template match="src:function[

    src:type/src:name='void' and 

    src:cppconst() and 
       
    (src:data_members() or
              
      (src:real_call()[src:is_pure_call() and 

      (not(src:name/src:operator='.') or src:name/src:name[1]=src:class_name()) or
         
      src:calling_object()[src:is_data_member()]])) 
  ]
         " mode="voidaccessor"><xsl:call-template name="method_voidaccessor"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="voidaccessor"/>


  <!-- Mutators -->

  <!-- stereotype set 

       method is not const

       return type is void or bool, or return the object (for chaining), i.e., 'return *this'

       number of real calls in expression statements is at most 1

       number of data members written to in expression statements is 1
  -->
  <xsl:template match="src:function[

    (src:type/src:name[.='void' or .='boolean' or .='Boolean'] or

    not(src:const()) and

    not(src:return()[not(count(*)=1 and *[1][self::src:name='this'])]))

    and

    not(src:real_call()[2]) and

    count(src:data_members_write())=1

  ]" mode="set"><xsl:call-template name="method_set"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="set"/>

  <!-- stereotype command

       method is not const

       return type contains void or bool

       for expression statements at least one of the following holds:

           more then one data member is written to

           exactly one data member is written to and the number of calls
     in expression statements or returns is at least 2

           no data members are written to and their is a call not
     in a throw statement that is a simple real call (not a constructor call)
     or a complex call for a data member

           Note:  A set is formed by src:union with the written data members and the
     src:type.  This way, the predicate is always evaluated, even if there are
     no data members written.  The actual number of data members written is one
     less then last().  So, "last()=1 and ..." is evaluated when there are no
     data members written.
  -->
  <xsl:template match="src:function[

    src:type/src:name[.='void' or .='boolean' or .='Boolean'] and

    not(src:const()) and

    src:union(src:data_members_write(), src:type)[

      last()&gt;2 or last()=2 and src:real_call()[2] or last()=1 and

      src:real_call()[
        src:is_pure_call() and

        not(src:is_static()) or src:calling_object()[src:is_data_member()][1]
      ][1]
    ][1]

  ]" mode="command"><xsl:call-template name="method_command"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="command"/>

  <!-- stereotype non-void-command

       method is not const

       return type is not void or bool (??? void* ???)

       for expression statements at least one of the following holds:

           more then one data member is written to

           exactly one data member is written to and the number of real calls
     is at least 2

           no data members are written to and their is a call not
     in a throw statement that is a simple real call (not a constructor call from new)
     or a complex call for a data member

           Note:  A set is formed by src:union with the written data members and the
     src:type.  This way, the predicate is always evaluated, even if there are
     no data members written.  The actual number of data members written is one
     less then last().  So, "last()=1 and ..." is evaluated when there are no
     data members written.
  -->
  <xsl:template match="src:function[

    not(src:type/src:name[.='void' or .='boolean' or .='Boolean']) and

    not(src:const()) and

    src:union(src:data_members_write(), src:type)[

      last()&gt;2 or last()=2 and src:real_call()[2] or last()=1 and

      src:real_call()[src:is_pure_call() and
   
      not(src:is_static()) or src:calling_object()[src:is_data_member()][1]][1]][1]

  ]" mode="non-void-command"><xsl:call-template name="method_non-void-command"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="non-void-command"/>


  <!-- Controllers -->


  <!-- stereotype collaborational-predicate 

       method is const

       return type includes bool
       
       data members are not used
       
       no pure calls, a() a::b(); no calls on data members 

       at least one return expression contains:

        false
        true
        no variable, or more then one variable
        call
        variable plus an operators
        one of the variables is not a data member
  -->
  <xsl:template match="src:function[

    src:type/src:name[.='boolean' or .='Boolean'] and

    src:cppconst() and
         
      not(src:data_members()) and
         
      not(
        src:real_call()[src:is_pure_call() and 
         
        (not(src:name/src:operator='.') or src:name/src:name[1]=src:class_name()) or
         
        src:calling_object()[src:is_data_member()]]) and 
     
    src:return()[

      .='false' or .='true' or

      src:call[1] or

      count(src:name)!=1 or

      *[2][self::src:operator] or

      src:primary_variable_name(src:name)[src:is_declared()]][1]

    ]" mode="collaborational-predicate"><xsl:call-template name="method_collaborational-predicate"/></xsl:template>

  <xsl:template match="src:function" mode="collaborational-predicate"/>

  <!-- stereotype collaborational-property

       method is const

       return type is not void or bool
       
       data members are not used
       
       no pure calls, a() a::b(); no calls on data members 

       return expression contains one of the following:

           more then one variable, or no variables
     a call
     single variable with an operator
     single variable that is not a data member
  -->
  <xsl:template match="src:function[

    not(src:type/src:name[.='void' or .='boolean' or .='Boolean']) and

    src:cppconst() and
     
    not(src:data_members()) and

    not(
      src:real_call()[src:is_pure_call() and 

      (not(src:name/src:operator='.') or src:name/src:name[1]=src:class_name()) or

      src:calling_object()[src:is_data_member()]]
    ) and 

    src:return()[1] and
     
      not(src:return()[
        not(
          *[2][self::src:operator] or

          src:call[1] or

          count(src:name)!=1 or

          src:primary_variable_name(src:name)[src:is_declared()])
    ][1]

    )]" mode="collaborational-property"><xsl:call-template name="method_collaborational-property"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="collaborational-property"/>


  <!-- stereotype collaborational-voidaccessor 

       specifier is const
       
       data members are not used
       
       no pure calls, a() a::b(); no calls on data members 

       return is void (??? void * allowed ???)
  -->
  <xsl:template match="src:function[

    src:type/src:name='void' and

    src:cppconst() and 

    not(src:data_members()) and

    not(
      src:real_call()[src:is_pure_call() and 

      (not(src:name/src:operator='.') or src:name/src:name[1]=src:class_name()) or

      src:calling_object()[src:is_data_member()]]) 

  ]" mode="collaborational-voidaccessor"><xsl:call-template name="method_collaborational-voidaccessor"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="collaborational-voidaccessor"/>


  <!-- stereotype collaborational-command

       method is not const

       no data members are written

       (
       (one or more calls:

           no pure calls, a() a::b()

           no calls on data members)

       or parameter or local variable is written
       )

      Calls allowed:  f->g() where f is not a data member, new f() (which isn't a real call)
  --> 
  <xsl:template match="src:function[

    not(src:const()) and

    not(src:data_members_write()) and

    not(
        src:real_call()[src:is_pure_call() and 

        (not(src:name/src:operator='.') or src:name/src:name[1]=src:class_name()) or

        src:calling_object()[src:is_data_member()]]) and 

    (src:real_call()

      or

      src:expr_name()[src:is_written() and src:primary_variable_name(.)[not(src:is_data_member())]] or

      src:block//src:decl[src:type/src:name[src:is_object()]][src:init])

  ]" mode="collaborational-command"><xsl:call-template name="method_collaborational-command"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="collaborational-command"/>

  <!-- stereotype collaborator 

       A type name is an object, but not of this class
  -->                                                                                 
  <xsl:template match="src:function[

    src:all_type_names_nonclass_object(.//src:type/src:name, src:class_name())[1]

  ]" mode="collaborator"><xsl:call-template name="method_collaborator"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="collaborator"/>

  <!-- Factory -->

  <!-- stereotype factory
`
       return type includes pointer to object

       a return statement includes a new operator, or a variable which is a parameter or a local variable
  -->
  <xsl:template match="src:function[

    src:type[src:modifier='*' and src:name[src:is_object()]] and
          
    src:return()[

      src:operator='new' or

      src:primary_variable_name(src:name)[src:is_declared()]][1]

  ]" mode="factory"><xsl:call-template name="method_factory"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="factory"/>


  <!-- Degenerate  -->    

  <!-- stereotype stateless
       
       includes at least one non-comment statement

       one real call (including new calls)

       no data members used (except for read on 'this')
  -->
  <xsl:template match="src:function[

    src:block/*[not(self::src:comment)][1] and

    count(src:stateless_real_call())=1 and

    not(src:data_members()[not(.='this' and not(src:is_written()))])

  ]" mode="stateless"><xsl:call-template name="method_stateless"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="stateless"/>

  <!-- stereotype incidental
       
       includes at least one non-comment statement

       no real calls (including new calls)

       no data members used
          not(src:stateless_real_call()) and
  -->
  <xsl:template match="src:function[

    src:block/*[not(self::src:comment)][1] and

    (count(src:block/src:return) + count(src:block/src:throw) + count(src:block/src:throws) + count(src:block/src:expr_stmt[.//src:expr/src:call/src:name[.='assert' or .='assertEquals']]))= count(src:block/*[not(self::src:comment)]) and

    not(src:block/src:return//src:name) and 

    not(src:data_members())

  ]" mode="incidental"><xsl:call-template name="method_incidental"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="incidental"/>

  <!-- stereotype empty

       no statements, except for comments
  -->
  <xsl:template match="src:function[

     not(src:block/*[not(self::src:comment)][1])

  ]" mode="empty"><xsl:call-template name="method_empty"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <xsl:template match="src:function" mode="empty"/>

  <!-- stereotype constructor

       default
  -->
  <xsl:template match="src:constructor" mode="mode-constructor"><xsl:call-template name="method_constructor"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <!-- stereotype copy-constructor

       single parameter
       name same as class name
  -->
  <xsl:template match="src:constructor[

  	src:parameter_list/src:parameter[last()=1]/src:decl/src:type/src:name = current()/src:name

  ]" mode="mode-constructor"><xsl:call-template name="method_copy-constructor"><xsl:with-param name="function" select="."/></xsl:call-template></xsl:template>

  <!-- Supporting functions -->

  <xsl:variable name="namespaces" select="str:split($more_namespaces)"/>

  <!--
      Definition of native types.  Additional types can be declared in
      the variable more_types
  -->
  <!--
  <xsl:variable name="native" select="str:split(concat('int long double float string char unsigned signed wchar_t char16_t char32_t bool vector list map ', $more_native))"/> -->

  <!-- Java -->
  <xsl:variable name="native" select="str:split(concat('byte char short int long float double boolean Boolean void null String StringBuffer map List Set', $more_native))"/>

  <!--
      Definition of names that occur in types, but are modifiers, etc.
      More types can be declared in the variable more_modifiers
  -->
  <xsl:variable name="modifiers" select="str:split(concat('std void emit virtual inline static const ', $more_modifiers))"/>

  <!--
      Definition of calls that aren't really calls, e.g., dynamic_cast.
      More types can be declared in the variable more_modifiers
  -->
  <xsl:variable name="ignorable_calls" select="str:split(concat('assert static_cast const_cast dynamic_cast reinterpret_cast assertEquals clone equals finalize getClass hashCode notify notifyAll toString wait', $more_ignorable_calls))"/>

  <!--
    Functions - For stereotype matching.
  -->

  <!--
    pure virtual function declaration
  -->
  <func:function name="src:isvirtual">
    <xsl:param name="function"/>

    <func:result select="normalize-space($function/src:specifier[last()])='= 0'"/>
  </func:function>

  <!--
    locate all top-level function
  -->
  <func:function name="src:function">

    <func:result select="ancestor-or-self::src:function[1]"/>

  </func:function>

  <!--
    return the name of a class to a function/method.
  -->
  <func:function name="src:class_name">

    <!-- Java -->
    <func:result select="ancestor::src:class[1]/src:name[1]"/>

  </func:function>

  <!--
    Locate all return expressions within a function.
  -->
  <func:function name="src:return">

    <func:result select="key('return', generate-id(src:function()))/src:expr"/>

<!--    <func:result select="src:function()/src:block/descendant::src:return/src:expr"/>-->

  </func:function>

  <!--
    Get the names of all parameters.
  -->

  <func:function name="src:param_name">

    <func:result select="src:final_name(key('param', generate-id(src:function()))/src:name)"/>

  </func:function>

  <!--
    Identifiers in parameter types
  -->
  <func:function name="src:param_type_name">

    <func:result select="src:all_type_names(key('param', generate-id(src:function()))/src:type/src:name)"/>

  </func:function>

  <!--
    Identifiers in declaration types
  -->
  <func:function name="src:decl_type_name">

    <func:result select="src:all_type_names(key('decl', generate-id(src:function()))[parent::src:decl_stmt]/src:type/src:name)"/>

  </func:function>

  <!-- variable names in declarations (not including parameters)  -->
  <func:function name="src:decl_name">

    <func:result select="src:final_name(key('decl', generate-id(src:function()))/src:name)"/>

  </func:function>

  <!-- 
    Check for the type of a variable.
  -->
  <func:function name="src:variable_type">
     <xsl:param name="context"/>

    <func:result select="key('decl', generate-id(src:function()))[src:name=$context]/src:type"/>

  </func:function>

  <!--
    Returns the last name in a list of names, that isn't a nested list of names.
  -->
  <func:function name="src:final_name">
    <xsl:param name="cur"/>

    <func:result select="$cur/self::src:name[not(src:name)] | $cur/self::src:name/src:name[1]"/>

  </func:function>

  <!--
    The object being used to make the call.
  -->
  <func:function name="src:calling_object">
    <xsl:param name="context" select="."/>

    <func:result select="src:final_name(self::*[not(src:is_pure_call())]/preceding-sibling::*[2][self::src:name])"/>

  </func:function>

  <!--
    Check if a data member is being written into.
  -->
  <func:function name="src:data_members_write">

    <func:result select="src:expr_stmt_name()[src:is_written() and src:is_data_member()]"/>

  </func:function>

<!--
  Test to see if a variable is a data member of a class.
-->

<xsl:key name="return" match="src:return" use="generate-id(ancestor::src:function[1])"/>

<xsl:key name="param" match="src:function/src:parameter_list/src:parameter/src:decl" use="generate-id(ancestor::src:function[1])"/>

<xsl:key name="call" match="src:function/src:block/descendant::src:expr/src:call" use="generate-id(ancestor::src:function[1])"/>

<xsl:key name="decl" match="src:function/src:block/descendant::src:decl" use="generate-id(ancestor::src:function[1])"/>

<xsl:key name="expr_name" match="src:function/src:block/descendant::src:expr/src:name" use="generate-id(ancestor::src:function[1])"/>

<xsl:key name="data_members" match="src:function/src:block/descendant::src:expr/src:name[src:is_data_member()]" use="generate-id(ancestor::src:function[1])"/>

<xsl:key name="expr_stmt_name" match="src:function/src:block/descendant::src:expr_stmt/src:expr/src:name" use="generate-id(ancestor::src:function[1])"/>

<!--
  All data members in the current function
-->
<func:function name="src:data_members">

  <func:result select="key('data_members', generate-id(src:function()))"/>

</func:function>

<!--
  All data members in the current function
-->
<func:function name="src:data_member_is_written">

  <func:result select="src:data_members()[src:is_written()]"/>

</func:function>

  <!--
    Check if something is static.
  -->
  <func:function name="src:is_static">

    <func:result select="src:name/src:name[1] and src:name/src:name[1]!=src:class_name()"/>

  </func:function>


  <!--
    Check if a function is a data member.
  -->
  <func:function name="src:is_data_member">

    <func:result select="not(.=src:param_name() or .=src:decl_name() or
             (.//src:operator='.' and not(./src:name[1]=src:class_name())))"/>

  </func:function>

  <!--
    Check if a variable is declared as part of a method.
  -->
  <func:function name="src:is_declared">

    <func:result select=".=src:param_name() or .=src:decl_name()"/>

  </func:function>

  <!--
    Check if a variable is declared as part of a method.
  -->
  <func:function name="src:const">

    <func:result select="src:specifier='const' or not(src:data_member_is_written())"/>

  </func:function>

  <func:function name="src:cppconst">

    <func:result select="src:const()"/>

  </func:function>

  <!--
    Variable name in expression (including return expressions).
  -->
  <func:function name="src:expr_name">

    <func:result select="key('expr_name', generate-id(src:function()))"/>

  </func:function>

<!--
  Test to see if a variable is a data member of a class.
-->

  <!--
    Variable name in expression statements
  -->
  <func:function name="src:expr_stmt_name">

    <func:result select="src:final_name(key('expr_stmt_name', generate-id(src:function())))"/>

  </func:function>

  <!--
    Variable name in expression statements.
  -->
  <func:function name="src:call">

    <func:result select="key('call', generate-id(src:function()))"/>

  </func:function>

  <!--
    Make sure that the call is not a call to a constructor.
  -->
  <func:function name="src:base_call">
    <xsl:param name="nodes"/>

    <func:result select="$nodes/src:call[
             not(preceding-sibling::*[1][self::src:operator='new']) and
             not((src:name | src:name/src:name[1])[.=$ignorable_calls]) and
             not(ancestor::src:throw) and
             not(ancestor::src:throws) and
             not((src:name | src:name/src:name[1])[src:is_native_type()])
  ]"/>

  </func:function>

  <func:function name="src:real_call">

    <func:result select="src:base_call(src:function()/src:block/descendant::src:expr)"/>

  </func:function>

  <!-- 
    Check if the call is a stateless one.
  -->
  <func:function name="src:stateless_real_call">

    <func:result select="key('call', generate-id(src:function()))[
             not((src:name | src:name/src:name[1])[.=$ignorable_calls]) and
             not(ancestor::src:throw) and
             not(ancestor::src:throws) and
             not((src:name | src:name/src:name[1])[src:is_native_type()])
  ]"/>

  </func:function>

  <!--
      Primary variable name.

      Either the direct name, or for a complex name with multiple subnames, the last one.

      E.g.,

          a  -> a
          b::a -> a
          c::b::a -> a
  -->
  <func:function name="src:primary_variable_name">
     <xsl:param name="context"/>

     <func:result select="$context[not(src:name)] | $context/src:name[last()]"/>

  </func:function>

  <!--
      All names in a type

      Includes a direct name, or for a complex name, multiple subnames.
      E.g.,

          a  -> a
          b::a -> b
    std::a -> a
    a<b> -> {a, b}
    a<b<c> > -> {a, b, c}
  -->
  <func:function name="src:all_type_names">
     <xsl:param name="context"/>

     <func:result select="$context[not(src:name) and not(.='std')] |
        $context//src:name[not(src:name) and not(.='std') and
        not(preceding-sibling::src:name[1][not(.='std')])]"/>

  </func:function>

  <!-- 
    A derivative of all_type_names not sure what this does.
  -->
  <func:function name="src:all_type_names_nonclass_object">
     <xsl:param name="context"/>
     <xsl:param name="class"/>

     <func:result select="$context[not(src:name)][not(.='std') and not(.=$class) and src:is_object()] |
        $context//src:name[not(src:name) and not(.='std') and
        not(preceding-sibling::src:name[1][not(.='std')]) and not(.=$class) and src:is_object()]"/>

  </func:function>


  <!--
    Type name is a native type
  -->
  <func:function name="src:is_native_type">

     <func:result select=".=$native"/>

  </func:function>

  <!-- 
    Not sure if this is necessary anymore because modifier has been migrated into
    the src namespace.
  -->
<!--   <func:function name="src:is_modifier">

     <func:result select=".=$modifiers"/>

  </func:function> -->


  <!-- 
    Not sure what this means exactly.
  -->
  <func:function name="src:is_pure_call">

     <func:result select="not(preceding-sibling::*[1][self::src:operator='.' or self::src:operator='-&gt;'])"/>

  </func:function>

  <!--  -->
  <func:function name="src:is_object">

     <func:result select="not(.=$modifiers or .=$native)"/>

  </func:function>

  <!-- -->
  <func:function name="src:is_written">

    <!-- @todo Why is both << and >> used? -->
    <func:result select="following-sibling::*[1][contains(self::src:operator, '=') or
             self::src:operator='&lt;&lt;' or
             self::src:operator='&gt;&gt;'
             ]"/>

  </func:function>

  <!--
    Union two lists together.
  -->
  <func:function name="src:union">
    <xsl:param name="first"/>
    <xsl:param name="second"/>

    <func:result select="$first | $second"/>

  </func:function>


  <!--
    Used for calculating whitespace indentation level.
  -->
  <func:function name="src:last_ws">
    <xsl:param name="s"/>

    <xsl:choose>
      <xsl:when test="contains($s, '&#xa;')">
        <func:result select="src:last_ws(substring-after($s, '&#xa;'))"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="raw" select="substring-before($s, '*')"/>
        <xsl:choose>
          <xsl:when test="string-length(normalize-space($raw)) = 0">
            <func:result select="$raw"/>
          </xsl:when>
          <xsl:otherwise>
            <func:result select="''"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </func:function>

  
  <xsl:template match="/src_old:unit">
    <xsl:message terminate="yes">ERROR: Newer version of srcML required.</xsl:message>
  </xsl:template>

  <xsl:template match="src:unit[number(concat(str:split(@revision, '.'),'')) >= 95]">
    <xsl:message terminate="yes">ERROR: Newer version of srcML required.</xsl:message>
  </xsl:template>

  <!-- copy any element which does not contain a function -->
  <xsl:template match="@*|node()">
    <xsl:copy-of select="."/>
  </xsl:template>

  <!-- default function copy -->
  <xsl:template match="*[.//src:function or .//src:constructor]">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
    </xsl:copy>
  </xsl:template>

</xsl:stylesheet>