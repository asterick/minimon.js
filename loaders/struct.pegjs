Body
	= _ body:Entities
    	{ return body }

Entities
	= body:Entity*
    	{ return body.filter(v => v) }

Entity
	= Comment
    	{ return null; }
	/ Union
    / Struct
    / Integer
    / Instance

Union
	= "union" WB name:Identifier? fields:Scope instance:Identifier? ";" _
		{ return { type: "Union", name, fields, instance } }

Struct
	= "struct" WB name:Identifier? fields:Scope instance:Identifier? ";" _
		{ return { type: "Struct", name, fields, instance } }

Instance
	= type:Identifier name:Identifier ";" _
    	{ return { type: "Instance", name } }

Integer
	= "int" size:Number "_t" WB _ name:Identifier ";" _
    	{ return { type:"Integer", align: true, signed: true, size, name } }
	/ "uint" size:Number "_t" WB _ name:Identifier ";" _
    	{ return { type:"Integer", align: true, signed: false, size, name } }
    / ("signed" WB)? "int" WB name:Identifier _ ":" _ size:Number ";" _
    	{ return { type:"Integer", align: false, signed: true, size, name } }
    / "unsigned" WB "int" WB name:Identifier _ ":" _ size:Number ";" _
    	{ return { type:"Integer", align: false, signed: false, size, name } }

Scope
	= "{" _ l:Entities "}" _
    	{ return l }

Number
	= v:$[0-9]+
		{ return parseInt(v, 10) }

Identifier
	= !Reserved v:$([a-z_]i [a-z0-9_]i*) _
    	{ return v }

Reserved
	= ("union" / "struct") WB

Comment
	= "#" (![\n\r] .)* _
    / "/*" (!"*/" .)* "*/" _
    / "//" (!"\n" .)* _

WB
	= !WC _

WC
	= [0-9a-z_]i

_ "whitespace"
	= [ \t\n\r]*

