#include "Report.h"

Report::Report() {
    counter = 0;
    sum = 0.0;
}

Report::~Report () {}


void Report::setCounter(double x) {
    counter = x;
}

int Report::getCounter () {
    return counter;
}

void Report::setSum(double x) {
    sum = x;
}

double Report::getSum () {
    return sum;
}

void Report::updateCounter() {
    counter++;
}

void Report::updateSum(double x) {
    sum += x;
}

void Report::updateRecord(double x) {
    updateSum(x);
    updateCounter();
}

double Report::getAvg() {
    if (counter!=0)
        return sum/counter;
    else
        return 0;
}

void Report::resetData() {
    counter = 0;
    sum = 0.0;
}
