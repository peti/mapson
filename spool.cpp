/*
 * Copyright (c) 2001-2007 Peter Simons <simons@cryp.to>
 *
 * This software is provided 'as-is', without any express or
 * implied warranty. In no event will the authors be held liable
 * for any damages arising from the use of this software.
 *
 * Copying and distribution of this file, with or without
 * modification, are permitted in any medium without royalty
 * provided the copyright notice and this notice are preserved.
 */

#include "mapson.hpp"
#include "md5/md5.h"

using namespace std;

string spool(const string& mail)
{
  // Calculate the md5 hash of the mail.

  unsigned char sum[16];
  char buf[33];
  md5_buffer(mail.data(), mail.size(), sum);
  snprintf(buf, 33, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
          sum[0], sum[1], sum[2], sum[3], sum[4], sum[5], sum[6], sum[7],
          sum[8], sum[9], sum[10], sum[11], sum[12], sum[13], sum[14], sum[15]);

  // Make sure the spool directory exists.

  struct stat mystat;
  if (stat(config->spool_dir.c_str(), &mystat) == -1)
    if (mkdir(config->spool_dir.c_str(), S_IRUSR | S_IWUSR | S_IRWXU) == -1)
      throw runtime_error(string("Can't create spool directory '") + config->spool_dir + "'");

  // Open the spool file and write store the mail there.

  string filename = config->spool_dir + "/" + buf;
  info("Spooling e-mail '%s' as '%s'.", config->message_id.c_str(), filename.c_str());
  int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd < 0)
    throw system_error(string("Can't open spool file '") + filename + "' for writing");
  fd_sentry sentry(fd);
  for (size_t len = 0; len < mail.size(); )
  {
    ssize_t rc = write(fd, mail.data()+len, mail.size()-len);
    if (rc < 0)
      throw system_error(string("Failed writing to the spool file '") + filename + "'");
    else
      len += rc;
  }

  return buf;
}