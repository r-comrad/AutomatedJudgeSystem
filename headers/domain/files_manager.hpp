#ifndef FILES_MANAGER_HPP
#define FILES_MANAGER_HPP

#include <fstream>

#include "domain/domain.hpp"
#include "domain/my_strings.hpp"
#include "domain/errors.hpp"

void copyFile(str_const_ref aFromFileName, str_const_ref aToFileName);

#endif // !FILES_MANAGER_HPP