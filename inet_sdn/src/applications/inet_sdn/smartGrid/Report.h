#ifndef REPORT_H
#define REPORT_H

#include <omnetpp.h>

class Report {
    private:
        int counter;
        double sum;
    public:
        Report();
        ~Report();
        
        int getCounter();
        double getSum();
        
        void setCounter(double x);
        void setSum(double x);
        
        void updateCounter(); 
        void updateSum(double x);
        void updateRecord(double x);
        
        double getAvg();        
        void resetData();
        
};
#endif

