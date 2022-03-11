#ifndef FILES_MANAGER_HPP
#define FILES_MANAGER_HPP

#include <fstream>

#include "domain/domain.hpp"
#include "domain/my_strings.hpp"
#include "domain/errors.hpp"

void copyFile(std_string_ref aFromFileName, std_string_ref aToFileName);

#endif // !FILES_MANAGER_HPP