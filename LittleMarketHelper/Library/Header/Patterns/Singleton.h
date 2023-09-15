
// ========================================================================
//		Template for Singleton pattern
// ========================================================================

#pragma once

#include <type_traits>


namespace lmh {

	//
	// Usage:
	// 
	// class Example : public Singleton<Example>
	// {
	//		friend class Singleton<Example>;
	// 
	//	private:
	//		Example() {}
	//		...
	// 
	//	public:
	//		...
	// };

	template<class Type>
	class Singleton
	{
	public:

		Singleton(const Singleton&) = delete;				// Disable copy operator
		Singleton(Singleton&&) = delete;					// Disable move operator
		Singleton& operator=(const Singleton&) = delete;	// Disable copy assignment operator
		Singleton& operator=(Singleton&&) = delete;			// Disable move assignment operator
		virtual ~Singleton() = default;

		inline static Type& instance()
		{
			static Type instance;
			return instance;
		}

	protected:

		Singleton() = default;
	};

}