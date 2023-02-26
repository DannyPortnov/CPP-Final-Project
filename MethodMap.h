#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <map>
#include <cassert>

struct member_base_ptr {
	virtual ~member_base_ptr() = default;
};

template <typename T, typename RT, typename... Args>
struct member_ptr : public member_base_ptr {

	std::function<RT(T*, Args...)> m_ptr;

	member_ptr(RT(T::* method)(Args...)) {
		m_ptr = [method](T* obj, Args... args) { return (obj->*method)(args...); };
	}

	RT call(T* instance, Args&&... methodArgs) const {
		return m_ptr(instance, std::forward<Args>(methodArgs)...);
	}
};

class MethodMap {
private:
	std::unordered_map<std::string, std::unique_ptr<member_base_ptr>> m_method_map;
public:
	template <typename... Args, typename RT, typename T>
	void Insert(std::string key, RT(T::* method)(Args...)) {
		std::unique_ptr<member_base_ptr> ptr = std::make_unique<member_ptr<T, RT, Args ...>>(method);
		m_method_map.insert(std::make_pair(key, std::move(ptr)));
	}

	template <typename RT, typename T, typename... Args>
	RT Call(const std::string& key, T* instance, Args&&... methodArgs) const {
		auto it = m_method_map.find(key);
		if (it != m_method_map.end()) {
			member_base_ptr* base_ptr = it->second.get();
			auto test = dynamic_cast<member_ptr<T, RT, Args...> *>(base_ptr);
			if (test == nullptr) {
				throw std::runtime_error("casting failed");
			}
			return test->call(instance, std::forward<Args>(methodArgs)...);
		}
		throw std::runtime_error("not found");
	}
};
