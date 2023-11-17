

// ========================================================================
//		Cache
// ========================================================================

#pragma once

#include <map>
#include <queue>
#include <memory>
#include <assert.h>

#include "Assertions.h"


namespace lmh {

	// Interface
	template<typename Key, typename Value>
	class Cache
	{
	public:

		using ValuePtr = std::shared_ptr<Value>;
		using ConstValuePtr = std::shared_ptr<const Value>;
		using Iterator = std::map<Key, ValuePtr>::iterator;

	public:

		Cache(size_t maxSize);
		virtual ~Cache() = default;
		Cache(const Cache&) = delete;
		Cache(Cache&&) = delete;
		Cache& operator=(const Cache&) = delete;
		Cache& operator=(Cache&&) = delete;

		// Returns a pointer pointing to an element in the cache, if not found, returns nullptr
		ConstValuePtr get(const Key& key) const;
		// Inserts an element in the cache. Returns false if there is an element with that key already stored
		// and performs no insertion
		template<typename K, typename V> 
		bool put(K&& key, V&& value);


		bool has(const Key& key) const;

	private:

		virtual void registerKey(const Iterator& iterator) = 0;
		virtual void evict() = 0;

	protected:

		std::map<Key, ValuePtr> cached_;
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
		
		FifoCache(size_t maxSize = (size_t) - 1);
		virtual ~FifoCache() = default;

	private:

		virtual void registerKey(const Iterator& iterator) override;
		virtual void evict() override;

	private:

		std::queue<Iterator> keys_;
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
	template<typename K, typename V> bool Cache<Key, Value>::put(K&& key, V&& value)
	{
		Iterator iterator;
		bool inserted;

		std::tie(iterator, inserted) = cached_.insert(
			{ std::forward<K>(key), std::make_shared<Value>(std::forward<V>(value)) }
		);

		if (inserted)
		{
			// Register new key 
			registerKey(iterator);

			// Evict element based following the specific policy (only when the cache is full)
			evict();
		}

		return inserted;
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
	inline void FifoCache<Key, Value>::registerKey(const Iterator& iterator)
	{
		keys_.push(iterator);
	}

	template<typename Key, typename Value>
	inline void FifoCache<Key, Value>::evict()
	{
		assert(this->cached_.size() == keys_.size());

		// Keep cache size within limits
		if (this->cached_.size() > this->maxSize_)
		{
			Iterator evicted = keys_.front();
			// Remove cached element
			auto removedElements = this->cached_.erase(evicted);
			// Remove key
			keys_.pop();

			assert(this->cached_.size() == this->maxSize_);
		}
	}

}
