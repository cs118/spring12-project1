/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * @file Header for HttpResponse class
 *
 * Skeleton for UCLA CS118 Spring quarter class
 */

#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include "http-headers.h"

/**
 * @brief Class to parse/create HTTP responses
 *
 *  HttpResponse objects are created from parsing a buffer containing a HTTP
 *  response. 
 */
class HttpResponse : public HttpHeaders
{
public:
  /**
   * @brief Default constructor 
   *
   * Example:
   * HttpResponse resp;
   * ...
   */
  HttpResponse ();
  
  /**
   * @brief Parse HTTP header 
   *
   * Example:
   * HttpResponse resp;
   * resp.ParseResponse (buf, size);
   */
  const char*
  ParseResponse (const char *buffer, size_t size);

  /**
   * @brief Get total length of the HTTP header (buffer size necessary to hold formatted HTTP response)
   */
  size_t
  GetTotalLength () const;

  /**
   * @brief Format HTTP response
   *
   * Note that buffer size should be enough to hold the response (e.g., obtained from GetTotalLength () call). Otherwise, anything can happen.
   *
   * @param buffer [out] Buffer that will hold formatted response
   * @returns Number of bytes actually written to the buffer
   */
  char*
  FormatResponse (char *buffer) const;
  
  // Getters/Setters for HTTP response fields
  
  /**
   * @brief Get version of the HTTP response
   */
  const std::string &
  GetVersion () const;
  
  /**
   * @brief Set version of the HTTP response
   */
  void
  SetVersion (const std::string &version);

  /**
   * @brief Get status code
   */
  const std::string &
  GetStatusCode () const;

  /**
   * @brief Set status code
   */
  void
  SetStatusCode (const std::string &code);

  /**
   * @brief Get status message
   */
  const std::string &
  GetStatusMsg () const;

  /**
   * @brief Set status message
   */
  void
  SetStatusMsg (const std::string &msg);
  
private:
  std::string m_version;
  std::string m_statusCode;
  std::string m_statusMsg;
};

#endif // _HTTP_RESPONSE_H_
