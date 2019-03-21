#ifndef COMMUNICATE_EPOLL_INCLUDE_EPOLL_H_
#define COMMUNICATE_EPOLL_INCLUDE_EPOLL_H_

/*创建套接字并进行绑定*/
int socket_bind( const char* ip, int port);
/*IO多路复用epoll*/
void do_epoll( int listenfd);
/*事件处理函数*/
void handle_events( int epollfd, struct epoll_event *events, int num, int listenfd, char *buf);
/*处理接收到的连接*/
void handle_accpet( int epollfd, int listenfd);
/*读处理*/
void do_read( int epollfd, int fd, char *buf);
/*写处理*/
void do_write( int epollfd, int fd, char *buf);
/*添加事件*/
void add_event( int epollfd, int fd, int state);
/*修改事件*/
void modify_event( int epollfd, int fd, int state);
/*删除事件*/
void delete_event( int epollfd, int fd, int state);

#endif /* COMMUNICATE_EPOLL_INCLUDE_EPOLL_H_ */
