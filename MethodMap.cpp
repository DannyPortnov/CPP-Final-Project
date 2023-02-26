//#include "MethodMap.h"
//
//template <typename T, typename... Args>
//void MethodMap::Insert(const std::string& key, void (T::* method)(Args...)) {
//    method_map[key] = [method](T* obj, Args... args) { (obj->*method)(args...); };
//
//   /* method_map[key] = [methodName, methodArgs...]() {
//        (methodName)(methodArgs...);
//    };*/
//}
//
//template <typename T, typename... Args>
//void MethodMap::Call(const std::string& key, T* instance, Args&&... methodArgs) const{
//    auto it = method_map.find(key);
//    if (it != method_map.end()) {
//        auto& func = it->second;
//        // use tuple to store and forward the arguments
//        std::tuple<Args...> arg_tuple(std::forward<Args>(methodArgs)...);
//        std::apply(func, std::tuple_cat(std::make_tuple(instance), arg_tuple));
//        return;
//    }
//    std::cerr << "Error: method '" << key << "' not found" << std::endl;
//}
//
////template<typename MethodPtr>
////void MethodMap::Insert(const std::string& key, MethodPtr method) {
////    auto type_index = std::type_index(typeid(method));
////    m_method_map.emplace(key, make_pair((voidFunctionType)method, type_index));
////    //method_map[key] = [method](T* obj) { (obj->*method)(); };
////
////    //static_assert(is_invocable_v<MethodPtr> || is_invocable_v<MethodPtr, const std::string&>, "Method must be callable with either no parameters or one std::string parameter.");
////    ////we store it in the std::variant using the std::variant constructor
////    ////that takes the value as an argument.
////    //if constexpr (is_invocable_v<MethodPtr>) {
////    //    method_map[key] = variant<voidFunctionType, voidstd::stringFunctionType>{ reinterpret_cast<voidFunctionType>(method) };
////    //}
////    //else {
////    //    method_map[key] = variant<voidFunctionType, voidstd::stringFunctionType>{ reinterpret_cast<voidstd::stringFunctionType>(method) };
////    //}
////}
////
////template<typename T, typename... Args>
////void MethodMap::Get_Method(const std::string& key, T* obj, Args&&... args) const {
////
////    auto it = m_method_map.find(key);
////    if (it != m_method_map.end()) {
////        auto& mapVal = it->second;
////        auto& func = (T(*)(Args ...))(mapVal.first);
////        func(obj, std::forward<Args>(args)...);
////    }
////    else {
////        cerr << "Error: method '" << key << "' not found" << std::endl;
////    }
////
////    //static_assert(is_invocable_v<MethodPtr> || is_invocable_v<MethodPtr, const std::string&>,
////    //    "Method must be callable with either no parameters or one std::string parameter.");
////    //// we use std::get_if to check if the value in the std::variant
////    //// is of the correct type for the requested method pointer. 
////    ////If it is, we return a pointer to the method. Otherwise, we return nullptr.
////    //auto iter = method_map.find(key);
////    //if (iter != method_map.end()) {
////    //    auto method = get_if<MethodPtr>(&iter->second);
////    //    if (method != nullptr) {
////    //        return *method;
////    //    }
////    //}
////
////    //return nullptr;
////}
