#pragma once
#include <vector>
#include "MemoryManager.h"

class _Pool_Base {
	void RemoveObjectFromActive(const void* o) {
		auto it = std::find(activeList.begin(), activeList.end(), o);

		if(it != activeList.end()) {
			std::swap(*it, activeList.back());
			activeList.pop_back();
		}
	}
protected:
	static MemoryManager* memManage;
	std::vector<void*> activeList;
	std::vector<void*> inactiveList; //Linked list with head/tail ptrs
public:

	static void RegisterMemory(MemoryManager* mem) {
		memManage = mem;
	}

	void DeactivateMemory(void* o) {
		RemoveObjectFromActive(o);
		inactiveList.push_back(o);
	}

	void* ActivateMemory() {
		if(inactiveList.size()) {
			activeList.push_back(inactiveList[0]);
			inactiveList.erase(inactiveList.begin());
			return activeList.back();
		}
		else {
			throw std::out_of_range("Pool is not of sufficient size.");
			//GET READY TO CRASH :D
			//we already did D:
			return nullptr;
		}
	}
};

template<typename T>
class Pool:
	//Since we implement our own versions (template specific) of the (de)activate methods, we don't want the PoolBase ones to be exposed, therefore we use private inheritance
	private _Pool_Base {
	T* elements;
	size_t pool_size;
public:
	Pool(size_t size = (size_t)128): pool_size((size_t)size), elements((T*)memManage->RequestMemory((unsigned)size, sizeof(T))) {
		inactiveList.resize(pool_size);
		activeList.reserve(pool_size);
		for(size_t i = 0; i < pool_size; ++i) {
			//WTF WHY DOES THIS WORK BUT NOT &elements ????????
			//inactiveList[i] = (char*)new ((char*)elements + sizeof(T) * i) T();
			inactiveList[i] = (char*)new (&elements[i]) T();
		}
	}

	~Pool() {
		//We want to destruct in reverse order so the first created is the first destructed
		for(size_t i = pool_size; i-- > 0;) { 
			elements[i].~T();
		}
	}

	inline T &operator[](const size_t index) { return *(T*)activeList[index]; }

	/// <summary>
	/// Gets the items.
	/// </summary>
	/// <returns>T *.</returns>
	inline T* GetItems() const { return elements; }

	/// <summary>
	/// Activates the specified object.
	/// </summary>
	/// <param name="o">The object to activate.</param>
	/// <returns>A pointer to the activated or added Object.</returns>
	inline T* ActivateMemory() { return (T*)_Pool_Base::ActivateMemory(); }

	inline void DeactivateMemory(void* reset) { _Pool_Base::DeactivateMemory(reset); }

	/// <summary>
	/// Gets the active count.
	/// </summary>
	/// <returns>size_t.</returns>
	inline size_t GetActiveCount() const { return activeList.size(); }

	/// <summary>
	/// Gets the active list.
	/// </summary>
	/// <returns>std.vector&lt;T*&gt;*.</returns>
	inline std::vector<T*>* GetActiveList() const { return (std::vector<T*>*)&activeList; }
};
