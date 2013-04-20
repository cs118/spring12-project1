/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * @file Implementation for HttpRequest class
 *
 * Skeleton for UCLA CS118 Spring quarter class
 */

#include "http-headers.h"

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
#include <boost/algorithm/string.hpp>

HttpHeaders::HttpHeaders ()
{
}
  
const char*
HttpHeaders::ParseHeaders (const char *buffer, size_t size)
{
  const char *curPos = buffer;

  string key;
  string value;
  
  while (((size_t)(curPos-buffer) <= size-2) &&
         (*curPos != '\r' && *(curPos+1) != '\n'))
    {
      const char *endline = (const char *)memmem (curPos, size - (curPos-buffer), "\r\n", 2);
      if (endline == 0)
        {
          throw ParseException ("Header line does end with \\r\\n");
        }
      
      if (*curPos == ' ' || *curPos == '\t') // multi-line header
        {
          if (key == "")
            throw ParseException ("Multi-line header without actual header");

          
          string newline (curPos, endline-curPos);
          // TRACE ("Multi-line header: " << value << " + " << newline);

          // reusing key from previous iteration
          value += "\r\n" + newline;
          ModifyHeader (key, value);
        }
      else
        {
          const char *header_key = (const char*)memmem (curPos, endline - curPos, ":", 1);

          if (header_key == 0)
            {
              throw ParseException ("HTTP header doesn't contain ':'");
            }

          key = string (curPos, header_key-curPos);
          value = string (header_key+1, endline - header_key - 1);
          boost::trim (value); // remove any leading spaces if present
      
          // TRACE ("Key: [" << key << "], value: [" << value << "]");

          ModifyHeader (key, value);
        }
      
      curPos = endline + 2;
    }

  // TRACE ("Left: " << (int)(curPos-buffer) << ", size: " << size);
  if (static_cast<size_t> (curPos-buffer+2) <= size)
    {
      curPos += 2; // skip '\r\n'
    }
  else
    {
      throw ParseException ("Parsed buffer does not contain \\r\\n");
    }
  return curPos;
}

size_t
HttpHeaders::GetTotalLength () const
{
  size_t len = 0;
  for (std::list<HttpHeader>::const_iterator header = m_headers.begin ();
       header != m_headers.end ();
       header++)
    {
      len += header->m_key.size () + 2/*: */+ header->m_value.size () + 2/*\r\n*/;
    }
  
  return len;
}

char*
HttpHeaders::FormatHeaders (char *buffer) const
{
  char *bufLastPos = buffer;
  
  for (std::list<HttpHeader>::const_iterator header = m_headers.begin ();
       header != m_headers.end ();
       header++)
    {
      bufLastPos = stpncpy (bufLastPos, header->m_key.c_str (), header->m_key.size ());
      bufLastPos = stpncpy (bufLastPos, ": ", 2);
      bufLastPos = stpncpy (bufLastPos, header->m_value.c_str (), header->m_value.size ());
      bufLastPos = stpncpy (bufLastPos, "\r\n", 2);
    }
  
  return bufLastPos;
}


void
HttpHeaders::AddHeader (const std::string &key, const std::string &value)
{
  m_headers.push_back (HttpHeader (key, value));
}

void
HttpHeaders::RemoveHeader (const std::string &key)
{
  std::list<HttpHeader>::iterator item = std::find (m_headers.begin (), m_headers.end (), key);
  if (item != m_headers.end ())
    m_headers.erase (item);
}

void
HttpHeaders::ModifyHeader (const std::string &key, const std::string &value)
{
  std::list<HttpHeader>::iterator item = std::find (m_headers.begin (), m_headers.end (), key);
  if (item != m_headers.end ())
    item->m_value = value;
  else
    AddHeader (key, value);
}

std::string
HttpHeaders::FindHeader (const std::string &key)
{
  std::list<HttpHeader>::iterator item = std::find (m_headers.begin (), m_headers.end (), key);
  if (item != m_headers.end ())
    return item->m_value;
  else
    return "";
}
