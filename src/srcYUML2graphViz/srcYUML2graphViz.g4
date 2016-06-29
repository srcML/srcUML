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
	: (node | relationship | NEWLINE)+ EOF
	;

relationship
	: (node (aggregation | composition | realization | generalization)  node)
	;

node
	: '[' text ']'
	;

aggregation
	: text '+' text '-' text '>' text
	;

composition
	: text '+' text '+' text '-' text '>'
	;

realization
	: text '^' text '-' text '.' text '-'
 	;
 
 generalization
 	: text '^' text '-' text
 	; 

 text
 	: (LETTER | NUMBER | ('|') | ('-') | ('+') | ('#') | ('<') | ('>') | ';' | '(' | ')' | ':' | ' ' | '*')*
 	;

 LETTER
 	: [a-zA-Z]
 	;

 NUMBER
 	: [0-9]
 	;

NEWLINE
	: '\n'
	;