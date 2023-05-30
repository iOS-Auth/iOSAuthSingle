#include <iOSAuthRuntime/authorize.h>



// 线程
void auth_single_exec_thread(SOCKET _Socket) noexcept
{
	auth_exec_sync_socket(_Socket);
	auth_socket_close(_Socket);
}

// 执行
int auth_single_exec() noexcept
{
	auto 		vSocket = auth_socket_bind(27987);
	if(INVALID_SOCKET == vSocket)
	{
		return -1;
	}

	// 循环监听
	while(true)
	{
		struct sockaddr_in	vClientAddress {};
		socklen_t		vClientSize = sizeof(struct sockaddr_in);
		auto			vClient = accept(vSocket, (struct sockaddr*)&vClientAddress, &vClientSize);
		if(INVALID_SOCKET == vClient)
		{
			break;
		}
		auto			vClientIP = inet_ntoa(vClientAddress.sin_addr);
		std::printf(u8"[%s : %d] Client ip: %s\n", __FUNCTION__, __LINE__, vClientIP);
		std::thread(auth_single_exec_thread, vClient).detach();
	}
	auth_socket_close(vSocket);
	return 0;
}



// 入口
int main(int _Argc, char** _Argv)
{
	UNREFERENCED_PARAMETER(_Argc);
	UNREFERENCED_PARAMETER(_Argv);

	WSADATA		vWsaData;
	WSAStartup(MAKEWORD(2, 2), &vWsaData);
	OleInitialize(nullptr);

	auth_single_exec();

	OleUninitialize();
	WSACleanup();
	return 0;
}
