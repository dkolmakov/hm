/*
   Copyright 2019 Dmitry Kolmakov

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once

#include <vector>
#include <initializer_list>
#include <string>
#include <unordered_map>

#include "query.hpp"
#include "utils.hpp"

struct SchemaException : UtilException {
    SchemaException(const std::string& _reason) :
        UtilException("History manager error ", _reason) {}
};

namespace schema {

struct Schema {
    const std::string table_name;
    
    class ColumnBase {
        mutable int pos;
    public:
        const std::string name;
        const std::string sqlType;
        const std::string bName;

        int getPos() const { return pos; }
        void setPos(const int p) const { pos = p; }
        
        ColumnBase(const std::string& _name, const std::string& _sqlType) 
            : name(_name), sqlType(_sqlType), bName(":" + name) {}
        
        virtual ~ColumnBase() = default;
    };

    template<typename T>
    struct Column : public ColumnBase {
        Column(const std::string& _name, const std::string& _sqlType) noexcept 
            : ColumnBase(_name, _sqlType) {}
        
        void safeBind(const sqlite::Query& q, T value) const {
            q.bind(bName, value);
        }
    };
    
    void doForAll(const std::function<void(const ColumnBase&)>&) const {}
    
    template<typename T, typename ...Others>
    void doForAll(const std::function<void(const ColumnBase&)>& lambda, const Column<T>& value, const Column<Others>&... others) const {
        lambda(value);
        doForAll(lambda, others...);
    }

    template<typename ...Others>
    std::string getAnyList(const std::function<std::string(const ColumnBase&)>& getVal, const Column<Others>&... others) const {
        std::string result;
        
        doForAll([&result, &getVal](const ColumnBase& value) {
                    result += getVal(value);
                    result += ",";
                 }, others...);
        
        result.pop_back();
        return result;
    }

    template<typename ...Others>
    std::string getList(const Column<Others>&... others) const {
        return getAnyList([](const ColumnBase& v) {return v.name;}, others...);
    }

    template<typename ...Others>
    std::string getBindingsList(const Column<Others>&... others) const {
        return getAnyList([](const ColumnBase& v) {return ":" + v.name;}, others...);
    }

    template<typename ...Others>
    std::string getListWithTypes(const Column<Others>&... others) const {
        return getAnyList([](const ColumnBase& v) {return v.name + " " + v.sqlType;}, others...);
    }
    
    template<typename ...Others>
    int setPositions(const Column<Others>&... others) const {
        int pos = 0;
        
        doForAll([&pos](const ColumnBase& value) {
                    value.setPos(pos);
                    ++pos;
                 }, others...);
        
        return pos;
    }
    
    Schema(const std::string& _name) : table_name(_name) {} 
    
};

// class Schema {
//     const std::vector<std::string> schema;
//     std::unordered_map<std::string, int> fieldToPos;
// 
// public:
//     
//     const std::string fieldsList;
//     const std::string bindFieldsList;
//     
//     template<typename ...Args>
//     Schema(const std::initializer_list<Field<Args...>>& list) : 
//         schema(list), fieldsList(getFieldsList(false)), bindFieldsList(getFieldsList(true)) 
//     {
//         for (size_t i = 0; i < schema.size(); ++i) {
//             fieldToPos[schema[i]] = i;
//         }
//     }
//     
//     std::string getFieldsList(const bool bindable, const char separator = ',') const {
//         std::string list;
//         
//         for (const auto& field : schema) {
//             if (bindable)
//                 list += ":";
//             
//             list += field + separator;
//         }
//         
//         list.pop_back();
//         return list;
//     }
//     
//     int getFieldPos(const std::string& field) {
//         return fieldToPos[field];
//     }
//     
// };

}
