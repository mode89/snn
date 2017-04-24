Feature: Core functionality

    Scenario: Construct network
        Given network with 100 neurons

    Scenario: Step network
        Given network with 100 neurons
        And update

    Scenario: Plot
        Given network with 1000 neurons
        And monitor
        When update 1000 times
        Then plot output
