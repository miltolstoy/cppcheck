/*
 * Cppcheck - A tool for static C/C++ code analysis
 * Copyright (C) 2007-2009 Daniel Marjamäki, Reijo Tomperi, Nicolas Le Cam,
 * Leandro Penz, Kimmo Varis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */


#include "../src/tokenize.h"
#include "../src/checkdangerousfunctions.h"
#include "testsuite.h"

#include <sstream>

extern std::ostringstream errout;

class TestDangerousFunctions : public TestFixture
{
public:
    TestDangerousFunctions() : TestFixture("TestDangerousFunctions")
    { }

private:



    void check(const char code[])
    {
        // Tokenize..
        Tokenizer tokenizer;
        std::istringstream istr(code);
        tokenizer.tokenize(istr, "test.cpp");
        tokenizer.simplifyTokenList();

        // Assign variable ids
        tokenizer.setVarId();

        // Fill function list
        tokenizer.fillFunctionList();

        // Clear the error buffer..
        errout.str("");

        // Check for buffer overruns..
        Settings settings;
        settings._showAll = true;
        CheckDangerousFunctionsClass checkDangerousFunctions(&tokenizer, settings, this);
        checkDangerousFunctions.dangerousFunctions();
    }

    void run()
    {
        TEST_CASE(testmktemp);
        TEST_CASE(testgets);
        TEST_CASE(testscanf);
    }



    void testmktemp()
    {
        check("void f()\n"
              "{\n"
              "    char *x = mktemp(\"/tmp/zxcv\");\n"
              "}\n");
        ASSERT_EQUALS(std::string("[test.cpp:3]: Found 'mktemp'. You should use 'mkstemp' instead\n"), errout.str());
    }

    void testgets()
    {
        check("void f()\n"
              "{\n"
              "    char *x = gets();\n"
              "}\n");
        ASSERT_EQUALS(std::string("[test.cpp:3]: Found 'gets'. You should use 'fgets' instead\n"), errout.str());
    }

    void testscanf()
    {
        check("void f()\n"
              "{\n"
              "    char *x;\n"
              "    scanf(\"%s\", x);\n"
              "}\n");
        ASSERT_EQUALS(std::string("[test.cpp:4]: Found 'scanf'. You should use 'fgets' instead\n"), errout.str());
    }

};

REGISTER_TEST(TestDangerousFunctions)


