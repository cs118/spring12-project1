/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * @file Header for HttpRequest class
 *
 * Skeleton for UCLA CS118 Spring quarter class
 */

#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include <string>
#include <list>
#include <boost/exception/all.hpp>

struct HttpHeader
{
  HttpHeader (const std::string &key, const std::string &value)
    : m_key (key), m_value (value) { }

  bool
  operator == (const std::string &key) const
  {
    return key == m_key;
  }
  
  std::string m_key;
  std::string m_value;
};

/**
 * Exception that will be thrown when parsing cannot be performed
 */
class ParseException : public boost::exception, public std::exception { };
class MethodNotSupported : public boost::exception, public std::exception { };

/**
 * @brief Class to parse/create HTTP requests
 *
 *  HttpRequest objects are created from parsing a buffer containing a HTTP
 *  request. The request buffer consists of a request line followed by a number
 *  of headers. Request line fields such as method, protocol etc. are stored
 *  explicitly. Headers such as 'Content-Length' and their values are maintained
 *  in a linked list.
 *
 * Example:
 *      // command line parsing
 *      HttpRequest req;
 *      req.SetHost ("www.google.com");
 *      req.SetPort (80);
 *      req.SetMethod (HttpRequest::GET);
 *      req.SetPath ("/");
 *      req.SetVersion ("1.0");
 *      req.AddHeader ("Accept-Language", "en-US");
 *    
 *      size_t reqLen = req.GetTotalLength ();
 *      char *buf = new char [reqLen];
 *    
 *      req.FormatRequest (buf);
 *      cout << buf;
 *      
 *      delete [] buf;
 */
class HttpRequest
{
public:
  enum MethodEnum
    {
      GET = 0,
      UNSUPPORTED = 1
    };

  /**
   * @brief Default constructor 
   *
   * Example:
   * HttpRequest req;
   * req.SetMethod (HttpRequest::GET);
   * req.SetHost ("www.google.com");
   * ...
   */
  HttpRequest ();
  
  /**
   * @brief Parse HTTP header 
   *
   * Example:
   * HttpRequest req;
   * const char *buf = "GET http://www.google.com:80/index.html/ HTTP/1.0\r\nContent-Length:"
   *                   " 80\r\nIf-Modified-Since: Sat, 29 Oct 1994 19:43:31 GMT\r\n\r\n";
   * req.Parse (buf);
   */
  void
  ParseRequest (const char *buffer);

  /**
   * @brief Get total length of the HTTP header (buffer size necessary to hold formatted HTTP request)
   */
  size_t
  GetTotalLength () const;

  /**
   * @brief Format HTTP request
   *
   * Note that formatted request will have \0 at the end, which should not be sent over TCP
   *
   * Another note. Buffer size should be enough to hold the request (e.g., obtained from GetTotalLength () call). Otherwise, anything can happen.
   *
   * @param buffer [out] Buffer that will hold formatted request
   * @returns Number of bytes actually written to the buffer
   */
  void
  FormatRequest (char *buffer) const;
  
  // Getters/Setters for HTTP request fields

  /**
   * @brief Get method of the HTTP request
   */
  MethodEnum
  GetMethod () const;

  /**
   * @brief Set method of the HTTP request
   */
  void
  SetMethod (MethodEnum method);

  // /**
  //  * @brief Set method of the HTTP request
  //  */
  // const std::string &
  // GetProtocol () const;

  // /**
  //  * @brief Set method of the HTTP request
  //  */
  // void
  // SetProtocol (const std::string &protocol);

  /**
   * @brief Get host of the HTTP request
   */
  const std::string &
  GetHost () const;

  /**
   * @brief Set host of the HTTP request
   */
  void
  SetHost (const std::string &host);

  /**
   * @brief Get port of the HTTP request
   */
  uint16_t
  GetPort () const;

  /**
   * @brief Set port of the HTTP request
   */
  void
  SetPort (uint16_t port);

  /**
   * @brief Get path of the HTTP request
   */
  const std::string &
  GetPath () const;
  
  /**
   * @brief Set path of the HTTP request
   */
  void
  SetPath (const std::string &path);
  
  /**
   * @brief Get version of the HTTP request
   */
  const std::string &
  GetVersion () const;
  
  /**
   * @brief Set version of the HTTP request
   */
  void
  SetVersion (const std::string &version);

  /**
   * @brief Get associated HTTP headers
   */
  const std::list<HttpHeader> &
  GetHeaders () const;

  /**
   * @brief Add HTTP header
   */
  void
  AddHeader (const std::string &key, const std::string &value);

  /**
   * @brief Remove HTTP header
   */
  void
  RemoveHeader (const std::string &key);

  /**
   * @brief Modify HTTP header
   *
   * Note that if header is currently not present, it will be added
   */
  void
  ModifyHeader (const std::string &key, const std::string &value);
  
private:
  MethodEnum  m_method;
  std::string m_host;
  uint16_t    m_port;
  std::string m_path;
  std::string m_version;
  
  std::list<HttpHeader> m_headers;
};

#endif // _HTTP_REQUEST_H_
