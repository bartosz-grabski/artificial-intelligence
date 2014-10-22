<?xml version="1.0"?>
<!--XSL by MacKo, 2004-->
<html xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xsl:version="1.0">
<head>
<title>Framsticks Development History &#8212; What's New?</title>
<style>
BODY {
	FONT-FAMILY: verdana, lucida, arial, helvetica, sans-serif; 
	FONT-SIZE: 80%;
	background-color:#eefaee;
	line-height: 150%;
}

pre, code {
-x-system-font:none;
background-color:white;
font-family:monospace;
font-size:115%;
font-size-adjust:none;
font-stretch:normal;
font-style:normal;
font-variant:normal;
font-weight:normal;
}

H1 { FONT-SIZE: 150%; }
H2 { FONT-SIZE: 130%; }
H3 { FONT-SIZE: 115%; }
H4 { FONT-SIZE: 100%; }

A:hover {color:#F49018;}

.nagztlem {
   font-size : 140%;
   font-weight:normal;
   background-color :#dcFfcc;
   text-align : left;
   color : #006644;
   margin-top: 2em;  
   padding-top: 0.1em;
   padding-bottom: 0.2em;

   border-top: 1px solid #006644;
   border-bottom: 1px solid #006644;
}
</style>
</head>
<body>
<h1><a href="http://www.framsticks.com/">Framsticks</a> Development History &#8212; What's New?</h1>
<xsl:for-each select="//release">


  <h2 class="nagztlem">Version <xsl:value-of select="@ver"/>, 
  <xsl:if test="@date = &quot;&quot;">to be</xsl:if>
  released
  <xsl:value-of select="@date"/></h2>
  
<!-- how to check for variables
<xsl:choose>
  <xsl:when test="string(@date)">
    the value of @date is: <xsl:value-of select="@date"/>
  </xsl:when>
  <xsl:when test="not(@date)">
    no @date found
  </xsl:when>
  <xsl:when test="not(string(@date))">
    empty string @date
  </xsl:when>
</xsl:choose>  
-->  
  
  <ul>
  
  <xsl:if test="@date = &quot;&quot;">
    [ Developmental versions (for advanced users) can be found <a href="http://www.framsticks.com/dev/files/" target="_blank">here</a> ]
    <div style="margin-top:1.5em" />
  </xsl:if>
  
  <xsl:for-each select="changes">
    <!--<p><xsl:value-of select="@in"/></p>-->
    <xsl:for-each select="change">
      <li>
      <xsl:if test="../@in != &quot;&quot;"><i><xsl:value-of select="../@in"/></i>: </xsl:if>
      <xsl:copy-of select="."/></li>
    </xsl:for-each>
    <div style="margin-top:1em" />
  </xsl:for-each>
  </ul>
</xsl:for-each>

<h3>Unofficial releases beginning late 1996
<br /><br />.
<br /><br />.
<br /><br />.
<br /><br />Big Bang
</h3>
</body>
</html>