/**
 * @file srcyuml.hpp
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

 /*

  Count each the occurrences of each srcML element.

  Input: input_file.xml
  Useage: srcyuml input_file.xml
  
  */

#include <srcyuml_handler.hpp>

#include <iostream>
#include <fstream>

/**
 * main
 * @param argc number of arguments
 * @param argv the provided arguments (array of C strings)
 * 
 * Invoke srcSAX handler to generate yUML.
 */
int main(int argc, char * argv[]) {

  if(argc < 2) {

    std::cerr << "Usage: srcyuml input_file.xml [output_file]\n";
    exit(1);

  }

  std::ostream * out = &std::cout;
  
  if(argc > 2)
    out = new std::ofstream(argv[2]);

  srcyuml_handler handler(argv[1], *out);

  if(argc > 2)
    delete out;

  return 0;
}
