#include "Subject.h"
#include <vector>
#include <algorithm>

namespace state {
    void Subject::addObserver(IObserver* observer) {
        observers.push_back(observer);
    }

    void Subject::removeObserver(IObserver* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void Subject::notifyObservers(StateEventID e) {
        for (IObserver* observer : observers){
            observer->getNotified(e);
        }
    }
}