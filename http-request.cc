/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * @file Implementation for HttpRequest class
 *
 * Skeleton for UCLA CS118 Spring quarter class
 */

#include "http-request.h"

HttpRequest::HttpRequest ()
  : m_method (UNSUPPORTED)
  , m_port (0)
{
}
  
void
HttpRequest::Parse (const char *buffer)
{
}


void
HttpRequest::GetTotalLength () const
{
}

size_t
HttpRequest::FormatRequest (char *buffer, size_t bufferSize) const
{
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

const std::string &
HttpRequest::GetProtocol () const
{
  return m_protocol;
}

void
HttpRequest::SetProtocol (const std::string &protocol)
{
  m_protocol = protocol;
}

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

