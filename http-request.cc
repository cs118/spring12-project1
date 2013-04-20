/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * @file Implementation for HttpRequest class
 *
 * Skeleton for UCLA CS118 Spring quarter class
 */

#include "http-request.h"

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
#include <boost/tokenizer.hpp>

HttpRequest::HttpRequest ()
  : m_method (UNSUPPORTED)
  , m_port (0)
{
}
  
const char*
HttpRequest::ParseRequest (const char *buffer, size_t size)
{
  const char *curPos = buffer;
  
  const char *endline = (const char *)memmem (curPos, size - (curPos-buffer), "\r\n", 2);
  if (endline == 0)
    {
      throw ParseException ("HTTP Request doesn't end with \\r\\n");
    }

  boost::char_separator<char> sep(" ");
  string requestLine (curPos, endline-curPos);
  boost::tokenizer< boost::char_separator<char> > tokens (requestLine,
                                                          boost::char_separator<char> (" "));
  
  // 1. Request type
  boost::tokenizer< boost::char_separator<char> >::iterator token = tokens.begin ();
  if (token == tokens.end ())
    throw ParseException ("Incorrectly formatted request");

  // TRACE ("Token1: " << *token);
  if (*token != "GET")
    {
      throw ParseException ("Request is not GET");
    }
  SetMethod (GET);

  // 2. Request path
  ++ token;
  if (token == tokens.end ())
    throw ParseException ("Incorrectly formatted request");

  // TRACE ("Token2: " << *token);
  size_t pos = token->find ("://");
  if (pos == string::npos)
    {
      // just path
      SetPath (*token);
    }
  else
    {
      // combined Host and Path
      string protocol = token->substr (0, pos);
      // TRACE (protocol);
      
      pos += 3;
      size_t posSlash = token->find ("/", pos);
      if (posSlash == string::npos)
        throw ParseException ("Request line is not correctly formatted");

      // TRACE (string (curPos, endline-curPos));
      // TRACE (*token);
      // TRACE (pos << ", " << posSlash);
      
      size_t posPort = token->find (":", pos);
      if (posPort != string::npos && posPort < posSlash) // port is specified
        {
          string port = token->substr (posPort + 1, posSlash - posPort - 1);
          // TRACE (port);
          SetPort (boost::lexical_cast<unsigned short> (port));

          string host = token->substr (pos, posPort-pos);
          // TRACE (host);
          SetHost (host);
        }
      else
        {
          SetPort (80);
          
          string host = token->substr (pos, posSlash-pos);
          // TRACE (host);
          SetHost (host);
        }

      string path = token->substr (posSlash, token->size () - posSlash);
      // TRACE (path);
      SetPath (path);
    }

  // 3. Request version
  ++token;
  if (token == tokens.end ())
    throw ParseException ("Incorrectly formatted request");
  // TRACE ("Token3: " << *token);
  size_t posHTTP = token->find ("HTTP/");
  if (posHTTP == string::npos)
    {
      throw ParseException ("Incorrectly formatted HTTP request");
    }
  string version = token->substr (5, token->size () - 5);
  // TRACE (version);
  SetVersion (version);
  
  curPos = endline + 2;
  return ParseHeaders (curPos, size - (curPos-buffer));
}


size_t
HttpRequest::GetTotalLength () const
{
  if (m_method != GET)
    throw ParseException ("Only GET method is supported");
      
  size_t len = 4; // 'GET '
  len += m_path.size () + 1; // '<path> '
  len += 5; // 'HTTP/'
  len += m_version.size (); // '1.0'
  len += 2; // '\r\n'

  len += HttpHeaders::GetTotalLength ();

  len += 2; // '\r\n'
  
  return len;
}

char*
HttpRequest::FormatRequest (char *buffer) const
{
  if (m_method != GET)
    throw ParseException ("Only GET method is supported");

  char *bufLastPos = buffer;
  
  bufLastPos = stpncpy (bufLastPos, "GET ", 4);
  bufLastPos = stpncpy (bufLastPos, m_path.c_str (), m_path.size ());
  bufLastPos = stpncpy (bufLastPos, " HTTP/", 6);
  bufLastPos = stpncpy (bufLastPos, m_version.c_str (), m_version.size ());
  bufLastPos = stpncpy (bufLastPos, "\r\n", 2);

  bufLastPos = HttpHeaders::FormatHeaders (bufLastPos);
  bufLastPos = stpncpy (bufLastPos, "\r\n", 2);

  return bufLastPos;
}


HttpRequest::MethodEnum
HttpRequest::GetMethod () const
{
  return m_method;
}

void
HttpRequest::SetMethod (HttpRequest::MethodEnum method)
{
  m_method = method;
} 

// const std::string &
// HttpRequest::GetProtocol () const
// {
//   return m_protocol;
// }

// void
// HttpRequest::SetProtocol (const std::string &protocol)
// {
//   m_protocol = protocol;
// }

const std::string &
HttpRequest::GetHost () const
{
  return m_host;
}

void
HttpRequest::SetHost (const std::string &host)
{
  m_host = host;

  if (m_port != 80)
    {
      ModifyHeader ("Host", m_host + ":" + boost::lexical_cast<string> (m_port));
    }
  else
    ModifyHeader ("Host", m_host);
}

unsigned short
HttpRequest::GetPort () const
{
  return m_port;
}

void
HttpRequest::SetPort (unsigned short port)
{
  m_port = port;

  if (m_port != 80)
    {
      ModifyHeader ("Host", m_host + ":" + boost::lexical_cast<string> (m_port));
    }
  else
    ModifyHeader ("Host", m_host);
}

const std::string &
HttpRequest::GetPath () const
{
  return m_path;
}
  
void
HttpRequest::SetPath (const std::string &path)
{
  m_path = path;
}

const std::string &
HttpRequest::GetVersion () const
{
  return m_version;
}

void
HttpRequest::SetVersion (const std::string &version)
{
  m_version = version;
}
