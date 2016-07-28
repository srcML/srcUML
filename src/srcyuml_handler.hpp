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

#include <ClassPolicy.hpp>

#include <iostream>

/**
 * srcyuml_handler
 *
 * Base class that provides hooks for processing.
 */
class srcyuml_handler : public srcSAXEventDispatch::PolicyListener {

private:

    std::ostream & out;
    std::vector<ClassPolicy::ClassData *> classes;
    

public:

    srcyuml_handler(const std::string & input_str, std::ostream & out) : out(out) {

        srcSAXController controller(input_str);
        run(controller);

    }

    srcyuml_handler(const char * input_filename, std::ostream & out) : out(out) {

        srcSAXController controller(input_filename);
        run(controller);
        output_yuml();

    }

    void run(srcSAXController & controller) {

        ClassPolicy class_policy{this};
        srcSAXEventDispatch::srcSAXEventDispatcher<ClassPolicy> handler { &class_policy };
        controller.parse(&handler);

    }

    void output_yuml() {

        for(ClassPolicy::ClassData * data : classes) 
            output_yuml_class(*data);

    }

    void output_yuml_class(const ClassPolicy::ClassData & data) {

        // out << '[';
        out << data.name;
        for(ClassPolicy::ParentData p_data : data.parents)
            out << '\t' << p_data.name << ": " << p_data.isVirtual << ',' << p_data.accessSpecifier << '\n';
        // out << "]\n";
        out << '\n';

    }

    virtual void Notify(const srcSAXEventDispatch::PolicyDispatcher * policy) override {

        if(typeid(ClassPolicy) == typeid(*policy)) {

            classes.emplace_back(policy->Data<ClassPolicy::ClassData>());

        }

    }

private:

};

#endif
