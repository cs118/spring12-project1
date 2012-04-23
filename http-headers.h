/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * @file Header for HttpRequest class
 *
 * Skeleton for UCLA CS118 Spring quarter class
 */

#ifndef _HTTP_HEADERS_H_
#define _HTTP_HEADERS_H_

#include <string>
#include <list>

/**
 * Exception that will be thrown when parsing cannot be performed
 */
class ParseException : public std::exception
{
public:
  ParseException (const std::string &reason) : m_reason (reason) { }
  virtual ~ParseException () throw () { }
  virtual const char* what() const throw ()
  { return m_reason.c_str (); }
private:
  std::string m_reason;
};

/**
 * @brief Class to parse/create HTTP headers
 */
class HttpHeaders
{
public:
  /**
   * @brief Default constructor 
   */
  HttpHeaders ();
  
  /**
   * @brief Parse HTTP headers 
   */
  const char*
  ParseHeaders (const char *buffer, size_t size);

  /**
   * @brief Get total length of the HTTP headers (buffer size necessary to hold formatted HTTP headers)
   */
  size_t
  GetTotalLength () const;

  /**
   * @brief Format HTTP headers
   *
   * Another note. Buffer size should be enough to hold the request (e.g., obtained from GetTotalLength () call). Otherwise, anything can happen.
   *
   * @param buffer [out] Buffer that will hold formatted request
   * @returns Number of bytes actually written to the buffer
   */
  char*
  FormatHeaders (char *buffer) const;
  
  // Getters/Setters for HTTP request fields

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

  /**
   * @brief Find value for the `key' http header
   *
   * If header doesn't exist, it the method will return a blank line
   */
  std::string
  FindHeader (const std::string &key);
  
private:
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

  std::list<HttpHeader> m_headers;
};

#endif // _HTTP_HEADERS_H_
