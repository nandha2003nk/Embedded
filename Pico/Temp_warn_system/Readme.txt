Temprature Warning System

1. main()

    1.1 Initialize Pico standard I/O

        → Initialize communication between
          Pico and PC

    1.2 Call application_init()

        → Hand system initialization
          to application layer

    1.3 Enter infinite superloop

        → Repeatedly call application_update()

    1.4 Return control to application
        on every loop iteration

    1.5 Repeat forever

2.1 application_init()

    2.1 Call adc_driver_init()

        → Hand ADC initialization
          to ADC driver

    2.2 Call gpio_driver_init()

        → Hand LED initialization
          to GPIO driver

    2.3 Return to main()

2.2 application_update()

    2.1 Request ADC value

        → Call adc_driver_get_value()

        → Hand control to ADC driver

    2.2 ADC driver reads ADC hardware

        → Returns raw ADC value

    2.3 Receive ADC value

        → Example:
          ADC value = 2478

    2.4 Compare ADC value with threshold

        IF ADC value >= 2000:

            2.4.1 Call gpio_driver_on()

                  → Hand control to GPIO driver

        ELSE:

            2.4.2 Call gpio_driver_off()

                  → Hand control to GPIO driver

    2.5 GPIO driver changes LED state

    2.6 Return to application

    2.7 Return to main()

    2.8 Repeat on next superloop iteration

3.1 adc_driver_init()

    3.1 Initialize ADC peripheral

        → Prepare RP2040 ADC hardware

    3.2 Configure GPIO26 for ADC operation

        → Connect GPIO26 to ADC input

    3.3 Select ADC channel corresponding
        to GPIO26

    3.4 Return to application_init()

3.2 adc_driver_get_value()

    3.1 Start / request ADC conversion

    3.2 Wait for ADC conversion to complete

    3.3 Read ADC result from RP2040 ADC hardware

    3.4 Store result as raw ADC value

        → Example:
          2478

    3.5 Return ADC value to application

4.1 gpio_driver_init()

    4.1 Initialize GPIO25

    4.2 Configure GPIO25 as OUTPUT

    4.3 Set GPIO25 LOW

        → Ensure LED starts OFF

    4.4 Return to application_init() 

4.2 gpio_driver_on()

    4.2.1 Set GPIO25 HIGH

    4.2.2 Physical LED turns ON

    4.2.3 Return to application

4.3 gpio_driver_off()

    4.3.1 Set GPIO25 LOW

    4.3.2 Physical LED turns OFF

    4.3.3 Return to application
