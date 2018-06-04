/*
 * utils_config.hpp
 *
 *  Created on: 24/09/2009
 *      Author: alanlohse
 */

#ifndef UTILS_CONFIG_HPP_
#define UTILS_CONFIG_HPP_

#include <cstdio>
#include <cstdlib>

#if defined(_WIN32) || defined(__WIN32) || defined(_WIN32_)
#  define UTILS_WINDOWS
#  if defined(USE_PTHREAD)
#    define PTHREAD_IMPL
#  else
#    define WINDOWS_THREAD_IMPL
#  endif
#endif

#if defined(_LINUX) || defined(__LINUX) || defined(_LINUX_)
# define UTILS_LINUX
# define PTHREAD_IMPL
#include "linux.inc"
#endif

#endif /* UTILS_CONFIG_HPP_ */
