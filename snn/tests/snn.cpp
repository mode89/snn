#include <gtest/gtest.h>
#include <snn/network.h>

TEST(SNN, Simple)
{
    snn::network network(1000);
    for (int time = 0; time < 1000; ++ time)
    {
        network.generate_random_input();
        network.process_firings();
        network.update_potentials();
    }
}
