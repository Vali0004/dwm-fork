/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#include "util.h"

void
die(const char *fmt, ...)
{
	va_list ap;
	int saved_errno;

	saved_errno = errno;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	if (fmt[0] && fmt[strlen(fmt)-1] == ':')
		fprintf(stderr, " %s", strerror(saved_errno));
	fputc('\n', stderr);

	exit(1);
}

void *
ecalloc(size_t nmemb, size_t size)
{
	void *p;

	if (!(p = calloc(nmemb, size)))
		die("calloc:");
	return p;
}

int
normalizepath(const char *path, char **normal)
{
  size_t len = strlen(path);
  *normal = (char *)malloc((len + 1) * sizeof(char));
  const char *walk = path;
  const char *match;
  size_t newlen = 0;

  while ((match = strchr(walk, '/'))) {
    // Copy everything between match and walk
    strncpy(*normal + newlen, walk, match - walk);
    newlen += match - walk;
    walk += match - walk;

    // Skip all repeating slashes
    while (*walk == '/')
      walk++;

    // If not last character in path
    if (walk != path + len)
      (*normal)[newlen++] = '/';
  }

  (*normal)[newlen++] = '\0';

  // Copy remaining path
  strcat(*normal, walk);
  newlen += strlen(walk);

  *normal = (char *)realloc(*normal, newlen * sizeof(char));

  return 0;
}

int
parentdir(const char *path, char **parent)
{
  char *normal;
  char *walk;

  normalizepath(path, &normal);

  // Pointer to last '/'
  if (!(walk = strrchr(normal, '/'))) {
    free(normal);
    return -1;
  }

  // Get path up to last '/'
  size_t len = walk - normal;
  *parent = (char *)malloc((len + 1) * sizeof(char));

  // Copy path up to last '/'
  strncpy(*parent, normal, len);
  // Add null char
  (*parent)[len] = '\0';

  free(normal);

  return 0;
}

int
mkdirp(const char *path)
{
  char *normal;
  char *walk;
  size_t normallen;

  normalizepath(path, &normal);
  normallen = strlen(normal);
  walk = normal;

  while (walk < normal + normallen + 1) {
    // Get length from walk to next /
    size_t n = strcspn(walk, "/");

    // Skip path /
    if (n == 0) {
      walk++;
      continue;
    }

    // Length of current path segment
    size_t curpathlen = walk - normal + n;
    char curpath[curpathlen + 1];
    struct stat s;

    // Copy path segment to stat
    strncpy(curpath, normal, curpathlen);
    strcpy(curpath + curpathlen, "");
    int res = stat(curpath, &s);

    if (res < 0) {
      if (errno == ENOENT) {
        DEBUG("Making directory %s\n", curpath);
        if (mkdir(curpath, 0700) < 0) {
          fprintf(stderr, "Failed to make directory %s\n", curpath);
          perror("");
          free(normal);
          return -1;
        }
      } else {
        fprintf(stderr, "Error statting directory %s\n", curpath);
        perror("");
        free(normal);
        return -1;
      }
    }

    // Continue to next path segment
    walk += n;
  }

  free(normal);

  return 0;
}

int
nullterminate(char **str, size_t *len)
{
  if ((*str)[*len - 1] == '\0')
    return 0;

  (*len)++;
  *str = (char*)realloc(*str, *len * sizeof(char));
  (*str)[*len - 1] = '\0';

  return 0;
}