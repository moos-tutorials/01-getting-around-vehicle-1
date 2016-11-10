# Vehicle 1 - Getting Around
## Introduction
To get you started on *MOOS-IvP*, let's take a step-by-step approach to making complex vehicles.
[Valentino Braitenberg][1] in his book [Vehicles, Experiments in synthetic psychology][2] builds complex robotic behaviors from very simple sensor to actuator link.

In this first chapter of *MOOS-IvP* tutorials, the first vehicle described will be built.

## Understanding Vehicle 1
**Vehicle 1** is a very simple vehicle that has only one sensor and one actuator.
The output of the sensor is directly linked to the input of the actuator.

![Vehicle 1][3]

Let's consider the sensor a light sensor, and the actuator a motor mounted on a wheel.
To simulate such a system, three applications are required:
- a source generator,
- a sensor interface, and
- a motor controller/vehicle simulator.

### The source
The source application will only indicate the coordinates of the source and it's intensity.
The application to simulate the light source will be named `uLightSource` and configured using:
- the intensity `I`,
- the position of the source set by `SOURCE_X` and `SOURCE_Y`.

### The sensor interface
The sensor interface is simulates and computes the intensity received by the light sensor.
The application to simulates the sensor will be named `uLightSensor` and configured using:
- the sensitivity `S`.

### The vehicle simulator
The first vehicle that will be simulated is extremely simple.
It only goes forward and it's speed is proportional to the intensity given by the light sensor.
Therefore it is possible in the application `uSimVehicle1` to configure:
- the ratio `K` between the intensity given by the vehicle and the output of the motor,
- the `MAX_SPEED` of the vehicle,
- the initial position of the vehicle set by `START_X`, `START_Y`, and `START_HEADING`.

## Building the blocks
**Vehicle 1** experiment is very easy to implement as the system is particularly simple.

This `git` repository contains the following branches:
- `master` branch contains the skeleton and the instructions on how to build the applications
- `01-uLightSource-final` contains a proposed solution of the `uLightSource` application.
- `02-uLightSensor-final` contains a proposed solution for the `uLightSensor` application.
- `03-uSimVehicle1-final` contains a proposed solution for the `uSimVehicle1` application.
- `04-final-project` contains a complete simulation of the system described above.

Once done with each of the following steps, one can checkout the corresponding branch and see the difference between the proposed solution and what was proposed.

### 1 - uLightSource
The light source is the simplest *MOOS* application one can imagine.
Once the application is connected to the `MOOSDB`, it needs to publish the value of the intensity of the source.

First, let's download the repository and make sure we are on the `master` branch:
``` shell
git clone https://github.com/moos-tutorials/01-getting-around-vehicle-1.git
cd 01-getting-around-vehicle-1
git checkout master
```

Let's call the `GenMOOSAppTutorial.sh` script to create the skeleton of our `uLightSource` app:
``` shell
bash ./GenMOOSAppTutorial.sh LightSource u "Your Name Here"
```
A new folder `src` has now appeared with a `CMakeLists.txt` file and a `uLightSource` directory.

Let's add the local attributes of this class called `m_intensity`, `m_x`, and `m_y` to the `LightSource` in `LightSource.h` file.
``` c++
private: // Configuration variables
  double m_intensity;
  double m_x, m_y;
```
And it should be given a default value of `0.0` at the constructor in the `LightSource.cpp`:
``` c++
LightSource::LightSource(): m_intensity(0.0),
  m_x(0.0), m_y(0.0)
{
}
```

#### Reading a value from a configuration file
The value of the intensity can be setup at the configuration as a variable `I`, as of `START_X` and `START_Y`.
This can be done in the `OnStartUp()` method call (the examples `FOO` and `BAR` have been removed) :
``` c++
if(param == "I") {
  m_intensity = atof(line.c_str());
  handled = true;
} else if(param == "START_X") {
  m_x = atof(line.c_str());
  handled = true;
} else if(param == "START_Y") {
  m_y = atof(line.c_str());
  handled = true;
}
```

A good practice is to keep the `*_Info.*` file up-to-date with the development of the current application.
In the `uLightSource_Info.cpp` add the relevant information in the `showExampleConfigAndExit()` function.


#### Publish a variable
Next, the application should publish the intensity at each iteration for instance.
In the `Iterate()` method, let's add the following call that will publish the value of the intensity to the `MOOSDB`:
``` c++
Notify("ULSOURCE_INTENSITY", m_intensity);
Notify("ULSOURCE_POS_X", m_x);
Notify("ULSOURCE_POS_Y", m_y);
```
This *MOOS* function call will publish a variable `ULSOURCE_INTENSITY` with the corresponding value of `m_intensity` immediately.
The `MOOSDB` will also be notified by the position of the source the same way.

#### Subscribing to a variable
It would be helpful to change the value of the intensity dynamically without having to launch the app.
To do so, let the app subscribe to a variable 'ULSOURCE_SET_INTENSITY' in the `registerVariables()` method:
``` c++
Register("ULSOURCE_SET_INTENSITY", 0);
```
And the new value is to be processed in the `OnNewMail()` method:
``` c++
if(key == "ULSOURCE_SET_INTENSITY")
  m_intensity = msg.GetDouble();
```

#### AppCasting
`AppCasting` is the capacity of an app to broadcast relevant information that a human can easily parse.
We will not get into the detail of `AppCasting` here, but our app would give us more information if we can easily acces a report about it's configuration and current status:
``` c++
bool LightSource::buildReport()
{
  m_msgs << "Configutation                                \n";
  m_msgs << "============================================ \n";

  ACTable actab(3);
  actab << "Position X | Position Y | Intensity";
  actab.addHeaderLines();
  actab << m_x << m_y << m_intensity;
  m_msgs << actab.getFormattedString();

  return(true);
}
```

Once the `uLightSource_Info.cpp` file has been updated too, it is time to build the project using the provided `build.sh` script.
``` shell
bash ./build.sh
```

A solution is provided in the bran `01-uLightSource-final`.
To check your code against the provided solution, a proposed way is to [stash][4] the changes and see the difference when the branch has been checked out:
``` shell
git stash
git checkout 01-uLightSource-final
git stash pop
git diff
```

The commands below will print out the differences between your modifications and the ones proposed.
There's multiple ways you can write software and *MOOS* is no exception.
Moreover, `git` will make sure that everything is exactly the same, even the number of space.

You can either overwrite the proposed solution with yours or continue with the solution by stashing back your code (`git stash`).

### 2 - uLightSensor
The light sensor is also not very complicated.
The app must subscribe to :
- `ULSOURCE_INTENSITY`, to read the value of the intensity
- `ULSOURCE_POS_X` and `ULSOURCE_POS_Y` to compute the intensity of illumination at
- `NAV_X`, `NAV_Y` the vehicle's position,
- `ULSENSOR_SET_SENSITIVITY` to set dynamically the sensitivity.

The app should also publish the:
- `ULSENSOR_READING` that can be computed via this equation:
``` c++
ULSENSOR_READING = S/(sqr(NAV_X-ULSOURCE_POS_X)+sqr(NAV_Y-ULSOURCE_POS_Y))
```

Finally, the app should accept a configuration variable:
- the sensitivity `S`.

When done programming the `uLightSensor` application, please cross check your solution with the provided solution in `02-uLightSensor-final`

### 3 - uSimVehicle1
For the vehicle itself it just needs to read the following configutation variables:
- `K` the factor between `ULSENSOR_READING` the vehicle's `NAV_SPEED`.
- `MAX_SPEED` of the vehicle,
- the initial position `START_X`, `START_Y`, and `START_HEADING`.

It is supposed to subscribe to
- `ULSENSOR_READING`.

And it is also supposed to publish the location from a simple Euler integration schema:
- `NAV_X`, `NAV_Y` and `NAV_HEADING`,
- `NAV_SPEED`.

To compute the `NAV_X` and `NAV_Y`:
``` c++
NAV_X = NAV_X + h*cos(headingToRadians(NAV_HEADING))*NAV_SPEED
NAV_Y = NAV_Y + h*sin(headingToRadians(NAV_HEADING))*NAV_SPEED
```

**NOTE:** `NAV_HEADING` is in degrees and zeroed at North.
It is mandatory to use the `headingToRadians()` functions that is part of the `geometry` *MOOS-IvP* library and including the header file `AngleUtils.h`

When done coding the `uSimVehicle1` application, please cross check your solution with the provided solution in `03-uSimVehicle1-final`.





[1]: https://en.wikipedia.org/wiki/Valentino_Braitenberg
[2]: https://books.google.com/books?hl=en&lr=&id=7KkUAT_q_sQC&oi=fnd&pg=PR9&ots=lZiyntKW_a&sig=yAVB7MtRiNauSkoXluJnWfGHZwc
[3]: .img/v1.svg
[4]: https://git-scm.com/docs/git-stash
