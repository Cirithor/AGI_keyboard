static int TONE_DUETIMER = 8; //Which Timer is used by the DueTimer library

DueTimer ToneTimer = DueTimer(TONE_DUETIMER);

static uint8_t pinEnabled[PINS_COUNT];
static bool TimerEnabled = false;
static boolean pin_state = false;

volatile static int32_t toggle_count;
static uint32_t tone_pin;

// frequency (in hertz) and duration (in milliseconds).
void tone(uint32_t ulPin, uint32_t frequency, int32_t duration)
{
  //unsigned long period = 1000000 / (frequency * 2);
  tone_pin = ulPin;
  toggle_count = 0;  // strange  wipe out previous duration
  if (duration > 0 ) toggle_count = 2 * frequency * duration / 1000;
  else toggle_count = -1;
  if (!pinEnabled[ulPin]) {
    pinMode(ulPin, OUTPUT);
    pinEnabled[ulPin] = 1;
  }
  if (!TimerEnabled) {
    ToneTimer.attachInterrupt(Toggle_Handler).setFrequency(frequency * 2).start();
    TimerEnabled = true;
  }
}

void noTone(uint32_t ulPin)
{
  ToneTimer.stop();  // stop timer
  digitalWrite(ulPin, LOW); // no signal on pin
  TimerEnabled = false;
}

// timer ISR  TC1 ch 0
void Toggle_Handler ( void ) {
  if (toggle_count != 0) {
    // toggle pin  TODO  better
    digitalWrite(tone_pin, pin_state = !pin_state);
    if (toggle_count > 0) toggle_count--;
  } else {
    noTone(tone_pin);
  }
}
