#include "src/rpc_server.h"

#include <sys/socket.h>

int add(int a, int b)
{
    return a + b;
}

int main()
{
    auto server = paddle::rpcServer::GetInstance();
    server.Register("add", add);
    server.Call("add","1 2",3);

    return 0;
}