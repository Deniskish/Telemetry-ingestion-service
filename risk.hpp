#pragma once

#include <string>

enum class Decision {
    Allow,
    Review,
    Deny
};

struct RiskRequest {
    std::string user_id;
    double amount;
};

struct RiskResponse {
    Decision decision;
};

int a;