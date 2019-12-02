# srcUML #
* A tool for efficiently reverse engineering accurate UML class diagrams.

### Cloning ###
* The project uses srcSAX and srcSAXEventDispatch as submodules.  The repo most be cloned with the `--recursive` option to clone the submodules.  The following is an example command:
```bash
git clone --recursive https://github.com/srcML/srcUML.git
```

### Build ###
* See BUILD.md

### Status ###
Highlights the current state of the program, including known bugs, potential future work, and library idiosyncrasies.

### Program Flow ###
The overall flow of the program is as shown in the image below
![](doc/images/srcuml_tool_chain.png)

### Creating a New Layout ###
- Creating a new layout for srcUML starts by creating a new class that inherits svg_outputter. The example below shows a standard template for a new layout.
```cpp
class svg_xyz_outputter : public svg_outputter {
    svg_xyz_outputter(bool method, bool attr){
        show_methods = method;
        show_attributes = attr;
    }

    bool output(…){
        init_standard(classes);

        //code for the new layout using OGDF methods
        //goes here.
	
	}
}
```
- In this case 'xyz' would represent the name of said layout algorithm. Notice that the layout is still based in an SVG output format. This is a coupling issue that could be a basis for future work. 
The key here is that the output function should do all the layout, classification, organization that you want. The output function has access to all the protected members
of the svg_outputter class. These protected members provide access to the nodes and edges of the graph which can then be manipulated to create the desired layout. How the layout is created
is up to the designer. 

- The parameters to the constructor. These represent the ability to turn attributes and methods on and off in the final diagram. They are NOT IMPLEMENTED. Some beginning code exists in
svg_printer.

- The svg_printer. This is a modified copy of the svg_printer from OGDF. It would be good to read through this quick as it represents the conversion from the UML model to the SVG format. 

- At the end of the output method, the below lines must be included.
```cpp
GraphIO::SVGSettings* svg_settings = new ogdf::GraphIO::SVGSettings();
		
if(!drawSVG(cga, out, *svg_settings, node_edge_arrow)){
	std::cout << "Error Write" << std::endl;
}

```
The first line creates a seetings object for the actuall svg printing mechanism. It allows for the setting of font size, family and color, as well as other parameters involving the whole of the diagram.
The if statement is then the actual print call. In this example it takes the cga which is the ClusterGraphAttributes object, however, if one is not dealing with clusters in their layout, you can provide ga,
the GraphAttributes object. out is the output stream to write the svg to, the settings are next and finally a node edge arrow object. This object describes the type of arrow to print between any node edge pair.
This is important as UML depends on the arrow heads to help identify relationship types. 

### Known Bugs ###

### Future Work ### 

### Quirks of OGDF ###

### Note ###
* The current build is dependent on files produced by srcML.


