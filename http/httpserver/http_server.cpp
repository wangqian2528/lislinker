#include "http_server.h"
#include <utility>
#include <iostream>

void HttpServer::Init(const std::string &port)
{
	m_port = port;
}

bool HttpServer::Start()
{
	mg_mgr_init(&m_mgr, NULL);
	mg_connection *connection = mg_bind(&m_mgr, m_port.c_str(), HttpServer::OnHttpSocketEvent);
	if (connection == NULL)
		return false;
	// for both http and websocket
	mg_set_protocol_http_websocket(connection);

	printf("starting http server at port: %s\n", m_port.c_str());
	// loop
	while (true)
		mg_mgr_poll(&m_mgr, 500); // ms

	return true;
}

void HttpServer::OnHttpSocketEvent(mg_connection *connection, int event_type, void *event_data)
{
	if (event_type == MG_EV_HTTP_REQUEST)
	{
		http_message *http_req = (http_message *)event_data;
		HandleHttpEvent(connection, http_req);
	}
}

// ---- simple http ---- //
static bool route_check(http_message *http_msg, const char *route_prefix)
{
	if (mg_vcmp(&http_msg->uri, route_prefix) == 0)
		return true;
	else
		return false;

	// TODO: 还可以判断 GET, POST, PUT, DELTE等方法
	//mg_vcmp(&http_msg->method, "GET");
	//mg_vcmp(&http_msg->method, "POST");
	//mg_vcmp(&http_msg->method, "PUT");
	//mg_vcmp(&http_msg->method, "DELETE");
}

static bool method_check(http_message *http_msg, const char *_method)
{
	// TODO: 还可以判断 GET, POST, PUT, DELTE等方法
	//mg_vcmp(&http_msg->method, "GET");
	//mg_vcmp(&http_msg->method, "POST");
	//mg_vcmp(&http_msg->method, "PUT");
	//mg_vcmp(&http_msg->method, "DELETE");
	if (mg_vcmp(&http_msg->method, _method) == 0)
		return true;
	else
		return false;
}

void HttpServer::AddHandler(const std::string &url, ReqHandler req_handler)
{
	if (s_handler_map.find(url) != s_handler_map.end())
		return;

	s_handler_map.insert(std::make_pair(url, req_handler));
}

void HttpServer::RemoveHandler(const std::string &url)
{
	auto it = s_handler_map.find(url);
	if (it != s_handler_map.end())
		s_handler_map.erase(it);
}

void HttpServer::SendHttpRsp(mg_connection *connection, std::string rsp)
{
	// --- 开启CORS
	mg_printf(connection, "HTTP/1.1 200 OK\r\n"
			  "Content-Type: *\n"
			  "Cache-Control: no-cache\n"
			  "Content-Length: %d\n"
			  "Access-Control-Allow-Origin: *\n"
			  "Access-Control-Allow-Headers: Content-Type,XFILENAME,XFILECATEGORY,XFILESIZE\n\n"
			  "%s\n", rsp.length(), rsp.c_str());

	// 以json形式返回
	mg_printf_http_chunk(connection, "%s", rsp.c_str());

	// 发送空白字符快，结束当前响应
	mg_send_http_chunk(connection, "", 0);
}

void HttpServer::HandleHttpEvent(mg_connection *connection, http_message *http_req)
{
	std::string req_str = std::string(http_req->message.p, http_req->message.len);
	printf("got request: %s\n", req_str.c_str());

	std::string url = std::string(http_req->uri.p, http_req->uri.len);
	std::string body = std::string(http_req->body.p, http_req->body.len);
	auto it = s_handler_map.find(url);
	if (it != s_handler_map.end())
	{
		ReqHandler handle_func = it->second;
		handle_func(url, body, connection, &HttpServer::SendHttpRsp);
	}

	// 其他请求
	if (method_check(http_req, "OPTIONS"))
	{
		SendHttpRsp(connection, "OK");
		return;
	}

	if (route_check(http_req, "/api/test")) 
	{
		// 直接回传
		SendHttpRsp(connection, "OK");
	}
	else
	{
		mg_printf(
			connection,
			"%s",
			"HTTP/1.1 501 Not Implemented\r\n" 
			"Content-Length: 0\r\n\r\n");
	}
}

bool HttpServer::Close()
{
	mg_mgr_free(&m_mgr);
	return true;
}
