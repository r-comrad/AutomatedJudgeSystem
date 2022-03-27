#ifndef FILES_MANAGER_HPP
#define FILES_MANAGER_HPP

#include <fstream>

#include "main/flags.hpp"

#include "domain/type.hpp"
#include "domain/my_string.hpp"
#include "domain/error_message.hpp"

void copyFile(str_const_ref aFromFileName, str_const_ref aToFileName);

#endif // !FILES_MANAGER_HPP