#pragma once

#include <QObject>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include "mongoose.h"

typedef void OnRspCallback(mg_connection *c, std::string);
using ReqHandler = std::function<bool (std::string, std::string, mg_connection *c, OnRspCallback)>;

class HttpServer :public QObject
{
    Q_OBJECT

public:
	HttpServer() {}
	~HttpServer() {}
    void Init(const std::string &port);
	bool Close(); // 关闭
    void AddHandler(const std::string &url, ReqHandler req_handler);
    void RemoveHandler(const std::string &url);
    static std::unordered_map<std::string, ReqHandler> s_handler_map;
    static std::string s_web_dir; // 网页根目录
    static mg_serve_http_opts s_server_option; // web服务器选项

public slots:
    bool Start();

private:
	static void OnHttpSocketEvent(mg_connection *connection, int event_type, void *event_data);
	static void HandleHttpEvent(mg_connection *connection, http_message *http_req);
	static void SendHttpRsp(mg_connection *connection, std::string rsp);
    std::string m_port;
    mg_mgr m_mgr;
};

