/**
 * @file srcyuml_relationship.hpp
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

#ifndef INCLUDED_ABSTRACT_SRCUML_OUTPUTTER_HPP
#define INCLUDED_ABSTRACT_SRCUML_OUTPUTTER_HPP

#include <srcyuml_class.hpp>
#include <srcyuml_relationship.hpp>


class abstract_srcuml_outputter {

public:

	virtual bool output(std::ostream & out, std::vector<std::shared_ptr<srcyuml_class>> & classes) = 0;

	virtual srcyuml_relationships analyze_relationships(std::vector<std::shared_ptr<srcyuml_class>> & classes) {

		return srcyuml_relationships(classes);

	}

};

#endif