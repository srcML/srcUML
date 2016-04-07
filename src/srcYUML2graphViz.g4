grammar srcYUML2graphViz;

yuml
	: line + EOF
	;

line
	: (class | relationship)
	;

relationship
	: (class + (aggregation | composition | realization | generalization) + class)
	;


class
	: 
	;

aggregation
	:
	;

composition
	:
	;

realization
	:
	;

generalization
	:
	;