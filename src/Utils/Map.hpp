#ifndef GP__MAP_HPP_
#define GP__MAP_HPP_

#include "Utils/HashFn.hpp"
#include <unordered_map>

#define UseMap(Key, Data)  std::unordered_map<Key, Data, HashStruct<Key> >


#endif
    
    
    
    







