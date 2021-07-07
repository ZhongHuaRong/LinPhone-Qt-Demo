/***************************************************************************
* config.h.cmake
* Copyright (C) 2014  Belledonne Communications, Grenoble France
*
****************************************************************************
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
****************************************************************************/

/* #undef ENABLE_DEBUGGING */
/* #undef SRTP_KERNEL */
/* #undef SRTP_KERNEL_LINUX */
/* #undef DEV_URANDOM */
/* #undef GENERIC_AESICM */
/* #undef USE_SYSLOG */
#define ERR_REPORTING_STDOUT 1
/* #undef USE_ERR_REPORTING_FILE */
/* #undef ERR_REPORTING_FILE */
/* #undef SRTP_GDOI */

#define CPU_CISC 1
/* #undef CPU_RISC */
#define HAVE_X86 1
/* #undef WORDS_BIGENDIAN */

#define HAVE_STDLIB_H 1
/* #undef HAVE_UNISTD_H */
/* #undef HAVE_BYTESWAP_H */
#define HAVE_STDINT_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_SYS_TYPES_H 1
/* #undef HAVE_MACHINE_TYPES_H */
/* #undef HAVE_SYS_INT_TYPES_H */
/* #undef HAVE_SYS_SOCKET_H */
/* #undef HAVE_NETINET_IN_H */
/* #undef HAVE_ARPA_INET_H */
#define HAVE_WINDOWS_H 1
#define HAVE_WINSOCK2_H 1
/* #undef HAVE_SYSLOG_H */

/* #undef HAVE_INET_ATON */
/* #undef HAVE_USLEEP */
/* #undef HAVE_SIGACTION */

#define HAVE_UINT8_T 1
#define HAVE_UINT16_T 1
#define HAVE_UINT32_T 1
#define HAVE_UINT64_T 1
#define SIZEOF_UNSIGNED_LONG 4
#define SIZEOF_UNSIGNED_LONG_LONG 8
#define PACKAGE_STRING "libsrtp2 2.2.0-pre"
#define PACKAGE_VERSION "2.0.1-pre"

#ifndef __cplusplus
/* #undef inline */
#endif
