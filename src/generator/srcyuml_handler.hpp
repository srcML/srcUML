/**
 * @file srcyuml_handler.hpp
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

#ifndef INCLUDED_SRCYUML_HANDLER_HPP
#define INCLUDED_SRCYUML_HANDLER_HPP

#include <srcSAXEventDispatchUtilities.hpp>
#include <srcSAXController.hpp>

#include <srcyuml_dispatcher.hpp>
#include <ClassPolicySingleEvent.hpp>

#include <srcyuml_class.hpp>
#include <srcyuml_relationship.hpp>
#include <srcyuml_output.hpp>

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <map>

/**
 * srcyuml_handler
 *
 * Base class that provides hooks for processing.
 */
class srcyuml_handler : public srcSAXEventDispatch::PolicyListener {

private:

    std::vector<std::shared_ptr<srcyuml_class>> classes;

public:

    srcyuml_handler(const std::string & input_str, std::ostream & out) {

        srcyuml_output outputter(out);
        srcSAXController controller(input_str);
        run(controller, outputter);

    }

    srcyuml_handler(const char * input_filename, std::ostream & out) {

        srcyuml_output outputter(out);
        srcSAXController controller(input_filename);
        run(controller, outputter);

    }

    ~srcyuml_handler() {}

    void run(srcSAXController & controller, srcyuml_output & outputter) {

        srcyuml_dispatcher<ClassPolicy> dispatcher(this);
        controller.parse(&dispatcher);
        outputter.output(classes); // ouputs using the outputters function

    }

    virtual void Notify(const srcSAXEventDispatch::PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {

        if(typeid(ClassPolicy) == typeid(*policy)) {

            ClassPolicy::ClassData * class_data = policy->Data<ClassPolicy::ClassData>();
            if(class_data && class_data->name)
                classes.emplace_back(std::make_shared<srcyuml_class>(class_data));

        }

    }

private:

};

#endif
