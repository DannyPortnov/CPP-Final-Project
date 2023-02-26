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

//template <typename T, typename... Args>
class MethodMap {
private:
	std::unordered_map<std::string, std::unique_ptr<member_base_ptr>> m_method_map;
public:
	//template <typename T>
	void Insert(std::string key, auto type) {
		std::unique_ptr<member_base_ptr> ptr = std::make_unique<decltype(member_ptr(type))>(type);
		m_method_map.insert(std::make_pair(key, std::move(ptr)));

		//m_method_map[key] = [method](T* obj, Args... args) { (obj->*method)(args...); }; //WORKS
		//method_map[key] = [method](T* obj, Args... args) { (*method)(obj, args...); };
		/* method_map[key] = [methodName, methodArgs...]() {
			 (methodName)(methodArgs...);
		 };*/
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

	////template <typename T, typename... Args>
	//void Call(const std::string& key, T* instance, Args&&... methodArgs) const {
	//	auto it = m_method_map.find(key);
	//	if (it != m_method_map.end()) {
	//		auto& func = it->second;
	//		// use tuple to store and forward the arguments
	//		std::tuple<Args...> arg_tuple(std::forward<Args>(methodArgs)...);
	//		std::apply(func, std::tuple_cat(std::make_tuple(instance), arg_tuple));
	//		return;
	//	}
	//	std::cerr << "Error: method '" << key << "' not found" << std::endl;
	//}

};
