/**
 * @file abstract_srcuml_outputter.hpp
 *
 * @copyright Copyright (C) 2016 srcML, LLC. (www.srcML.org)
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

#ifndef INCLUDED_ABSTRACT_SRCUML_OUTPUTTER_HPP
#define INCLUDED_ABSTRACT_SRCUML_OUTPUTTER_HPP

#include <srcuml_class.hpp>
#include <srcuml_relationship.hpp>


class abstract_srcuml_outputter {

public:

	virtual bool output(std::ostream & out, std::vector<std::shared_ptr<srcuml_class>> & classes) = 0;

	virtual srcuml_relationships analyze_relationships(std::vector<std::shared_ptr<srcuml_class>> & classes) {

		return srcuml_relationships(classes);

	}

};

#endif