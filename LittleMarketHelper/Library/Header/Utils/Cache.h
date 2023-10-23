

// ========================================================================
//		Cache
// ========================================================================

#pragma once

#include <unordered_map>
#include <queue>


namespace lmh {

	// Interface
	template<typename Key, typename Value>
	class Cache
	{
	public:

		using KeyValue = std::pair<Key, Value>;
		using Iter = std::unordered_map<Key, Value>::iterator;

	public:

		Cache(size_t maxSize = std::numeric_limits<size_t>().max);

		virtual Iter read(const Key& key) = 0;
		virtual Value write(KeyValue keyValue) = 0;
		virtual bool has(Key key) const;

	protected:

		std::unordered_map<Key, Value> cached_;
		size_t size_;
	};


	// First in, first out
	template<typename Key, typename Value>
	class FIFOCache : public Cache<Key, Value>
	{
	public:

		using KeyValue = Cache<Key, Value>::KeyValue;
		using Iter = Cache<Key, Value>::Iter;

	public:

		FIFOCache(size_t size);
	
		virtual Iter read(const Key& key) override;
		virtual Value write(KeyValue keyValue) override;

	private:

		std::queue<Key*> queue_;
	};


	// TODO: implement
	// Least recently used
	//template<typename Key, typename Value>
	//class LruCache : public Cache<Key, Value>
	//{
	//public:

	//	LruCache(size_t size);

	//	using KeyValue = Cache<Key, Value>::KeyValue;
	//	using Iter = Cache<Key, Value>::Iter;

	//public:

	//	virtual Iter read(const Key& key) override;
	//	virtual void write(KeyValue keyValue) override;
	//};



	// Implementation

	template<typename Key, typename Value>
	inline FIFOCache<Key, Value>::FIFOCache(size_t size)
		: Cache<Key, Value>(size)
	{
	}

	template<typename Key, typename Value>
	inline FIFOCache<Key, Value>::Iter FIFOCache<Key, Value>::read(const Key& key)
	{
		if (!Cache<Key, Value>::has(key))
			return Cache<Key, Value>::cached_.end();
	}

	template<typename Key, typename Value>
	inline Value FIFOCache<Key, Value>::write(KeyValue keyValue)
	{
		return Value();
	}

	template<typename Key, typename Value>
	inline Cache<Key, Value>::Cache(size_t size)
		: size_(size)
	{
	}

	template<typename Key, typename Value>
	inline bool Cache<Key, Value>::has(Key key) const
	{
		return cached_.find(key) != cached_.end();
	}

}
