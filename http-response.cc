/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * @file Implementation for HttpResponse class
 *
 * Skeleton for UCLA CS118 Spring quarter class
 */

#include "http-response.h"

#include <string> // C++ STL string
#include <string.h> // helpers to copy C-style strings

#include "compat.h"

using namespace std;

// comment the following line to disable debug output
#define _DEBUG 1 

#ifdef _DEBUG
#include <iostream>
#define TRACE(x) std::clog << x << endl;
#else
#endif // _DEBUG

#include <boost/lexical_cast.hpp>

HttpResponse::HttpResponse ()
{
}
  
const char*
HttpResponse::ParseResponse (const char *buffer, size_t size)
{
  const char *curPos = buffer;
  
  const char *endline = (const char *)memmem (curPos, size - (curPos-buffer), "\r\n", 2);
  if (endline == 0)
    {
      throw ParseException ("HTTP response doesn't end with \\r\\n");
    }

  string responseLine (curPos, endline-curPos);
  size_t posFirstSpace = responseLine.find (" ");
  size_t posSecondSpace = responseLine.find (" ", posFirstSpace+1);

  if (posFirstSpace == string::npos ||
      posSecondSpace == string::npos)
    {
      throw ParseException ("Incorrectly formatted response");
    }

  // 1. HTTP version
  if (responseLine.compare (0, 5, "HTTP/") != 0)
    {
      throw ParseException ("Incorrectly formatted HTTP response");
    }
  string version = responseLine.substr (5, posFirstSpace - 5);
  // TRACE (version);
  SetVersion (version);

  // 2. Response code
  string code = responseLine.substr (posFirstSpace + 1, posSecondSpace - posFirstSpace - 1);
  // TRACE (code);
  SetStatusCode (code);

  string msg = responseLine.substr (posSecondSpace + 1, responseLine.size () - posSecondSpace - 1);
  // TRACE (msg);
  SetStatusMsg (msg);

  curPos = endline + 2;
  return ParseHeaders (curPos, size - (curPos-buffer));
}


size_t
HttpResponse::GetTotalLength () const
{
  size_t len = 0;
  
  len += 5; // 'HTTP/'
  len += m_version.size (); // '1.0'
  len += 1 + m_statusCode.size (); // ' code'
  len += 1 + m_statusMsg.size (); // ' <msg>'
  len += 2; // '\r\n'

  len += HttpHeaders::GetTotalLength ();

  len += 2; // '\r\n'
  
  return len;
}

char*
HttpResponse::FormatResponse (char *buffer) const
{
  char *bufLastPos = buffer;
  
  bufLastPos = stpncpy (bufLastPos, "HTTP/", 5);
  bufLastPos = stpncpy (bufLastPos, m_version.c_str (), m_version.size ());
  bufLastPos = stpncpy (bufLastPos, " ", 1);
  bufLastPos = stpncpy (bufLastPos, m_statusCode.c_str (), m_statusCode.size ());
  bufLastPos = stpncpy (bufLastPos, " ", 1);
  bufLastPos = stpncpy (bufLastPos, m_statusMsg.c_str (), m_statusMsg.size ());
  bufLastPos = stpncpy (bufLastPos, "\r\n", 2);

  bufLastPos = HttpHeaders::FormatHeaders (bufLastPos);
  bufLastPos = stpncpy (bufLastPos, "\r\n", 2);

  return bufLastPos;
}


const std::string &
HttpResponse::GetVersion () const
{
  return m_version;
}

void
HttpResponse::SetVersion (const std::string &version)
{
  m_version = version;
}

const std::string &
HttpResponse::GetStatusCode () const
{
  return m_statusCode;
}

void
HttpResponse::SetStatusCode (const std::string &code)
{
  m_statusCode = code;
}

const std::string &
HttpResponse::GetStatusMsg () const
{
  return m_statusMsg;
}

void
HttpResponse::SetStatusMsg (const std::string &msg)
{
  m_statusMsg = msg;
}
