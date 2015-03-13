/**
 * @file srcyuml.hpp
 *
 * @copyright Copyright (C) 2015  SDML (www.srcML.org)
 *
 * srcYUML is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * srcYUML is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

 /*

  Count each the occurrences of each srcML element.

  Input: input_file.xml
  Useage: srcyuml input_file.xml
  
  */

#include <srcYUMLHandler.hpp>
#include <srcSAXController.hpp>

#include <map>
#include <iostream>

/**
 * main
 * @param argc number of arguments
 * @param argv the provided arguments (array of C strings)
 * 
 * Invoke srcSAX handler to generate yUML.
 */
int main(int argc, char * argv[]) {

  if(argc < 2) {

    std::cerr << "Useage: srcyuml input_file.xml\n";
    exit(1);

  }

  srcSAXController control(argv[1]);
  srcYUMLHandler handler;
  control.parse(&handler);

  return 0;
}
