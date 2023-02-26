#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <map>
#include <cassert>

template <typename T, typename... Args>
class MethodMap {
private:
	std::map<std::string, std::function<void(T*, Args...)>> method_map;
public:
	template <typename T, typename... Args>
	void Insert(const std::string& key, void (T::* method)(Args...)) {
		method_map[key] = [method](T* obj, Args... args) { (obj->*method)(args...); };

		/* method_map[key] = [methodName, methodArgs...]() {
			 (methodName)(methodArgs...);
		 };*/
	}

	template <typename T, typename... Args>
	void Call(const std::string& key, T* instance, Args&&... methodArgs) const {
		auto it = method_map.find(key);
		if (it != method_map.end()) {
			auto& func = it->second;
			// use tuple to store and forward the arguments
			std::tuple<Args...> arg_tuple(std::forward<Args>(methodArgs)...);
			std::apply(func, std::tuple_cat(std::make_tuple(instance), arg_tuple));
			return;
		}
		std::cerr << "Error: method '" << key << "' not found" << std::endl;
	}

};
