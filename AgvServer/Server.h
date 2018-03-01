#pragma once

#include <boost/bind.hpp>
#include <boost/pool/pool.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/thread/executors/basic_thread_pool.hpp>

#include "TcpServer.h"

//单例模式
class Server
{
public:
	virtual ~Server();

	static Server* GetInstance();

	//1.初始化数据库连接，这是基础
	bool initSql();

	//2.根据数据库内容，初始化所有的member
	bool initAll();

	//3.开启监听
	void initListen();


	void pushPackage(Client_Msg package);

	void PopPackage();

	void publisher_agv_position();
	void publisher_agv_status();
	void publisher_task();
	void publisher_log();

	template <typename T>
	void RunTask(T task)
	{
		m_task_pool.submit(task);
	}

	template <typename T>
	void RunPackTask(T task)
	{
		m_pack_pool.submit(task);
	}
private:
	Server();

	static  Server *m_instance;

	//任务线程池
	boost::basic_thread_pool m_task_pool;

	//消息(pack)处理线程池
	boost::basic_thread_pool m_pack_pool;

	//消息队列
	boost::lockfree::queue<Client_Msg> *m_package;

	int         getQueueLength() const
	{
		return _push_times - _pop_times;
	}

	boost::atomic_uint32_t               _push_times;
	boost::atomic_uint32_t               _pop_times;
};
