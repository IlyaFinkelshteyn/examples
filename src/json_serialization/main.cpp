/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 - 2016 Axel Menzel <info@rttr.org>                     *
*                                                                                   *
*   This file is part of RTTR (Run Time Type Reflection)                            *
*   License: MIT License                                                            *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#include <cstdio>
#include <string>
#include <vector>
#include <array>

#include <iostream>
#include <rttr/registration>
#include "json_serialize.h"

using namespace rttr;

enum class color
{
    red,
    green,
    blue
};

struct point2d
{
    int x = 0;
    int y = 0;
    std::vector<int> array;
};

struct base
{
    base()
    {

    }

    bool b = true;
    color c = color::blue;
    point2d p;
    std::array<std::array<point2d, 4>, 2> array;


    RTTR_ENABLE()
};

struct derived : base
{
    derived() : array({{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}) {}
    double d = 0.0;
    uint64_t big_value = 0;
    std::string name = "";
    std::array<int, 10> array;

    int no_serialize = 100;

    RTTR_ENABLE(base)
};

RTTR_REGISTRATION
{
    rttr::registration::class_<base>("base")
        .property("b", &base::b)
        .property("color", &base::c)
        .property("array", &base::array)
        .property("point", &base::p);

    rttr::registration::class_<derived>("derived")
        .property("d", &derived::d)
        .property("big_value", &derived::big_value)
        .property("name", &derived::name)
        .property("array2", &derived::array)
        .property("no_serialize", &derived::no_serialize)
        (
            metadata("NO_SERIALIZE", true)
        )
        ;

    rttr::registration::class_<point2d>("point2d")
        .property("x", &point2d::x)
        .property("y", &point2d::y)
        .property("array", &point2d::array)
        ;


    rttr::registration::enumeration<color>("color")
        (
            value("red", color::red),
            value("blue", color::blue),
            value("green", color::green)
        );
}


int main(int argc, char** argv)
{
    derived d;
    base& b = d;

    type t = type::get(b);

    b.b = false;
    b.c = color::red;
    b.p.x= 42;
    b.p.y= 10;
    b.array[0][0].x = 100;
    b.array[1][3].x = 1230;
    b.array[1][3].array.resize(5);
    b.array[1][3].array[4] = 23;

    d.array[3] = 222;
    d.array = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};

    d.d = 23.42;
    d.big_value = 18e+14;
    d.name = "John Doe";

    auto json_text = io::to_json(b);
    std::cout << json_text;




    derived d2;
    io::from_json(json_text, d2);
    std::cout << "\n############################" << std::endl;
    std::cout << io::to_json(d2);
    return 0;
}

