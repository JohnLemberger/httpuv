#ifndef WEBAPPLICATION_HPP
#define WEBAPPLICATION_HPP

#include <boost/function.hpp>
#include <uv.h>
#include <Rcpp.h>
#include "websockets.h"

class HttpRequest;
class HttpResponse;

class WebApplication {
public:
  virtual ~WebApplication() {}
  virtual void onHeaders(HttpRequest* pRequest, boost::function<void(HttpResponse*)> callback) = 0;
  virtual void onBodyData(HttpRequest* pRequest,
                          const char* data, size_t len) = 0;
  virtual void getResponse(HttpRequest* request, boost::function<void(HttpResponse*)> callback) = 0;
  virtual void onWSOpen(HttpRequest* pRequest) = 0;
  virtual void onWSMessage(WebSocketConnection* conn,
                           bool binary, const char* data, size_t len) = 0;
  virtual void onWSClose(WebSocketConnection* conn) = 0;
};


class RWebApplication : public WebApplication {
private:
  Rcpp::Function _onHeaders;
  Rcpp::Function _onBodyData;
  Rcpp::Function _onRequest;
  Rcpp::Function _onWSOpen;
  Rcpp::Function _onWSMessage;
  Rcpp::Function _onWSClose;

public:
  RWebApplication(Rcpp::Function onHeaders,
                  Rcpp::Function onBodyData,
                  Rcpp::Function onRequest,
                  Rcpp::Function onWSOpen,
                  Rcpp::Function onWSMessage,
                  Rcpp::Function onWSClose) :
    _onHeaders(onHeaders), _onBodyData(onBodyData), _onRequest(onRequest),
    _onWSOpen(onWSOpen), _onWSMessage(onWSMessage), _onWSClose(onWSClose) {
  }

  virtual ~RWebApplication() {}

  virtual void onHeaders(HttpRequest* pRequest, boost::function<void(HttpResponse*)> callback);
  virtual void onBodyData(HttpRequest* pRequest,
                          const char* data, size_t len);
  virtual void getResponse(HttpRequest* request, boost::function<void(HttpResponse*)> callback);
  virtual void onWSOpen(HttpRequest* pRequest);
  virtual void onWSMessage(WebSocketConnection* conn,
                           bool binary, const char* data, size_t len);
  virtual void onWSClose(WebSocketConnection* conn);
};


#endif // WEBAPPLICATION_HPP
