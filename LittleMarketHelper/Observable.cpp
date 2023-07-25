
#include "observable.h"


namespace lmh {

	// Observable definitions

	Observable& Observable::operator=(const Observable& other)
	{
		// TODO: the notification is sent before the data is actually copied,
		// so 'update()' is actually called too soon!
		if (this != &other)
		{
			this->notifyObservers();
		}

		return *this;
	}

	void Observable::notifyObservers()
	{
		if (observers_.empty()) return;

		bool successful = true;
		std::string errorMsg;

		for (auto& observer : observers_)
		{
			try
			{
				observer->update();
			}
			// This allows to finish the loop before throwing
			catch (std::exception& exception)
			{
				successful = false;
				errorMsg = exception.what();
			}
			catch (...)
			{
				successful = false;
				errorMsg = "Unknown reason";
			}
		}

		ENSURE(successful, "could not notify one or more Observers:  " << errorMsg);
	}

	std::pair<Observable::Iterator, bool> Observable::registerObserver(Observer* observer)
	{
		return observers_.insert(observer);
	}

	std::size_t Observable::unregisterObserver(Observer* observer)
	{
		return observers_.erase(observer);
	}


	// Observer definitions

	Observer::Observer(const Observer& other)
		: observables_(other.observables_)
	{
		for (auto& observable : observables_)
		{
			observable->registerObserver(this);
		}
	}

	Observer& Observer::operator=(const Observer& other)
	{
		// Unregister from existing observables...
		for (const auto& observable : observables_)
		{
			observable->unregisterObserver(this);
		}

		// ...and then register to the new ones
		observables_ = other.observables_;
		for (const auto& observable : observables_)
		{
			observable->registerObserver(this);
		}

		return *this;
	}

	Observer::~Observer()
	{
		for (const auto& observable : observables_)
		{
			observable->unregisterObserver(this);
		}
	}

	std::pair<Observer::Iterator, bool> Observer::registerWith(const std::shared_ptr<Observable>& observable)
	{
		if (observable)
		{
			observable->registerObserver(this);
			return observables_.insert(observable);
		}

		return std::make_pair(observables_.end(), false);
	}

	std::size_t Observer::unregisterWith(const std::shared_ptr<Observable>& observable)
	{
		if (observable)
		{
			observable->unregisterObserver(this);
		}

		return observables_.erase(observable);
	}

	void Observer::unregisterWithAll()
	{
		for (const auto& observable : observables_)
		{
			observable->unregisterObserver(this);
		}

		observables_.clear();
	}

}