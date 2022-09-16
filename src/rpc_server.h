#ifndef PADDLE_RPC_SERVER_H
#define PADDLE_RPC_SERVER_H

#include <functional>
#include <string>
#include <unordered_map>

namespace paddle
{
    class rpcServer
    {
    private:
        std::unordered_map<std::string, std::function<void()>> methods_;

    public:
        /**
         * @brief 注册方法
         * 
         * @tparam F 
         * @tparam Args 
         * @param method_name 方法名
         * @param f 
         * @param args 
         */
        template <typename F, typename... Args>
        void Register(std::string method_name, F &&f, Args &&...args)
        {
            std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            methods_[method_name] = func;
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

        /**
         * @brief 调用指定方法
         * 
         * @param method_name 方法名 
         * @param data 参数
         */
        void Call(std::string method_name,std::string data){

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