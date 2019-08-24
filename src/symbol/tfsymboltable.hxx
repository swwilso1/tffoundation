/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2019 Tectiform Inc.

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

#ifndef TFSYMBOLTABLE_HXX
#define TFSYMBOLTABLE_HXX

#define NEEDS_OSTREAM
#define NEEDS_UNORDERED_MAP
#define NEEDS_LIST
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"

namespace TF
{

    namespace Foundation
    {

        /**
         * @brief helper class for SymbolTable class.
         */
        class Symbol
        {
        public:
            virtual ~Symbol()
            {
            }
        };

        /**
         * @brief helper class for SymbolTable class
         * @tparam T the type of object stored in the symbol
         */
        template<typename T>
        class SpecializedSymbol : public Symbol
        {
        public:
            using symbol_type = T;
            SpecializedSymbol()
            {
            }
            symbol_type m_value;
        };


        /**
         * @brief Templated symbol table class.
         * @tparam K the type of the symbol table key.  As of this writing the type must
         * be hashable by std::hash.
         */
        template<typename K>
        class SymbolTable : public AllocatorInterface
        {
        public:
            /** @brief the key type */
            using key_type = K;

            /** @brief size type */
            using size_type = Size_t;

            /** @brief the type used for lists of keys */
            using key_list_type = std::list<key_type>;

            /** @brief simple default constructor */
            SymbolTable()
            {
            }

            /** @brief destructor */
            ~SymbolTable();

            /**
             * @brief method to see if the symbol table has a value for the given key
             * @param k the key
             * @return true if the symbol table has a value for @e k and false otherwise
             */
            bool hasValueForKey(const key_type &k) const;

            /**
             * @brief method to set a value for a key
             * @tparam T the type of the value
             * @param k the key
             * @param value the value to set.
             */
            template<typename T>
            void setValueForKey(const key_type &k, const T &value);

            /**
             * @brief method to get a value for a key
             * @tparam T the type of the value
             * @param k the key
             * @param value a reference to a @e T object.  When the method
             * returns value will contain the stored value.
             */
            template<typename T>
            bool getValueForKey(const key_type &k, T &value);

            /**
             * @brief method to remove a key/value pair from the table
             * @param k the key to remove from the table.
             */
            void removeValueForKey(const key_type &k);

            /**
             * @brief method to remove all key/value pairs from the table.
             */
            void clear();

            /**
             * @brief method that returns the number of key/value pairs in the table.
             * @return the number of key/value pairs in the table;
             */
            size_type size() const
            {
                return m_theTable.size();
            }

            /**
             * @brief method to get a list of keys in use in the table.
             * @return a list of keys
             */
            key_list_type keys() const;

            /**
             * @brief method to write the contents of the table to a stream object
             * @param o the stream object
             * @return @e o the stream object
             */
            std::ostream &description(std::ostream &o) const;

        private:
            /** @brief the type stored in the symbol table */
            using symbol_type = Symbol;

            /** @brief the type of the internal table */
            using map_type = std::unordered_map<key_type, symbol_type *>;

            /** @brief the internal table object */
            map_type m_theTable;
        };

        /**
         * @brief method to write a SymbolTable to a stream.
         * @tparam K the type of key used by the SymbolTable
         * @param o the stream object.
         * @param t the SymbolTable object
         * @return @e o the stream object.
         */
        template<typename K>
        std::ostream &operator<<(std::ostream &o, const SymbolTable<K> t);

    }    // namespace Foundation

}    // namespace TF

#include "tfsymboltable.cxx"

#endif    // TFSYMBOLTABLE_HXX
