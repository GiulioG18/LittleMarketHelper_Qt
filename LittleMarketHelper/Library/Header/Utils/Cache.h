

// ========================================================================
//		Cache
// ========================================================================

#pragma once

#include <unordered_map>
#include <queue>
#include <memory>


namespace lmh {

	// Interface
	template<typename Key, typename Value>
	class Cache
	{
	public:

		using KeyValue = std::pair<Key, Value>;
		using ValueStored = std::shared_ptr<Value>;
		using ValueReturned = std::shared_ptr<const Value>;

	public:

		Cache(size_t maxSize = std::numeric_limits<size_t>().max);

		virtual bool has(const Key& key) const;

		// [ MAY THROW ]
		virtual ValueReturned get(const Key& key) const = 0;
		//virtual bool put(KeyValue keyValue) = 0;

	private:

		void makeSpace();

	protected:

		std::unordered_map<Key, ValueStored> cached_;
		const size_t size_;
	};


	// First in, first out
	template<typename Key, typename Value>
	class FifoCache : public Cache<Key, Value>
	{
	public:

		FifoCache(size_t size);
	
		virtual Cache<Key, Value>::ValueReturned get(const Key& key) const override;
		//virtual bool put(Cache<Key, Value>::KeyValue keyValue) override;

	private:

		std::queue<Key*> keys_;
	};


	// Implementation

	// Base

	template<typename Key, typename Value>
	inline Cache<Key, Value>::Cache(size_t size)
		: size_(size)
	{
	}

	template<typename Key, typename Value>
	inline bool Cache<Key, Value>::has(const Key& key) const
	{
		return cached_.contains(key);
	}

	template<typename Key, typename Value>
	inline void Cache<Key, Value>::makeSpace()
	{
		if (cached_.size() >= size_)
		{
			// TODO: impl
		}
	}


	// Fifo

	template<typename Key, typename Value>
	inline FifoCache<Key, Value>::FifoCache(size_t size)
		: Cache<Key, Value>(size)
	{
	}

	template<typename Key, typename Value>
	inline Cache<Key, Value>::ValueReturned FifoCache<Key, Value>::get(const Key& key) const
	{
		auto it = Cache<Key, Value>::cached_.find(key);
		if (it == Cache<Key, Value>::cached_.end())
			return nullptr;
			
		return it->second;
	}

	//template<typename Key, typename Value>
	//inline bool FifoCache<Key, Value>::put(Cache<Key, Value>::KeyValue keyValue)
	//{
	//	// There is already a key with that name
	//	if (Cache<Key, Value>::has(keyValue.first))
	//	{

	//	}

	//	// 1. [HAS FUNC] check if key is in the queue. if so, update the value in the cache
	//	// 2. else...[MAKE SPACE FUNC --->] check size, if full, delete oldest (check keys queue) element (from both cache and queue)
	//	// 3. add key to queue, and the pair to the cache
	//	// 3. return true if element was added, false if the element has not been added

	//	return true;
	//}

}
