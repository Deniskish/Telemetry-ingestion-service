#include "risk.hpp"
#include "sliding_window.hpp"

#include <chrono>
#include <iostream>

RiskResponse evaluate(
    const RiskRequest& request,
    int payments10m)
{
    RiskResponse response;

    if (request.amount > 50000 &&
        payments10m > 10)
    {
        response.decision = Decision::Review;
    }
    else {
        response.decision = Decision::Allow;
    }

    return response;
}

int main()
{
    SlidingWindow metrics;

    RiskRequest request;

    request.user_id = "user42";
    request.amount = 70000;

    // Имитируем историю пользователя:
    // он совершил 11 предыдущих платежей.
    for (int i = 0; i < 11; ++i) {
        metrics.addPayment(request.user_id);
    }

    // Пришла текущая операция.
    metrics.addPayment(request.user_id);

    int payments10m =
        metrics.countPayments(request.user_id);

    std::cout
        << "Payments in last 10 minutes: "
        << payments10m
        << '\n';

    RiskResponse response =
        evaluate(request, payments10m);

    if (response.decision == Decision::Review) {
        std::cout << "Decision: REVIEW\n";
    }
    else {
        std::cout << "Decision: ALLOW\n";
    }

    return 0;
}