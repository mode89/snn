#include <gtest/gtest.h>
#include <snn/group.h>
#include <snn/network.h>

TEST(SNN, Simple)
{
    snn::network network(1000);
    auto gE = network.add_group(1000);
    network.connect(gE, gE, 0.1f, -0.5f, 0.5f);

    for (int time = 0; time < 1000; ++ time)
    {
        gE->generate_random_input();
        network.find_fired_neurons();
        network.reset_fired_neurons();
        network.deliver_spikes();
        network.update_potentials();
    }
}

TEST(SNN, UpdateWeights)
{
    snn::network network(1000);
    for (int time = 0; time < 1000; ++ time)
    {
        network.generate_random_input();
        network.process_firings();
        network.update_weights();
        network.update_potentials();
    }
}
