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

using namespace std;

AddressDB::AddressDB(const string& filename_arg)
  : filename(filename_arg)
{
  // Open the address db file and lock it exclusively.

  fd = open(filename.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd < 0)
    throw system_error(string("Can't open address db '") +
                      filename + "' for reading");
  fd_sentry sentry(fd);

  struct flock lock;
  lock.l_type = F_WRLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start  = 0;
  lock.l_len    = 0;
  if (fcntl(fd, F_SETLKW, &lock) != 0)
    throw system_error(string("Can't lock file '") + filename + "'");

  // Read the file into memory.

  char buffer[1024];
  ssize_t rc;
  for (rc = read(fd, buffer, sizeof(buffer));
       rc > 0;
       rc = read(fd, buffer, sizeof(buffer)))
    data.append(buffer, rc);
  if (rc < 0)
    throw system_error(string("Failed to read address db '") +
                      filename + "' into memory");

  // Success. Don't close the file descriptor.

  sentry.fd = -1;
}

AddressDB::~AddressDB() throw()
{
  close(fd);
}

bool AddressDB::find(const string& key) const
{
  if (key.empty())
    throw invalid_argument("AddressDB::find() called with empty string.");

  // Find the string in the buffer. Then check whether it begins at
  // the beginning of a line and whether it ends at the end of a
  // line. If it does not, it's not a valid hit.

  string::size_type pos = 0;
  do
  {
    pos = data.find(key, pos);
    if (pos != string::npos)
    {
      if ((pos == 0 || data[pos-1] == '\n') &&
         ((pos + key.size() == data.size()) || (data[pos+key.size()] == '\n')))
      {
        return true;
      }
      else
        ++pos;
    }
  }
  while (pos != string::npos);
  return false;
}

void AddressDB::insert(const string& key)
{
  if (key.empty())
    throw invalid_argument("AddressDB::find() may not be called with an empty string.");

  // Append the address to our buffer and then write it to disk
  // immediately. If any of these operations fail, the memory buffer
  // might be in an undefined state.

  string::size_type end_pos = data.size();

  if (data.size() > 0 && data[data.size()-1] != '\n')
    data.append("\n");
  data.append(key).append("\n");

  for (size_t len = end_pos; len < data.size(); )
  {
    ssize_t rc = write(fd, data.data()+len, data.size()-len);
    if (rc < 0)
      throw system_error(string("Failed writing to the address db '") + filename + "'");
    else
      len += rc;
  }
}