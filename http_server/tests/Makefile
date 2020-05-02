test_FixedThreadPool:test_FixedThreadPool.cpp ../Thread.cpp ../FixedThreadPool.cpp
	g++ test_FixedThreadPool.cpp ../Thread.cpp ../FixedThreadPool.cpp ../mutex.cpp -lpthread -g -o test_FixedThreadPool
test_client:test_client.cpp ../utils.cpp ../Log.cpp ../Socket.cpp ../InetAddress.cpp ../Thread.cpp ../mutex.cpp
	g++ test_client.cpp ../utils.cpp ../Log.cpp ../Socket.cpp ../InetAddress.cpp ../Thread.cpp ../mutex.cpp -lpthread -g -o test_client
test_epoller:test_epoller.cpp ../eventloop.cpp ../mutex.cpp ../InetAddress.cpp ../Log.cpp ../utils.cpp ../dispatcher.cpp ../epoller.cpp ../Socket.cpp
	g++ test_epoller.cpp ../utils.cpp ../eventloop.cpp ../Log.cpp ../mutex.cpp ../InetAddress.cpp ../dispatcher.cpp ../epoller.cpp ../Socket.cpp -lpthread -g -o test_epoller
test_server:test_server.cpp ../utils.cpp ../Log.cpp ../Socket.cpp ../InetAddress.cpp ../Thread.cpp ../mutex.cpp
	g++ test_server.cpp ../utils.cpp ../Log.cpp ../Socket.cpp ../InetAddress.cpp ../Thread.cpp ../mutex.cpp -lpthread -g -o test_server
test_TimerHeap:test_TimerHeap.cpp ../TimerHeap.cpp ../mutex.cpp
	g++ test_TimerHeap.cpp ../TimerHeap.cpp ../mutex.cpp -lpthread -g -o test_TimerHeap
