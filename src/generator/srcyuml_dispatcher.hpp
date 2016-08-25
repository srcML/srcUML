/**
 * @file srcyuml_dispatcher.hpp
 *
 * @copyright Copyright (C) 2016 srcML, LLC. (www.srcML.org)
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

#ifndef INCLUDED_SRCYUML_DISPATCHER_HPP
#define INCLUDED_SRCYUML_DISPATCHER_HPP

#include <srcSAXSingleEventDispatcher.hpp>

template <typename ...policies>
class srcyuml_dispatcher : public srcSAXEventDispatch::srcSAXSingleEventDispatcher<policies...> {

private:
    bool dispatched;

public:

   srcyuml_dispatcher(srcSAXEventDispatch::PolicyListener * listener)
        : srcSAXEventDispatch::srcSAXSingleEventDispatcher<policies...>(listener) {
       srcSAXEventDispatch::srcSAXEventDispatcher<policies...>::RemoveEvents({"if", "for", "while", "typedef", "call", "macro", "init", "expr_stmt", "expr", "member_list" });
   }

};


#endif
