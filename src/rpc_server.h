#ifndef PADDLE_RPC_SERVER_H
#define PADDLE_RPC_SERVER_H

#include <functional>
#include <string>
#include <unordered_map>

#include "logging.h"
#include "traits.h"

namespace paddle
{
    template <typename F, size_t... i, typename T>
    decltype(auto) call_func_helper(F &&func, std::index_sequence<i...>, T &&t)
    {
        return func(std::get<i>(std::forward<T>(t))...);
    }

    template <typename F, typename T>
    decltype(auto) call_func(F &&func, T &&t)
    {
        constexpr auto size = std::tuple_size<typename std::decay<T>::type>::value;
        return call_func_helper(std::forward<F>(func), std::make_index_sequence<size>{}, std::forward<T>(t));
    }
    class rpcServer
    {
    private:
        std::unordered_map<std::string, std::function<void(const char *data, int len)>> methods_;

    public:
        //注册函数
        template <typename F>
        void Register(const std::string funcName, F handler)
        {
            methods_[funcName] = std::bind(&rpcServer::CallProxy<F>, this, handler, std::placeholders::_1, std::placeholders::_2);
        }

        /**
         * @brief 注销方法
         * 
         * @param method_name 
         */
        void UnRegister(std::string method_name)
        {
            methods_.erase(method_name);
        }

        //客户端处理函数调用
        void Call(std::string funcName, const char *data, int len)
        {
            auto func = methods_[funcName];
            func(data, len); //跳转到callproxy
        }

        template <typename F>
        void CallProxy(F func, const char *data, int len)
        {
            //把函数参数类型萃取出来
            using args_type = typename function_traits<F>::tuple_type;
            int err = 0;
            args_type args{1, 2};
            auto res = paddle::call_func(func,args);
            std::cout<<res<<std::endl;
        }

    private:
        rpcServer() {}

    public:
        static rpcServer GetInstance()
        {
            static rpcServer instance;
            return instance;
        }

        /**
         * @brief server运行
         * 
         */
        void Run()
        {
        }

    }; //rpcServer
} //paddle

#endif