
// ========================================================================
//		Observable-Observer pattern
// ========================================================================

#pragma once

#include "errors.h"

#include <unordered_set>
#include <set>
#include <cstddef>

namespace lmh {

	// Forward declarations
	class Observer;



	// Object that notifies its changes to its Observers
	class Observable
	{
		friend class Observer;

		using ObserverSet = std::set<Observer*>;
		using Iterator = ObserverSet::iterator;

	public:

		Observable() = default;
		// NB: the observer set is not copied: observers must
		// explicitely register with the copy
		Observable(const Observable& other) = default;
		// This time is slightly different: since the object exists already,
		// we need to notify the observers of the change. (observer set is not copied)
		Observable& operator=(const Observable& other);
		virtual ~Observable() = default;

		// Should be called at the end of every non-const method or whenever needed
		void notifyObservers();

	private:

		std::pair<Iterator, bool> registerObserver(Observer* observer);
		std::size_t unregisterObserver(Observer* observer);

	private: 

		ObserverSet observers_;
	};



	// Object that gets notified when its Observables change
	class Observer
	{
		using ObservableType = std::unordered_set<std::shared_ptr<Observable>>;
		using Iterator = ObservableType::iterator;

	public:

		Observer() = default;
		Observer(const Observer& other);
		Observer& operator=(const Observer& other);
		virtual ~Observer();

		// Returns the iterator corresponding to the inserted observable, and true if succeded
		std::pair<Iterator, bool> registerWith(const std::shared_ptr<Observable>& observable);
		std::size_t unregisterWith(const std::shared_ptr<Observable>& observable);
		void unregisterWithAll();
		// This method must be implemented in the derived class
		// NB: the derived object does never call this method directly,
		// this is done by 'Observable::notifyObservers()' instead
		virtual void update() = 0;

	private:

		ObservableType observables_;
	};
}