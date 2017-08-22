#ifndef TIGERS_UTILS_H_
#define TIGERS_UTILS_H_

#include <memory>

namespace tigers {

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>{new T(std::forward<Args>(args)...)};
}
  
} /* tigers  */ 

#endif /* ifndef TIGERS_UTILS_H_ */
