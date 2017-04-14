from network import Network

@given("network")
def step_impl(context):
    context.network = Network()

@given("step")
def step_impl(context):
    context.network.step()
