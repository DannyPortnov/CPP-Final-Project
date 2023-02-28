//#include "MethodMap.h"
//
//template <typename... Args, typename RT, typename T>
//void MethodMap::Insert(std::string key, RT(T::* method)(Args...)) {
//	std::unique_ptr<member_base_ptr> ptr = std::make_unique<member_ptr<T, RT, Args ...>>(method);
//	m_method_map.emplace(key, std::move(ptr));
//}
//
//void MethodMap::Clear() {
//	m_method_map.clear();
//}
//
//template <typename RT, typename T, typename... Args>
//RT MethodMap::Call(const std::string& key, T* instance, Args&&... methodArgs) const {
//	auto it = m_method_map.find(key);
//	if (it != m_method_map.end()) {
//		member_base_ptr* base_ptr = it->second.get();
//		auto test = dynamic_cast<member_ptr<T, RT, Args...> *>(base_ptr);
//		if (test == nullptr) {
//			throw std::runtime_error("casting failed");
//		}
//		return test->call(instance, std::forward<Args>(methodArgs)...);
//	}
//	throw std::runtime_error("not found");
//}