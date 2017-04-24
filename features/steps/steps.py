from network import Monitor, Network
import matplotlib.pyplot as plt

@given("network with {neuron_num:d} neurons")
def step_impl(context, neuron_num):
    context.network = Network(neuron_num)

@given("update")
def step_impl(context):
    context.network.update()

@given("monitor")
def step_impl(context):
    context.monitor = Monitor(context.network)

@when("update {n:d} times")
def step_impl(context, n):
    for i in range(n):
        context.network.update()
        if hasattr(context, "monitor"):
            context.monitor.update()

@then("plot output")
def step_impl(context):
    plt.scatter(
        context.monitor.x,
        context.monitor.y,
        color="black",
        marker=".")
    plt.show()
