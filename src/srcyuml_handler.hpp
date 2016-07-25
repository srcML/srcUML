/**
 * @file srcyuml_handler.hpp
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

#ifndef INCLUDED_SRCYUML_HANDLER_HPP
#define INCLUDED_SRCYUML_HANDLER_HPP

#include <srcSAXEventDispatchUtilities.hpp>
#include <srcSAXController.hpp>

#include <TypePolicy.hpp>
#include <ParamTypePolicy.hpp>
#include <FunctionCallPolicy.hpp>
#include <FunctionSignaturePolicy.hpp>

#include <iostream>

/**
 * srcyuml_handler
 *
 * Base class that provides hooks for processing.
 */
class srcyuml_handler : public srcSAXEventDispatch::Listener {

private:

    std::ostream & out;

    CallPolicy calldata;
    DeclTypePolicy typedata;
    ParamTypePolicy paramdata;
    std::unordered_map<srcSAXEventDispatch::ParserState, std::function<void(const srcSAXEventDispatch::srcSAXEventContext&)>, std::hash<int>> process_map, process_map2;

public:

    srcyuml_handler(const std::string & input_str, std::ostream & out) : out(out) {

        srcSAXController control(input_str);

        srcSAXEventDispatch::srcSAXEventDispatcher<srcyuml_handler, DeclTypePolicy, ParamTypePolicy, CallPolicy> handler { this, &typedata, &paramdata, &calldata };
        control.parse(&handler);

    }

    srcyuml_handler(const char * input_filename, std::ostream & out) : out(out) {

        srcSAXController control(input_filename);

        srcSAXEventDispatch::srcSAXEventDispatcher<srcyuml_handler, DeclTypePolicy, ParamTypePolicy, CallPolicy> handler { this, &typedata, &paramdata, &calldata };
        control.parse(&handler);

    }

private:

};

#endif
