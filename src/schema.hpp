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

#include <string>

#include "query.hpp"
#include "utils.hpp"

namespace schema {

struct Schema {
    const std::string table_name;
    
    Schema(const std::string& _name) noexcept : table_name(_name) {} 
    
    
    class ColumnBase {
        mutable int pos;
        void setPos(const int p) const { pos = p; }
        friend Schema;
    public:
        const std::string name;
        const std::string sqlType;
        const std::string bName;
        const std::string binding;

        int getPos() const { return pos; }
        
        ColumnBase(const std::string& _name, const std::string& _sqlType) noexcept
            : name(_name), sqlType(_sqlType), bName(":" + name), binding(name + " = " + bName) {}
        
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
    
    static void doForAll(const std::function<void(const ColumnBase&)>&) {}
    
    template<typename T, typename ...Others>
    static void doForAll(const std::function<void(const ColumnBase&)>& lambda, const Column<T>& value, const Column<Others>&... others) {
        lambda(value);
        doForAll(lambda, others...);
    }

    template<typename ...Others>
    static std::string getAnyList(const std::function<std::string(const ColumnBase&)>& getVal, const Column<Others>&... others) {
        std::string result;
        
        doForAll([&result, &getVal](const ColumnBase& value) {
                    result += getVal(value);
                    result += ",";
                 }, others...);
        
        result.pop_back();
        return result;
    }

    template<typename ...Others>
    static std::string getList(const Column<Others>&... others) {
        return getAnyList([](const ColumnBase& v) {return v.name;}, others...);
    }

    template<typename ...Others>
    static std::string getBindingsList(const Column<Others>&... others) {
        return getAnyList([](const ColumnBase& v) {return ":" + v.name;}, others...);
    }

    template<typename ...Others>
    static std::string getListWithTypes(const Column<Others>&... others) {
        return getAnyList([](const ColumnBase& v) {return v.name + " " + v.sqlType;}, others...);
    }
    
    template<typename ...Others>
    static int setPositions(const Column<Others>&... others) {
        int pos = 0;
        
        doForAll([&pos](const ColumnBase& value) {
                    value.setPos(pos);
                    ++pos;
                 }, others...);
        
        return pos;
    }
};

} // End of schema namespace

