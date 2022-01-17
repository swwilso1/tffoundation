/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2015 to 2022 Tectiform Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
SOFTWARE.


******************************************************************************/

#include "tfformatter.hpp"

namespace TF
{

    namespace Foundation
    {

        template<typename K>
        SymbolTable<K>::SymbolTable(const SymbolTable &t)
        {
            for(auto pair : t.m_theTable)
            {
                m_theTable.insert(pair);
            }
        }


        template<typename K>
        SymbolTable<K>::~SymbolTable()
        {
            m_theTable.clear();
        }

        template<typename K>
        SymbolTable<K> &SymbolTable<K>::operator=(const SymbolTable &t)
        {
            if(this != &t)
            {
                m_theTable.clear();

                for(auto pair : t.m_theTable)
                {
                    m_theTable.insert(pair);
                }
            }

            return *this;
        }


        template<typename K>
        bool SymbolTable<K>::hasValueForKey(const key_type &k) const
        {
            if(m_theTable.count(k) > 0)
                return true;
            return false;
        }

        template<typename K>
        template<typename T>
        void SymbolTable<K>::setValueForKey(const key_type &k, const T &value)
        {
            auto theSymbol = std::make_shared<SpecializedSymbol<T>>();
            theSymbol->m_value = value;
            m_theTable.insert(std::make_pair(k, theSymbol));
        }

        template<typename K>
        template<typename T>
        bool SymbolTable<K>::getValueForKey(const key_type &k, T &value)
        {
            auto theSymbol = m_theTable[k];
            if(theSymbol != nullptr)
            {
                SpecializedSymbol<T> *theSpecialSymbol = dynamic_cast<SpecializedSymbol<T> *>(theSymbol.get());
                if(theSpecialSymbol != nullptr)
                {
                    value = theSpecialSymbol->m_value;
                    return true;
                }
            }
            return false;
        }

        template<typename K>
        template<typename T>
        T SymbolTable<K>::getValueForKey(const key_type &k)
        {
            auto theSymbol = m_theTable[k];
            if(theSymbol != nullptr)
            {
                SpecializedSymbol<T> *theSpecialSymbol = dynamic_cast<SpecializedSymbol<T> *>(theSymbol.get());
                if(theSpecialSymbol != nullptr)
                {
                    return theSpecialSymbol->m_value;
                }

                throw std::runtime_error("Symbol table does not contain a symbol of the requested type");
            }

            throw std::runtime_error("Symbol table does not contain a value for the key");
        }


        template<typename K>
        void SymbolTable<K>::removeValueForKey(const key_type &k)
        {
            m_theTable.erase(k);
        }

        template<typename K>
        void SymbolTable<K>::clear()
        {
            m_theTable.clear();
        }

        template<typename K>
        typename SymbolTable<K>::key_list_type SymbolTable<K>::keys() const
        {
            key_list_type theKeys;
            for(auto &entry : m_theTable)
            {
                theKeys.push_back(entry.first);
            }
            return theKeys;
        }

        template<typename K>
        void SymbolTable<K>::update(const SymbolTable &t)
        {
            for(auto &entry : t.m_theTable)
            {
                m_theTable[entry.first] = entry.second;
            }
        }


        template<typename K>
        std::ostream &SymbolTable<K>::description(std::ostream &o) const
        {
            ClassFormatter *formatter = FormatterFactory::getFormatter();
            if(formatter != nullptr)
            {
                formatter->setClassName("SymbolTable");
                formatter->addClassMember<map_type>("m_theTable", m_theTable);
                o << *formatter;
                delete formatter;
            }
            return o;
        }

        template<typename K>
        std::ostream &operator<<(std::ostream &o, const SymbolTable<K> t)
        {
            return t.description(o);
        }

    }    // namespace Foundation

}    // namespace TF
