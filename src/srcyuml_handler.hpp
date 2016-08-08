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

#include <srcSAXSingleEventDispatcher.hpp>
#include <ClassPolicySingleEvent.hpp>


#include <srcyuml_class.hpp>

#include <iostream>
#include <iomanip>
#include <algorithm>

/**
 * srcyuml_handler
 *
 * Base class that provides hooks for processing.
 */
class srcyuml_handler : public srcSAXEventDispatch::PolicyListener {

private:

    std::ostream & out;
    std::vector<srcyuml_class> classes;
    
public:

    srcyuml_handler(const std::string & input_str, std::ostream & out) : out(out) {

        srcSAXController controller(input_str);
        run(controller);

    }

    srcyuml_handler(const char * input_filename, std::ostream & out) : out(out) {

        srcSAXController controller(input_filename);
        run(controller);

    }

    ~srcyuml_handler() {}

    void run(srcSAXController & controller) {

        ClassPolicy class_policy{this};
        srcSAXEventDispatch::srcSAXSingleEventDispatcher<ClassPolicy> handler { &class_policy };
        controller.parse(&handler);
        process_data();
        output_yuml();

    }

    void process_data() {

        for(srcyuml_class & aclass : classes)
            aclass.analyze_data();


    }

    void output_yuml() {

        for(const srcyuml_class & aclass : classes) 
            out << aclass;

    }

    virtual void Notify(const srcSAXEventDispatch::PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {

        if(typeid(ClassPolicy) == typeid(*policy)) {

            classes.emplace_back(policy->Data<ClassPolicy::ClassData>());

        }

    }

private:

};

#endif
