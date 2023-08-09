/**
 * @file pkgman_config.cs
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifdef __CYGWIN__
#  include <dlfcn.h>
#  include <dirent.h>
#  include <unistd.h>
#  include <cygwin/version.h>
#endif

// #include <sys/types.h>
// #include <sys/stat.h>




#if __has_include(<nettle/crypto.h>)
#  include <nettle/crypto.h>
#  define HAVE_LIBNETTLE 1
#elif __has_include(<openssl/crypto.h>)
#  include <openssl/crypto.h>
#  define HAVE_OPENSSL 1
#else
#  error "No crypto provider found!?"
#endif

#include "pkgman/pkgman_config.h"
#ifndef PKGMAN_CONFIGURATION_H
# error "No config header found?"
#endif

#if (HAVE_LIBNETTLE)
#  define CRYPTO_INCLUDE_SYMBOL __TO_STRING(<nettle/crypto.h>)
#elif (HAVE_OPENSSL)
#  define CRYPTO_INCLUDE_SYMBOL __TO_STRING(<openssl/crypto.h>)
#endif

#define CRYPTO_LIB __TO_STRING(libcrypto.dll)

#if HAVE_OPENSSL
#  define CRYPTO_H_PATH __TO_STRING(C:/msys64/usr/include/openssl)
#elif HAVE_LIBNETTLE
#  define CRYPTO_H_PATH __TO_STRING(C:/msys64/usr/include/nettle)
#endif


#if (__WIN32__) || (__CYGWIN__)
int WinMain()
{
#else
int main()
{
#endif

	printf("\n");
	printf("pkgman_config\n");

	#ifdef PKGMAN_VERSION
		printf("PKGMAN_VERSION: %d\n", PKGMAN_VERSION);
	#endif

#if defined(__CYGWIN__) && CYGWIN_VERSION_API_MINOR < 262
		void *libc = dlopen ("cygwin1.dll", 0);
		struct mntent *(*getmntent_r) (FILE *, struct mntent *, char *, int) = dlsym (libc, "getmntent_r");
#endif

	printf("\n");

	printf("Build information:\n\n");

	printf("prefix                  : %s\n", prefix);
  	printf("sysconfdir              : %s\n", SYSCONFDIR);
  	printf("conf file           	: %s/pacman.conf\n", SYSCONFDIR);
  	printf("localstatedir           : %s\n", LOCALSTATEDIR);
  	printf("database dir        	: %s/lib/pacman/\n", LOCALSTATEDIR);
  	printf("cache dir           	: %s/cache/pacman/pkg/\n", LOCALSTATEDIR);
  	printf("compiler                : @0@ @1@'.format(cc.get_id(), cc.version())\n");

	printf("\n");

	printf("Architecture            : @0@'.format(carch)\n");
  	printf("Host Type               : @0@'.format(chost)\n");
  	printf("File inode command      : @0@'.format(inodecmd)\n");
  	printf("File seccomp command    : @0@'.format(filecmd)\n");
  	printf("libalpm version         : @0@'.format(libalpm_version)\n");
    printf("pacman version          : @0@'.format(PACKAGE_VERSION)\n");

	printf("\n");

	printf("Directory and file information:\n\n");

	printf("root working directory  : %d\n", ROOTDIR);
  	printf("package extension       : @0@'.format(get_option('pkg-ext'))\n");
    printf("  source pkg extension  : @0@'.format(get_option('src-ext'))\n");
    printf("build script name       : @0@'.format(BUILDSCRIPT)\n");
    printf("template directory      : @0@'.format(get_option('makepkg-template-dir'))\n");

	printf("\n");

	printf("Compilation options:\n\n");

	printf("i18n support            : @0@'.format(get_option('i18n'))\n");
    printf("Build docs              : @0@'.format(build_doc)\n");
  	printf("debug build             : @0@'.format(get_option('buildtype') == 'debug')\n");
  	printf("Use libcurl             : @0@'.format(conf.get('HAVE_LIBCURL'))\n");
  	printf("Use GPGME               : @0@'.format(conf.get('HAVE_LIBGPGME'))\n");
  	printf("Use OpenSSL             : @0@'.format(conf.has('HAVE_LIBSSL') and conf.get('HAVE_LIBSSL') == 1)\n");
  	printf("Use nettle              : @0@'.format(conf.has('HAVE_LIBNETTLE') and conf.get('HAVE_LIBNETTLE') == 1)\n");

	printf("\n");

	printf("\n");
	printf("Checking for dependencies...\n");
	printf("\n");
/** Check for header path... this could be more thorough, of course... */

	DIR *dip;
    struct dirent *dit;
    struct stat lsbuf;
    char currentPath[__FILENAME_MAX__];

	const char* crypto_header_path = {CRYPTO_H_PATH};

    if((dip = opendir(CRYPTO_H_PATH)) == NULL)
    {
        perror("opendir()");
        return errno;
    }

    if((getcwd(currentPath, FILENAME_MAX)) == NULL)
    {
        perror("getcwd()");
        return errno;
    }

    /* stat the file - if it exists, do some checks */
		if(stat(currentPath, &lsbuf) == -1)
    {
        perror("stat()");
		return errno;

    } else {

		if(S_ISDIR(lsbuf.st_mode)) {

			printf("\"%s\" exists on this system.\n", CRYPTO_H_PATH);

		} else {

			printf("\"%s\" does not exist on this system.\n", CRYPTO_H_PATH);

			printf("\n");

			exit(EXIT_FAILURE);
		}

		printf("Included '%s' successfully.\n", CRYPTO_INCLUDE_SYMBOL);
	}


	/** Get the chosen crypto lib */
	void* handle;
	char *error;

	handle = dlopen(CRYPTO_LIB, RTLD_LAZY);
	error = dlerror();

	if (!handle || error != NULL)
	{
		fprintf(stderr, "Loading 'libcrypto.dll' attempt failed: %s\n", error);

		exit(EXIT_FAILURE);

	} else {

		printf("Loaded '%s' successfully.\n", CRYPTO_LIB);
	}

	dlclose(handle);

	printf("\n");

	printf("\n");
	printf("Compiler info:\n");
	printf("\n");

#ifdef PKGMAN_CLANG_VERSION
	printf("PKGMAN_CLANG_VERSION: %d\n", PKGMAN_CLANG_VERSION);
#endif

#ifdef PKGMAN_GCC_VERSION
	printf("PKGMAN_GCC_VERSION: %d\n", PKGMAN_GCC_VERSION);
#endif

#ifdef PKGMAN_ICC_VERSION
	printf("PKGMAN_ICC_VERSION: %d\n", PKGMAN_ICC_VERSION);
#endif

#ifdef PKGMAN_MSC_VERSION
	printf("PKGMAN_MSC_VERSION: %d\n", PKGMAN_MSC_VERSION);
#endif

	printf("\n");

	printf("\n");
	printf("Feature detection macros:\n");
	printf("\n");

#ifdef _POSIX_SOURCE
	printf("_POSIX_SOURCE defined\n");
#endif

#ifdef _POSIX_C_SOURCE
	printf("_POSIX_C_SOURCE defined: %jdL\n", (intmax_t) _POSIX_C_SOURCE);
#endif

#ifdef _ISOC99_SOURCE
	printf("_ISOC99_SOURCE defined\n");
#endif

#ifdef _ISOC11_SOURCE
	printf("_ISOC11_SOURCE defined\n");
#endif

#ifdef _XOPEN_SOURCE
	printf("_XOPEN_SOURCE defined: %d\n", _XOPEN_SOURCE);
#endif

#ifdef _XOPEN_SOURCE_EXTENDED
	printf("_XOPEN_SOURCE_EXTENDED defined\n");
#endif

#ifdef _LARGEFILE64_SOURCE
	printf("_LARGEFILE64_SOURCE defined\n");
#endif

#ifdef _FILE_OFFSET_BITS
	printf("_FILE_OFFSET_BITS defined: %d\n", _FILE_OFFSET_BITS);
#endif

#ifdef _TIME_BITS
	printf("_TIME_BITS defined: %d\n", _TIME_BITS);
#endif

#ifdef _BSD_SOURCE
	printf("_BSD_SOURCE defined\n");
#endif

#ifdef _SVID_SOURCE
	printf("_SVID_SOURCE defined\n");
#endif

#ifdef _DEFAULT_SOURCE
	printf("_DEFAULT_SOURCE defined\n");
#endif

#ifdef _ATFILE_SOURCE
	printf("_ATFILE_SOURCE defined\n");
#endif

#ifdef _GNU_SOURCE
	printf("_GNU_SOURCE defined\n");
#endif

#ifdef _REENTRANT
	printf("_REENTRANT defined\n");
#endif

#ifdef _THREAD_SAFE
	printf("_THREAD_SAFE defined\n");
#endif

#ifdef _FORTIFY_SOURCE
	printf("_FORTIFY_SOURCE defined\n");
#endif

	return(EXIT_SUCCESS);
}