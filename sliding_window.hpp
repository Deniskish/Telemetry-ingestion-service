#pragma once

#include <chrono>
#include <deque>
#include <mutex>
#include <string>
#include <unordered_map>

class SlidingWindow {
public:
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    void addPayment(const std::string& userId, TimePoint now = Clock::now()){
        std::lock_guard<std::mutex> lock(mutex_);

        auto& events = payments_[userId];

        events.push_back(now);

        removeOld(events, now);
    }

    int countPayments(const std::string& userId,TimePoint now = Clock::now()){
        std::lock_guard<std::mutex> lock(mutex_);

        auto it = payments_.find(userId);

        if (it == payments_.end()) {
            return 0;
        }

        removeOld(it->second, now);

        return static_cast<int>(it->second.size());
    }

private:
    void removeOld(std::deque<TimePoint>& events, TimePoint now){
        const auto border = now - std::chrono::minutes(10);

        while (!events.empty() && events.front() < border){
            events.pop_front();
        }
    }

private:
    std::unordered_map<std::string, std::deque<TimePoint>> payments_;

    std::mutex mutex_;
};