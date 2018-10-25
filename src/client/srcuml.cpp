/**
 * @file srcuml.hpp
 *
 * @copyright Copyright (C) 2015-2016 srcML, LLC. (www.srcML.org)
 *
 * This file is part of srcUML.
 *
 * srcUML is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * srcUML is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with srcUML.  If not, see <http://www.gnu.org/licenses/>.
 */

 /*

  Count each the occurrences of each srcML element.

  Input: input_file.xml
  Useage: srcuml input_file.xml
  
  */

#include <srcuml_handler.hpp>
#include <boost/program_options.hpp>

#include <iostream>
#include <fstream>
#include <algorithm>

using namespace boost;
using namespace std;


/**
 * main
 * @param argc number of arguments
 * @param argv the provided arguments (array of C strings)
 * 
 * Invoke srcSAX handler to generate yUML.
 */
int main(int argc, char * argv[]) {

    //ostream * out = &cout;

    /*try {

        namespace po = boost::program_options;

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("output,o", po::value<string>(), "set output file")
            ("input", po::value<string>(), "input file")
        ;

        po::positional_options_description p;
        p.add("input", -1);

        po::variables_map vm;        
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        po::notify(vm);    

        if (vm.count("help")) {
            cout << desc << "\n";
            return 0;
        }

        if (vm.count("output")) {
          cout << "output\n";
            //string temp = vm["output"].as<string>();
            //cout << "Ouput file is: " << vm["output"].as<string>() << ".\n";
            //out = new std::ofstream(vm["output"].as<string>());
        }else{
            cout << "Ouput File was not set.\n";
        }

    }catch(std::exception& e){
        cerr << "error: " << e.what() << "\n";
        return 1;

    }catch(...) {
        cerr << "Exception of unknown type!\n";
    }*/

  if(argc < 2) {

    std::cerr << "Usage: srcuml input_file.xml [output_file]\n";
    exit(1);

  }

  std::ostream * out = &std::cout;
  
  
  if(argc > 2)
    out = new std::ofstream(argv[2]);

  srcuml_handler handler(argv[1], *out);

  if(argc > 2)
    delete out;

    return 0;
}
