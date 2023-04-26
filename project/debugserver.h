#pragma once

#include "httplib.h"

#include "pocketpy.h"
#include <thread>
#include <atomic>
#include <queue>

using namespace pkpy;

enum DebugServerState{
    DSS_NONE,
    DSS_LISTENING,
    DSS_PROCESSING,
    DSS_RETURNED
};

struct DebugServer {
    httplib::Server _server;
    std::thread _thread;
    
    std::string _buffer;
    bool _is_eval;
    bool _has_error;
    std::atomic<DebugServerState> _state;

    void _handler(const httplib::Request& req, httplib::Response &res){
        if(_state != DSS_LISTENING){
            res.status = 404;
            return;
        }
        _state = DSS_PROCESSING;
        _buffer = req.body;
        while(_state != DSS_RETURNED) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        if(_has_error) res.status = 500;
        else res.status = 200;
        res.set_content(_buffer, "text/plain");
        _state = DSS_LISTENING;
    }

    DebugServer() {
        _state = DSS_NONE;
        _server.Post("/pype/eval", [this](const httplib::Request& req, httplib::Response &res){
            _is_eval = true;
            _handler(req, res);
        });

        _server.Post("/pype/exec", [this](const httplib::Request& req, httplib::Response &res){
            _is_eval = false;
            _handler(req, res);
        });
    }

    void update(VM* vm){
        if(_state != DSS_PROCESSING) return;
        std::cout << "[" << (_is_eval?"EVAL":"EXEC") << " BEGIN]" << std::endl;
        std::cout << _buffer << std::endl;
        std::cout << "[" << (_is_eval?"EVAL":"EXEC") << " END]" << std::endl;
        try{
            CodeObject_ code = vm->compile(_buffer, "<remote>", _is_eval?EVAL_MODE:EXEC_MODE);
            PyObject* ret = vm->_exec(code, vm->_main);
            ret = vm->asRepr(ret);
            _buffer = CAST(Str&, ret).str();
            _has_error = false;
        }catch(Exception& e){
            _buffer = e.summary().str();
            _has_error = true;
        }
        _state = DSS_RETURNED;
    }

    void start() {
        _thread = std::thread([this]() {
            _state = DSS_LISTENING;
            _server.listen("127.0.0.1", 7433);
        });
    }

    ~DebugServer() {
        _state = DSS_NONE;
        _server.stop();
        _thread.join();
    }
};