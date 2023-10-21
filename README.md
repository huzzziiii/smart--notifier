# Notifier App
The app acts a Notifier that keeps the user updated with the relevant information over serial terminal as well as [nRF Connect App](https://apps.apple.com/us/app/nrf-connect-for-mobile/id1054362403) over BLE. The data that's displayed depends on what the user has subscribed to through the supported commands.

## Supported Commands
User can interface with the app over UART and [nRF Connect App](https://apps.apple.com/us/app/nrf-connect-for-mobile/id1054362403), which acts as a central and talks to the peripheral (nRF52 DK) running the FW.

Following are the currently supported commands:

| Commands        | Function      
| ------------- |:------------- | 
| tempOff     | unsubscribes the temperature sensor from the `Notification` class: user would no longer see any updated temperature values
| tempOn  | subscribes the temperature sensor to the `Notification`: user would receive notifications containing temperature values in C°

## Observer Pattern
The sensor class forms a concrete Subject that stores a reference to the Observer which in our case is the Notification class. 

As soon as there's a sensor data ready to be pushed to a notification list, it invokes a corresponding `Subject::notify()` that in turn calls a correspondg update() call of Notification classes that takes care of updating the notification list with a desired data.

This pattern is used for scalability: for X amount of sensors to be used (in the future), each of which could be attached by the `Notification` class. And there's always flexibility to unsubscribe any sensor from the Notification class.

<img width="1008" alt="Screen Shot 2022-01-18 at 4 23 51 PM" src="https://user-images.githubusercontent.com/15020079/150040221-4bba6b52-3282-4a55-ad7a-98a3c334a869.png">

## User Interfacing
1) UART terminal with supported user commands alongside the view from Logic Analyzer

<img width="842" alt="Screen Shot 2022-01-30 at 4 58 04 PM" src="https://user-images.githubusercontent.com/15020079/151725771-1c1c237f-89f9-4c7b-bf38-b054c9bb4dc2.png">

Following is a zoomed-in view of a sensor reading
<img width="1394" alt="Screen Shot 2022-01-30 at 5 06 49 PM" src="https://user-images.githubusercontent.com/15020079/151726045-053e584e-599d-4ecf-88bc-398743ea22ff.png">



## Todo
1. The output over serial terminal seems to be lagging particularly after the scheduler kicks in. Need to investigate.
2. Finish the Logger class
3. Do something meaningful with the notification list -- thoughts: upload data to a server that displays the data graphically

