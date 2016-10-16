/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2015 Tectiform Inc.

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
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/

#include "tfdata.hpp"

#define NEEDS_CSTRING
#include "tfheaders.hpp"
#include "tfformatter.hpp"

namespace TF
{

	namespace Foundation
	{

		Data::Chunk::Chunk(const char *c, size_type length)
		{
			theBuffer = nullptr;
			theLength = 0;
			if(c != nullptr && length > 0)
			{
				theBuffer = new char[length];
				std::memcpy(reinterpret_cast<void *>(theBuffer),
					reinterpret_cast<void *>(const_cast<char *>(c)),
					length * sizeof(char));
				theLength = length;
			}
		}


		Data::Chunk::Chunk(const Chunk &c)
		{
			theBuffer = nullptr;
			theLength = 0;
			if(c.theBuffer != nullptr && c.theLength > 0)
			{
				theBuffer = new char[c.theLength];
				std::memcpy(reinterpret_cast<void *>(theBuffer),
					reinterpret_cast<void *>(const_cast<char *>(c.theBuffer)),
					c.theLength * sizeof(char));
				theLength = c.theLength;
			}
		}


		Data::Chunk::Chunk(const Chunk &&c)
		{
			Chunk &other = const_cast<Chunk &>(c);
			theBuffer = c.theBuffer;
			theLength = c.theLength;
			other.theBuffer = nullptr;
			other.theLength = 0;
		}



		Data::Chunk& Data::Chunk::operator=(const Chunk &c)
		{
			if(this != &c)
			{
				if(theBuffer != nullptr && theLength != 0)
				{
					delete[] theBuffer;
					theBuffer = nullptr;
					theLength = 0;
				}

				if(c.theBuffer != nullptr && c.theLength != 0)
				{
					theBuffer = new char[c.theLength];
					std::memcpy(reinterpret_cast<void *>(theBuffer),
						reinterpret_cast<void *>(const_cast<char *>(c.theBuffer)),
						c.theLength * sizeof(char));
					theLength = c.theLength;
				}
			}

			return *this;
		}


		Data::Chunk& Data::Chunk::operator=(const Chunk &&c)
		{
			if(this != &c)
			{
				if(theBuffer != nullptr && theLength != 0)
				{
					delete[] theBuffer;
					theBuffer = nullptr;
					theLength = 0;
				}

				if(c.theBuffer != nullptr && c.theLength != 0)
				{
					Chunk &other = const_cast<Chunk &>(c);
					theBuffer = c.theBuffer;
					theLength = c.theLength;
					other.theBuffer = nullptr;
					other.theLength = 0;
				}
			}

			return *this;
		}


		bool Data::Chunk::operator==(const Chunk &c) const
		{
			if(this != &c)
			{
				if(theLength != c.theLength)
					return false;

				for(size_type i = 0; i < theLength; i++)
				{
					if(*(theBuffer + i) != *(c.theBuffer + i))
						return false;
				}
			}

			return true;
		}


		bool Data::Chunk::operator!=(const Chunk &c) const
		{
			if(*this == c)
				return false;
			return true;
		}


		std::ostream& Data::Chunk::description(std::ostream &o) const
		{
			ClassFormatter *formatter = FormatterFactory::getFormatter();
			if(formatter != nullptr)
			{
				formatter->setClassName("Chunk");
				formatter->addClassMember("char *", "theBuffer", theBuffer, theLength);
				formatter->addClassMember("Size_t", "theLength", theLength);
				o << *formatter;
				delete formatter;
			}
			return o;
		}



		Data::Data(const char *bytes, size_type length)
		{
			chunk_type *newChunk = new chunk_type(bytes, length);
			chunkList.emplace_back(newChunk);
			theLength = length;
		}


		Data::Data(const Data &d)
		{
			theLength = 0;
			if(d.length() != 0)
			{
				chunk_type *newChunk = new chunk_type(d.bytes(), d.length());
				chunkList.emplace_back(newChunk);
				theLength = d.length();
			}
		}


		Data::Data(const Data &&d)
		{
			theLength = 0;
			if(d.theLength > 0)
			{
				chunkList = d.chunkList;
				theLength = d.theLength;
			}
		}


		Data::~Data()
		{
			if(theLength > 0)
			{
				for(auto chunk : chunkList)
				{
					delete chunk;
				}

				chunkList.clear();
			}
		}


		Data& Data::operator=(const Data &d)
		{
			if(this != &d)
			{
				if(theLength > 0)
				{
					for(auto chunk : chunkList)
					{
						delete chunk;
					}
					chunkList.clear();

					theLength = 0;
				}

				if(d.length() > 0)
				{
					chunk_type *newChunk = new chunk_type(d.bytes(), d.length());
					chunkList.emplace_back(newChunk);
					theLength = d.length();
				}
			}

			return *this;
		}


		Data& Data::operator=(const Data &&d)
		{
			if(this != &d)
			{
				if(theLength > 0)
				{
					for(auto chunk : chunkList)
						delete chunk;

					chunkList.clear();

					theLength = 0;
				}

				if(d.theLength > 0)
				{
					chunkList = d.chunkList;
					theLength = d.theLength;
				}
			}

			return *this;
		}


		bool Data::operator==(const Data &d) const
		{
			if(this != &d)
			{
				if(theLength != d.length())
					return false;

				{
					auto myBytes = this->bytes();
					auto hisBytes = d.bytes();

					for(size_type i = 0; i < theLength; i++)
					{
						if(*(myBytes + i) != *(hisBytes + i))
							return false;
					}
				}
			}

			return true;
		}


		bool Data::operator!=(const Data &d) const
		{
			if(*this == d)
				return false;
			return true;
		}



		Data::byte_type Data::operator[](size_type i) const
		{
			const char *theBytes = this->bytes();
			return theBytes[i];
		}


		Data::size_type Data::length() const
		{
			return theLength;
		}


		const char * Data::bytes() const
		{
			if(chunkList.size() == 0 && theLength == 0)
				return nullptr;

			if(chunkList.size() == 1)
			{
				auto chunk = chunkList.front();
				return chunk->bytes();
			}

			// We have more than one chunk in the list, so coalesce the list
			// into one big chunk.

			char *buffer = new char[theLength];
			char *tmp = buffer;

			Data &other = const_cast<Data &>(*this);

			for(auto chunk : chunkList)
			{
				std::memcpy(reinterpret_cast<void *>(tmp),
					reinterpret_cast<void *>(const_cast<char *>(chunk->bytes())),
					chunk->length() * sizeof(char));
				tmp += chunk->length() * sizeof(char);
				delete chunk;
			}

			other.chunkList.clear();

			chunk_type *newChunk = new chunk_type(buffer, theLength);
			other.chunkList.emplace_back(newChunk);

            delete[] buffer;

			return newChunk->bytes();
		}


		void Data::append(const char *bytes, size_type length)
		{
			if(bytes != nullptr && length != 0)
			{
				chunk_type *newChunk = new chunk_type(bytes, length);
				chunkList.emplace_back(newChunk);
				theLength += length;
			}
		}

		void Data::append(const Data &d)
		{
			if(d.length() > 0)
			{
				chunk_type *newChunk = new chunk_type(d.bytes(), d.length());
				chunkList.emplace_back(newChunk);
				theLength += d.length();
			}
		}


		void Data::append(const Data &&d)
		{
			if(d.length() > 0)
			{
				for(auto chunk : d.chunkList)
				{
					chunkList.emplace_back(chunk);
				}

				theLength += d.length();

				Data &other = const_cast<Data &>(d);
				other.chunkList.clear();
				other.theLength = 0;
			}
		}


		void Data::prepend(const char *bytes, size_type length)
		{
			if(bytes != nullptr && length > 0)
			{
				chunk_type *newChunk = new chunk_type(bytes, length);
				chunkList.emplace_front(newChunk);
				theLength += length;
			}
		}


		void Data::prepend(const Data &d)
		{
			if(d.length() > 0)
			{
				chunk_type *newChunk = new chunk_type(d.bytes(), d.length());
				chunkList.emplace_front(newChunk);
				theLength += d.length();
			}
		}


		void Data::prepend(const Data &&d)
		{
			if(d.length() > 0)
			{
				chunk_list_type::reverse_iterator riter;
				for(auto riter = d.chunkList.rbegin(); riter != d.chunkList.rend(); riter++)
				{
					chunkList.emplace_front(*riter);
					theLength += (*riter)->length();
				}

				Data &other = const_cast<Data &>(d);
				other.chunkList.clear();
				other.theLength = 0;
			}
		}


		std::ostream& Data::description(std::ostream &o) const
		{
			ClassFormatter *formatter = FormatterFactory::getFormatter();
			if(formatter != nullptr)
			{
				formatter->setClassName("Data");
				formatter->addClassMember("chunk_list_type", "chunkList", chunkList);
				formatter->addClassMember("Size_t", "theLength", theLength);
				o << *formatter;
				delete formatter;
			}
			return o;
		}


		std::ostream& operator<<(std::ostream &o, const Data::Chunk &c)
		{
			return c.description(o);
		}


		std::ostream& operator<<(std::ostream &o, const Data &d)
		{
			return d.description(o);
		}


	} // Foundation


} // TF
