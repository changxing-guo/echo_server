#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

#define SERVER_PORT 666

int main(void)
{
    int sock; // 代表信箱
    int i;
    struct sockaddr_in server_addr;


    //1.创建一个信箱
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    //2.清空标签。写上地址和端口号
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // 选择协议族，ipv4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);// 监听本地所有的IP地址
    server_addr.sin_port = htons(SERVER_PORT);//绑定端口号

    //实现标签贴到收信的信箱上
    bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    
    //把信箱挂在传达室，这样，保安就可以接信了
    listen(sock, 64);// 同一时间端只能接受64封

    // 完事具备，只能来信
    printf("等待客户端的连接\n");

    int done = 1;
    while (done) {
        struct sockaddr_in client;
        int client_sock;
        char client_ip[64];
        char buf[256];
        int len;

        socklen_t client_addr_len;
        client_addr_len = sizeof(client);
        client_sock = accept(sock, (struct sockaddr *)&client, &client_addr_len);
        
        // 打印客户端的IP地址和端口号
        printf("client ip: %s\t port: %d\n",
               inet_ntop(AF_INET, &client.sin_addr.s_addr, client_ip, sizeof(client_ip)),
               ntohs(client.sin_port));
        // 读取客户端发送的数据
        len = read(client_sock, buf, sizeof(buf) - 1);
        buf[len] = '\0';
        printf("recive[%d]: %s\n", len,  buf);

        // 转换成大写
        for(i=0; i<len; i++){
            /*if (buf[i] >= 'a' && buf[i] <= 'z') {
                buf[i] = buf[i] - 32;
            }*/
            buf[i] = toupper(buf[i]);
        }
        len = write(client_sock, buf, len);
        printf("write finished. len = %d\n", len);
        close(client_sock);
    }
    close(sock);
    return 0;
}
