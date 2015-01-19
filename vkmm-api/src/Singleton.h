#pragma once

#include <QObject>
#include <QMutex>

namespace VKMM
{

template <class T>
class Singleton
{
public:
	static T * instance()
    {
		_mutex.lock();

		if (!_instance)
		{
			if (!_destroyed)
			{
				static T instance;
				_instance = &instance;
			} else {
				// throw boost::exception("Cannot instance of destroyed singleton");
			}
		}
		_mutex.unlock();
        return _instance;   // return it
    }

protected:

	Singleton()
	{}

	~Singleton()
	{
		_instance = NULL;
		_destroyed = true;
	}

private:
    Singleton(const Singleton &); // hide copy constructor
    Singleton& operator=(const Singleton &); // hide assign op

	static T * _instance; // create static instance of our class
	static QMutex _mutex;
	static bool _destroyed;
};

template <class T> QMutex Singleton<T>::_mutex;
template <class T> T * Singleton<T>::_instance = NULL;
template <class T> bool Singleton<T>::_destroyed = false;

}
