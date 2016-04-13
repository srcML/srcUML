grammar srcYUML2graphViz;

yuml
	: line+ EOF
	;

line
	: (node | relationship)
	;

relationship
	: (node (aggregation | composition | realization | generalization)  node)
	;

node
	: '['  ']'
	;

aggregation
	: ('+->')
	;

composition
	: ('++->')
	;

realization
	: ('^-.-')
 	;
 
 generalization
 	: ('^-')
 	; 
