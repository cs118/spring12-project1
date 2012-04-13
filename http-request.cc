/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * @file Implementation for HttpRequest class
 *
 * Skeleton for UCLA CS118 Spring quarter class
 */

#include "http-request.h"

#include <string> // C++ STL string
#include <string.h> // helpers to copy C-style strings

using namespace std;

// comment the following line to disable debug output
#define _DEBUG 1 

#ifdef _DEBUG
#include <iostream>
#define TRACE(x) std::clog << x << endl;
#else
#endif // _DEBUG

#include <boost/throw_exception.hpp>
#include <boost/lexical_cast.hpp>

typedef boost::error_info<struct tag_errmsg, string> ErrInfo;

HttpRequest::HttpRequest ()
  : m_method (UNSUPPORTED)
  , m_port (0)
{
}
  
void
HttpRequest::ParseRequest (const char *buffer)
{
}


size_t
HttpRequest::GetTotalLength () const
{
  if (m_method != GET)
    throw ParseException () << ErrInfo ("Only GET method is supported");
      
  size_t len = 4; // 'GET '
  len += m_path.size () + 1; // '<path> '
  len += 5; // 'HTTP/'
  len += m_version.size (); // '1.0'
  len += 2; // '\r\n'

  len += 6; //'Host: '
  len += m_host.size ();
  if (m_port != 80)
    len += 1 + boost::lexical_cast<string> (m_port).size (); // :<port>
  len += 2; // '\r\n'
  
  for (std::list<HttpHeader>::const_iterator header = m_headers.begin ();
       header != m_headers.end ();
       header++)
    {
      len += header->m_key.size () + 2/*: */+ header->m_value.size () + 2/*\r\n*/;
    }
  
  return len + 1; // for last '\0'
}

void
HttpRequest::FormatRequest (char *buffer) const
{
  if (m_method != GET)
    throw ParseException () << ErrInfo ("Only GET method is supported");

  char *bufLastPos = buffer;
  
  bufLastPos = stpcpy (bufLastPos, "GET ");
  bufLastPos = stpcpy (bufLastPos, m_path.c_str ());
  bufLastPos = stpcpy (bufLastPos, " HTTP/");
  bufLastPos = stpcpy (bufLastPos, m_version.c_str ());
  bufLastPos = stpcpy (bufLastPos, "\r\n");

  bufLastPos = stpcpy (bufLastPos, "Host: ");
  bufLastPos = stpcpy (bufLastPos, m_host.c_str ());
  if (m_port != 80)
    {
      bufLastPos = stpcpy (bufLastPos, ":");
      bufLastPos = stpcpy (bufLastPos, boost::lexical_cast<string> (m_port).c_str ());
    }
  bufLastPos = stpcpy (bufLastPos, "\r\n");
  
  for (std::list<HttpHeader>::const_iterator header = m_headers.begin ();
       header != m_headers.end ();
       header++)
    {
      bufLastPos = stpcpy (bufLastPos, header->m_key.c_str ());
      bufLastPos = stpcpy (bufLastPos, ": ");
      bufLastPos = stpcpy (bufLastPos, header->m_value.c_str ());
      bufLastPos = stpcpy (bufLastPos, "\r\n");
    }
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
}

uint16_t
HttpRequest::GetPort () const
{
  return m_port;
}

void
HttpRequest::SetPort (uint16_t port)
{
  m_port = port;
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

////////////////////////////////////////////////////////////////////////

const std::list<HttpHeader> &
HttpRequest::GetHeaders () const
{
  return m_headers;
}

void
HttpRequest::AddHeader (const std::string &key, const std::string &value)
{
  m_headers.push_back (HttpHeader (key, value));
}

void
HttpRequest::RemoveHeader (const std::string &key)
{
  std::list<HttpHeader>::iterator item = std::find (m_headers.begin (), m_headers.end (), key);
  if (item != m_headers.end ())
    m_headers.erase (item);
}

void
HttpRequest::ModifyHeader (const std::string &key, const std::string &value)
{
  std::list<HttpHeader>::iterator item = std::find (m_headers.begin (), m_headers.end (), key);
  if (item != m_headers.end ())
    item->m_value = value;
  else
    AddHeader (key, value);
}
