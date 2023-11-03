

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

		using ValuePtr = std::shared_ptr<Value>;
		using ConstValuePtr = std::shared_ptr<const Value>;
		using Iterator = std::unordered_map<Key, ValuePtr>::iterator;

	public:

		Cache(size_t maxSize);
		virtual ~Cache() = default;

		// Returns a pointer pointing to an element in the cache, if not found, returns nullptr
		ConstValuePtr get(const Key& key) const;
		// Inserts (or updates if already there) an element in the cache
		template<typename K, typename V> 
		void put(K&& key, V&& value);


		bool has(const Key& key) const;

	private:

		virtual void replacementPolicy(const Iterator& element, bool inserted) = 0;

	protected:

		std::unordered_map<Key, ValuePtr> cached_;
		const size_t maxSize_;
	};


	// First in, first out
	template<typename Key, typename Value>
	class FifoCache : public Cache<Key, Value>
	{
	public:

		using ValuePtr = Cache<Key, Value>::ValuePtr;
		using ConstValuePtr = Cache<Key, Value>::ConstValuePtr;
		using Iterator = Cache<Key, Value>::Iterator;

	public:
		
		FifoCache(size_t maxSize = std::numeric_limits<size_t>().max());
		virtual ~FifoCache() = default;

	private:

		virtual void replacementPolicy(const Iterator& element, bool inserted) override;

	private:

		std::queue<Key*> keys_;
	};





	// Implementation

	// Base

	template<typename Key, typename Value>
	inline Cache<Key, Value>::Cache(size_t maxSize)
		: maxSize_(maxSize)
	{
	}

	template<typename Key, typename Value>
	inline Cache<Key, Value>::ConstValuePtr Cache<Key, Value>::get(const Key& key) const
	{
		auto it = cached_.find(key);
		if (it == cached_.end())
			return nullptr;

		return it->second;
	}

	template<typename Key, typename Value>
	template<typename K, typename V> void Cache<Key, Value>::put(K&& key, V&& value)
	{
		auto insertion = cached_.insert_or_assign(
			std::forward<K>(key),
			std::make_shared<Value>(std::forward<V>(value))
			);

		replacementPolicy(insertion.first, insertion.second);
	}

	template<typename Key, typename Value>
	inline bool Cache<Key, Value>::has(const Key& key) const
	{
		return cached_.contains(key);
	}


	// Fifo

	template<typename Key, typename Value>
	inline FifoCache<Key, Value>::FifoCache(size_t maxSize)
		: Cache<Key, Value>(maxSize)
	{
	}

	template<typename Key, typename Value>
	inline void FifoCache<Key, Value>::replacementPolicy(const FifoCache<Key, Value>::Iterator& element, bool inserted)
	{
		// TODO: impl
		
		// If cache is full, evict element
		if (Cache<Key, Value>::cached_.size() >= Cache<Key, Value>::maxSize_)
		{
			// TODO: impl
			// Should remove an element from the queue but also from the cache map
		}

		// Add key to queue
	}

}
