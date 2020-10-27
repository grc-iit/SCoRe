//
// Created by jaime on 10/27/20.
//

#ifndef SCORE_INSIGHT_H
#define SCORE_INSIGHT_H

#include <functional>

namespace score {
    class Insight {
        void init();
        virtual void execute();
        void finalize();

        void insight();
    public:
        Insight();
        void run();
    };

    class PointInsight: public Insight {
        void init();
        void execute();
        void finalize();

        void insight();
    public:
        PointInsight();
        void run();
    };

    class WindowInsight: public Insight {
        void init();
        void execute();
        void finalize();

        void insight();
    public:
        WindowInsight();
        void run();
    };

    class SumInsight: public PointInsight {
        void insight();

    public:
        SumInsight();
    };

    class RatioInsight: public PointInsight {
        void insight();

    public:
        RatioInsight();
    };
}


#endif //SCORE_INSIGHT_H
