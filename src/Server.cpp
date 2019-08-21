
#include "Server.h"

#include <sys/socket.h>	// for socket(), bind(), listen(), accept()
#include <netinet/in.h>	// for PF_INET, SOCK_STREAM, IPPROTO_TCP 
#include <errno.h>
#include <string.h>
#include <unistd.h>	
#include <errno.h>
#include "Logger.h"

Server::Server(std::shared_ptr<DataLogger> logger, int port) : port(port)
{
    this->logger = logger;
    this->port = port;
    
    socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(socket_fd == 0)
    {
        DEBUG_ERROR("Could not create socket");
        return;
    }

    int enable = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        DEBUG_ERROR("Could set socket options");
        return;
    }

    struct sockaddr_in address;
    memset(&address, 0, sizeof(sockaddr_in));

    address.sin_family = PF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);    
   
    int ret = 0;
    if ((ret = bind( socket_fd, (sockaddr *)&address, sizeof(sockaddr_in))) < 0 )
    { 
        DEBUG_ERROR("Failed to bind: return value {}, {}", ret, errno);
        return;
    }

    if ( listen( socket_fd, 5 ) < 0 )
    {
        DEBUG_ERROR("Failed to listen");
        return;
    }

};

Server::~Server()
{
    close(client_fd);
    close(socket_fd);
}



void Server::receiveData()
{
    while(threadRunning)
    {
        while(running)
        {
            sockaddr_in	client_address;
            socklen_t len = sizeof(client_address);
            memset(&client_address, 0, sizeof(sockaddr_in));

            DEBUG_INFO("Waiting for client to connect");
            client_fd = accept( socket_fd, (sockaddr *)&client_address, &len);
            if ( client_fd < 0 )
            {
                DEBUG_ERROR("Could not accept client");
                running = false;
                break;
            }
            state = AppStates::STANDBY;

            int ret = 0;
            DataFrame f;
            DEBUG_INFO("Client connected. Receiving data.");


            while((ret = recv(client_fd, &f, 65, 0)) > 0)
            {   
                if(!running)
                {
                    break;

                }
                switch((int)f.code)
                {
                    case 0:
                    DEBUG_INFO("Client switched to STANDBY");
                    state = AppStates::STANDBY;
                    break;
                    
                    case 1:
                    DEBUG_INFO("Client switched to LOGGING");
                    state = AppStates::LOGGING;
                    break;

                    case 5: case 6:
                    DEBUG_INFO("Pushing frame");
                    logger->queueDataFrame(f);
                    break;

                    default:
                    break;
                }
            }

            DEBUG_INFO("Client disconnected");
            state = AppStates::DISCONNECTED;
            close(client_fd);

        }
    }
}

void Server::Start()
{
    running = true;
}


void Server::Stop()
{
    DEBUG_INFO("Stopping server");


    threadRunning = false;
    running = false;

    shutdown(socket_fd, SHUT_RDWR);
    shutdown(client_fd, SHUT_RDWR);
}

