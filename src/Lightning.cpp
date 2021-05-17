
#include <Lightning.h>

void Lightning::isr() {
  interrupted = true;
}

bool Lightning::setup() {

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV16);   // Set the SPI speed to SPI_CLOCK_DIV16/1MHz max 2MHz, NEVER 500kHz
  SPI.setDataMode(SPI_MODE1);
  SPI.setBitOrder(MSBFIRST);

  if (!as3935.begin())
  {
    log.println("L: begin() failed. check your AS3935 Interface setting.");
    return false;
  }

  //check SPI connection.
  if (!as3935.checkConnection())
  {
    log.println("L: checkConnection() failed. check your SPI connection and SPI chip select pin. ");
    return false;
  }
  else
    log.println("L: SPI connection check passed. ");

  //check the IRQ pin connection.
  if (!as3935.checkIRQ())
  {
    log.println("L: checkIRQ() failed. check if the correct IRQ pin was passed to the AS3935SPI constructor. ");
    return false;
  }
  else
    log.println("L: IRQ pin connection check passed. ");

  //calibrate the resonance frequency. failing the resonance frequency could indicate an issue 
  //of the sensor. resonance frequency calibration will take about 1.7 seconds to complete.
  int32_t frequency = 0;
  if (!as3935.calibrateResonanceFrequency(frequency))
  {
    log.print("L: Resonance Frequency Calibration failed: is ");
    log.print(frequency);
    log.println(" Hz, should be 482500 Hz - 517500 Hz");
    return false;
  }
  else
    log.println("L: Resonance Frequency Calibration passed. ");

  log.print("L: Resonance Frequency is "); log.print(frequency); log.println(" Hz");

	//calibrate the RCO.
	if (!as3935.calibrateRCO())
	{
		log.println("L: RCO Calibration failed. ");
		return false;
	}
	else
		log.println("L: RCO Calibration passed. ");

	//set the analog front end to 'indoors'
	as3935.writeAFE(AS3935MI::AS3935_INDOORS);

	//set default value for noise floor threshold
	as3935.writeNoiseFloorThreshold(AS3935MI::AS3935_NFL_2);

	//set the default Watchdog Threshold
	as3935.writeWatchdogThreshold(AS3935MI::AS3935_WDTH_2);

	//set the default Spike Rejection 
	as3935.writeSpikeRejection(AS3935MI::AS3935_SREJ_2);

	//write default value for minimum lightnings (1)
	as3935.writeMinLightnings(AS3935MI::AS3935_MNL_1);

	//do not mask disturbers
	as3935.writeMaskDisturbers(false);

	log.println("L: Initialization complete, waiting for events...");

  return true;
}

void Lightning::loop() {
  uint8_t wdth;
  uint8_t srej;

  long now = millis();

  if (interrupted) {
    delay(2);
    interrupted = false;

    switch (as3935.readInterruptSource()) {
      case AS3935MI::AS3935_INT_NH:
        log.println("L: Noise floor too high. attempting to increase noise floor threshold. ");

        // if the noise floor threshold setting is not yet maxed out, increase the setting.
        // note that noise floor threshold events can also be triggered by an incorrect
        // analog front end setting.
        if (as3935.increaseNoiseFloorThreshold())
          log.println("L: Increased noise floor threshold");
        else
          log.println("L: Noise floor threshold already at maximum");
        break;
      case AS3935MI::AS3935_INT_D:
        log.println("L: Disturber detected, attempting to increase noise floor threshold. ");

        // increasing the Watchdog Threshold and / or Spike Rejection setting improves the AS3935s resistance 
        // against disturbers but also decrease the lightning detection efficiency (see AS3935 datasheet)
        wdth = as3935.readWatchdogThreshold();
        srej = as3935.readSpikeRejection();

        if ((wdth < AS3935MI::AS3935_WDTH_10) || (srej < AS3935MI::AS3935_SREJ_10))
        {
          senseAdjLast = now;

          //alternatively increase spike rejection and watchdog threshold 
          if (srej < wdth)
          {
            if (as3935.increaseSpikeRejection())
              log.println("L: Increased spike rejection ratio");
            else
              log.println("L: Spike rejection ratio already at maximum");
          }
          else
          {
            if (as3935.increaseWatchdogThreshold())
              log.println("L: Increased watchdog threshold");
            else
              log.println("L: Watchdog threshold already at maximum");
          }
        }
        else
        {
          log.println("L: Error: Watchdog Threshold and Spike Rejection settings are already maxed out.");
        }
        break;
      case AS3935MI::AS3935_INT_L:
        log.print("L: Lightning detected! Storm Front is ");
        log.print(as3935.readStormDistance());
        log.println("km away.");
        break;
    }
  }

  //increase sensor sensitivity every once in a while. SENSE_INCREASE_INTERVAL controls how quickly the code 
  //attempts to increase sensitivity. 
  if (now - senseAdjLast > SENSE_INCREASE_INTERVAL)
  {
    senseAdjLast = now;

    log.println("L: No disturber detected, attempting to decrease noise floor threshold.");

    wdth = as3935.readWatchdogThreshold();
    srej = as3935.readSpikeRejection();

    if ((wdth > AS3935MI::AS3935_WDTH_0) || (srej > AS3935MI::AS3935_SREJ_0))
    {
      //alternatively derease spike rejection and watchdog threshold 
      if (srej > wdth)
      {
        if (as3935.decreaseSpikeRejection())
          log.println("L: Decreased spike rejection ratio");
        else
          log.println("L: Spike rejection ratio already at minimum");
      }
      else
      {
        if (as3935.decreaseWatchdogThreshold())
          log.println("L: Decreased watchdog threshold");
        else
          log.println("L: Watchdog threshold already at minimum");
      }
    }
  }

}
