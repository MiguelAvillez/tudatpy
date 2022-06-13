/*    Copyright (c) 2010-2020, Delft University of Technology
 *    All rights reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */


#ifndef TUDATPY_DEPRECATION_WARNINGS_H
#define TUDATPY_DEPRECATION_WARNINGS_H

#include <iostream>


namespace tudatpy {

void printDeprecationWarning(
        const std::string& oldName,
        const std::string& newName,
        const std::string& description = "" );

} // namespace tudatpy

#endif//TUDATPY_DEPRECATION_WARNINGS_H
