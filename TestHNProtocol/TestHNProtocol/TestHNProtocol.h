#ifndef TEST_HN_PROTOCOL_H
#define TEST_HN_PROTOCOL_H



//#define TEST_TCP_CONNECT

#ifdef TEST_TCP_CONNECT
#define SERVER_TEST 0  // 编译时用于控制编译server/client
#if SERVER_TEST
#define CLIENT_TEST 0
#else
#define CLIENT_TEST 1
#endif
#endif // TEST_TCP_CONNECT














#endif