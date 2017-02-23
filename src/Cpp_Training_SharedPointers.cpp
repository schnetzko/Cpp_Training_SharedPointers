//============================================================================
// Name        : Cpp_Training_SharedPointers.cpp
// Author      : Nico
// Version     :
// Copyright   : Your copyright notice
// Description : Testing SmartPointers in C++, Ansi-style
//============================================================================

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;

struct Base {
	Base() { cout << "Base::Base() constructor called\n";};
	~Base() { cout << "Base::~Base() destructor called\n";};
};

struct Derived: public Base {
	Derived() { cout << "Derived::Derived() constructor called\n";};
	~Derived() { cout << "Derived::~Derived() destructor called\n";};
};

void thr(shared_ptr<Base> ptr){
	this_thread::sleep_for(chrono::seconds(1));
	shared_ptr<Base> local_ptr = ptr;
	{
		static mutex mut;
		lock_guard<mutex> lock(mut); // synchronization just for printing an output block for each thread
		cout << "\tlocal pointer in thread " << this_thread::get_id() << "\n";;
		cout << "\tlocal pointer address: " << local_ptr.get() << "\n";
		cout << "\tnumber of owners: " << local_ptr.use_count() << "\n";
	}
}

int main() {
	shared_ptr<Base> derived = make_shared<Derived>();
	cout << "created a shared pointer to a \"derived\" object\n";
	cout << "pointer address: " << derived.get() << "\n";
	cout << "number of owners: " << derived.use_count() << "\n";

	// create and start threads
	thread t1(thr, derived);
	thread t2(thr, derived);
	thread t3(thr, derived);

	// release ownership by the main thread
	derived.reset();
	cout << "main thread released his ownership of shared pointer" << "\n";
	cout << "pointer address: " << derived.get() << "\n";
	cout << "number of owners: " << derived.use_count() << "\n";

	t1.join(); t2.join(); t3.join();

	cout << "All threads are finished!" << "\n";

	return 0;
}
