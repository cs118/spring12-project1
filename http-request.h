/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * @file Header for HttpRequest class
 *
 * Skeleton for UCLA CS118 Spring quarter class
 */

#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include "http-headers.h"

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
class HttpRequest : public HttpHeaders
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
  const char*
  ParseRequest (const char *buffer, size_t size);

  /**
   * @brief Get total length of the HTTP header (buffer size necessary to hold formatted HTTP request)
   */
  size_t
  GetTotalLength () const;

  /**
   * @brief Format HTTP request
   *
   * Note that buffer size should be enough to hold the request (e.g., obtained from GetTotalLength () call). Otherwise, anything can happen.
   *
   * @param buffer [out] Buffer that will hold formatted request
   * @returns Number of bytes actually written to the buffer
   */
  char*
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
  unsigned short
  GetPort () const;

  /**
   * @brief Set port of the HTTP request
   */
  void
  SetPort (unsigned short port);

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
  
private:
  MethodEnum  m_method;
  std::string m_host;
  unsigned short m_port;
  std::string m_path;
  std::string m_version;
};

#endif // _HTTP_REQUEST_H_
