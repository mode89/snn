Feature: Core functionality

    Scenario: Construct network
        Given network

    Scenario: Step network
        Given network
        And step

    Scenario: Plot
        Given network
        And monitor
        When step 1000 times
        Then plot output
