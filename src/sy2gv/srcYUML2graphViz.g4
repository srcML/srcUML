/**
 * @file srcYUMLgraphViz.g4
 *
 * @copyright Copyright (C) 2015-2016 srcML, LLC. (www.srcML.org)
 *
 * This file is part of srcYUML.
 *
 * srcYUML is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * srcYUML is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with srcYUML.  If not, see <http://www.gnu.org/licenses/>.
 */

grammar srcYUML2graphViz;

yuml
	: ( classDef | relationship | NEWLINE )+ EOF
	;

classDef
	: '[' classID ( '|' variables ('|' methods )? )? ']'
	;

relationship
	: classDef ( aggregation | composition | realization | generalization ) classDef 
	;

aggregation
	: relationText '+' relationText '-' relationText '>' relationText
	;

composition
	: relationText '+' relationText '+' relationText '-' relationText '>'
	;

realization
	: relationText '^' relationText '-' relationText '.' relationText '-'
 	;
 
generalization
	: relationText '^' relationText '-' relationText
 	; 

classID
	: text
	;

variables
	: vmText
	;

methods
	: vmText
	;

text
	: ( LETTER | NUMBER | UNICODE | ('\t') | ('\r') | ('-') | ('+') | ('#') | ('<') | ('>') | '(' | ')' | ';' | ':' | ' ' | '*' | '［' | '］' | '«' | '»' | ('،') )*
	;

vmText
	: ( LETTER | NUMBER | UNICODE | ('\t') | ('\r') | ('-') | ('+') | ('#') | ('<') | ('>') | '(' | ')' | ':' | ';' | ' ' | '*' | '［' | '］' | '«' | '»' | ('،') )*
	;

relationText
	: ( LETTER | NUMBER | UNICODE | ('\t') | ('\r') | ('#') | '(' | ')' | ':' | ' ' | '*' | '［' | '］' | '«' | '»' | ('،') )*
	;

LETTER
	: [a-zA-Z]
 	;

NUMBER
	: [0-9]
 	;

UNICODE
	:[\u0021-\u002A\u002C\u002E-\u003A\u003D\u003F-\u007A\u007E-\uFFFF]
	;

NEWLINE
	: ( '\n' )
	;
