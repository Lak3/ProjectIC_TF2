#pragma once

//std::unique_ptr
#include <memory>

//Written by a pleb
namespace VMT
{
	class Table
	{
	private:
		unsigned int **baseclass = nullptr;
		std::unique_ptr<unsigned int[]> current;
		unsigned int *original = nullptr;
		int size = 0;

	public:
		void Init(const void *base)
		{
			baseclass = (unsigned int **)(base);

			if (size == 0) {
				while (static_cast<unsigned int *>(*baseclass)[size])
					size++;
			}

			original = *baseclass;
			current = std::make_unique<unsigned int[]>(size);
			std::memcpy(current.get(), original, size * sizeof(unsigned int));
			*baseclass = current.get();
		};

		template <typename FN>
		inline FN Original(const int index)	{ return reinterpret_cast<FN>(original[index]); }
		inline void Hook(const int index, void *function)	{ current[index] = reinterpret_cast<unsigned int>(function); }
		inline void Unhook(const int index)					{ current[index] = original[index]; }
		inline bool Initialized()                           { return size && original != nullptr; }
		inline void RestoreTable()                          { if (Initialized()) *baseclass = original; }
	};
}